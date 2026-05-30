# OctarinePackage.cmake — desktop packaging (Android packages via Gradle; iOS parked on defer/ios).
#
# Produces a self-contained, double-clickable artifact per OS that launches with NO `-p` argument:
# the engine binary sits beside a baked game project (assets + lua + config + asset_manifest.lua),
# and `SDL_GetBasePath()` resolves the project dir at the exe/bundle location. The baked manifest
# is what removes the runtime filesystem scan, so the same code works inside a read-only bundle.
#
# Usage (from the top-level CMakeLists, behind an opt-in):
#   include(cmake/OctarinePackage.cmake)
#   octarine_package(OctarineEngine PROJECT <game-project-dir> [NAME <pkg-name>])
#
# The engine resolves all of fonts/images/sounds/scripts/etc. by recursively scanning a single base
# dir, so we stage the whole PROJECT directory rather than separate assets/ + lua/ trees. PROJECT
# must contain the project's `config` and startup script; the bake step writes asset_manifest.lua
# into the staged copy.
#
# Layout:
#   octarine_package                  — public entrypoint, orchestrates the steps below
#   octarine_read_project_ini         — flat key=value INI -> ${PREFIX}_<key> vars (public; gradle mirrors)
#   _octarine_resolve_identity        — CLI cache > project.ini > default precedence per identity key
#   _octarine_validate_identity       — shipping-build identity gate (FATAL on bad/missing required keys)
#   _octarine_generate_desktop_icons  — runs scripts/octarine-icons.cmake for the host OS
#   _octarine_setup_desktop_install   — install(TARGETS) + bake CODE step + project DIRECTORY copy
#   _octarine_bundle_runtime_libs     — vcpkg dylibs/sos/DLLs + MSVC CRT redist beside the binary
#   _octarine_setup_cpack             — CPACK_* vars + generator pick + include(CPack)

# Guard against double-include.
if (DEFINED _OCTARINE_PACKAGE_INCLUDED)
    return()
endif ()
set(_OCTARINE_PACKAGE_INCLUDED ON)

# Capture this file's directory at include time so functions can resolve sibling paths reliably.
# CMAKE_CURRENT_LIST_DIR inside a function reflects the caller's listfile, not the definition site.
set(_OCTARINE_PACKAGE_DIR "${CMAKE_CURRENT_LIST_DIR}")

# Path to the shared icon/splash generator. Same script Gradle runs on Android — single source of
# truth for sizes/manifests so desktop/Android can't drift. CMAKE_CURRENT_LIST_DIR resolves to
# the directory of THIS file (cmake/) regardless of which CMakeLists.txt includes it.
set(_OCTARINE_ICON_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/../scripts/octarine-icons.cmake")

# Third-party license aggregator. Same file lives at cmake/octarine-licenses.cmake; including it
# here keeps the public octarine_collect_licenses() symbol available to the desktop branch of
# octarine_package.
include("${CMAKE_CURRENT_LIST_DIR}/octarine-licenses.cmake")

# NSIS is opt-in, not auto-detected: a stray/non-functional makensis on PATH (e.g. a Chocolatey
# shim) would otherwise make plain `cpack` fail before producing the reliable ZIP. Turn this ON only
# on a machine with a working NSIS install.
option(OCTARINE_PACKAGE_NSIS "Also emit an NSIS installer on Windows (requires a working makensis)" OFF)

# CLI overrides for project identity (precedence: CLI cache var > project.ini > built-in default).
# Empty default = "not set on the CLI"; the helper falls through to the file or default.
set(OCTARINE_PACKAGE_NAME         "" CACHE STRING "Override project.ini: name")
set(OCTARINE_PACKAGE_VERSION_NAME "" CACHE STRING "Override project.ini: version_name")
set(OCTARINE_PACKAGE_VERSION_CODE "" CACHE STRING "Override project.ini: version_code")
set(OCTARINE_PACKAGE_VENDOR       "" CACHE STRING "Override project.ini: vendor")
set(OCTARINE_PACKAGE_DESCRIPTION  "" CACHE STRING "Override project.ini: description")
set(OCTARINE_PACKAGE_ID           "" CACHE STRING "Override project.ini: package_id (Android bundle id)")
set(OCTARINE_PACKAGE_ORIENTATION  "" CACHE STRING "Override project.ini: orientation (portrait|landscape|all)")
set(OCTARINE_PACKAGE_FULLSCREEN   "" CACHE STRING "Override project.ini: fullscreen (true|false)")
set(OCTARINE_PACKAGE_PERMISSIONS  "" CACHE STRING "Override project.ini: permissions (comma list: internet,recording,camera,location,photos)")
set(OCTARINE_PACKAGE_CATEGORY     "" CACHE STRING "Override project.ini: category (reserved for Android category surfaces; currently informational)")

# Parse a flat key=value INI (no sections; Java Properties compatible) into ${PREFIX}_<key> vars in
# the caller's scope. Skips blank lines and `#`-prefixed comments. Unknown keys are still set —
# callers consume only the keys they recognize.
function(octarine_read_project_ini PROJECT_DIR PREFIX)
    set(_ini "${PROJECT_DIR}/project.ini")
    if (NOT EXISTS "${_ini}")
        return()
    endif ()
    file(STRINGS "${_ini}" _lines)
    foreach (_line IN LISTS _lines)
        # Strip surrounding whitespace by matching against a tolerant regex.
        if (_line MATCHES "^[ \t]*#")
            continue()
        endif ()
        if (_line MATCHES "^[ \t]*$")
            continue()
        endif ()
        if (_line MATCHES "^[ \t]*([A-Za-z0-9_]+)[ \t]*=[ \t]*(.*)$")
            set(_key "${CMAKE_MATCH_1}")
            set(_val "${CMAKE_MATCH_2}")
            # Trim trailing whitespace from the value.
            string(REGEX REPLACE "[ \t]+$" "" _val "${_val}")
            set("${PREFIX}_${_key}" "${_val}" PARENT_SCOPE)
        else ()
            message(WARNING "octarine_read_project_ini: ignoring malformed line in ${_ini}: ${_line}")
        endif ()
    endforeach ()
endfunction()

# Resolve a single identity key: CLI cache override > project.ini value > built-in default.
# Sets ${OUT_VAR} in the caller's scope.
function(_octarine_resolve_identity OUT_VAR CLI_VAL INI_VAL DEFAULT_VAL)
    if (NOT "${CLI_VAL}" STREQUAL "")
        set(${OUT_VAR} "${CLI_VAL}" PARENT_SCOPE)
    elseif (NOT "${INI_VAL}" STREQUAL "")
        set(${OUT_VAR} "${INI_VAL}" PARENT_SCOPE)
    else ()
        set(${OUT_VAR} "${DEFAULT_VAL}" PARENT_SCOPE)
    endif ()
endfunction()

# Validate project identity for a shipping build. Policy:
#  - File absent + SHIPPED → warn (the project hasn't supplied an identity; we'll use defaults).
#  - File present + SHIPPED → required keys must be non-empty; package_id must be reverse-DNS.
#  - SHIPPED off → no-op (dev builds don't gate on identity).
# Fail-fast at configure time so the build doesn't burn cycles producing a misnamed package.
function(_octarine_validate_identity PROJECT_DIR PKG_NAME PKG_ID PKG_VER SHIPPED)
    set(_ini "${PROJECT_DIR}/project.ini")
    if (NOT SHIPPED)
        return()
    endif ()
    if (NOT EXISTS "${_ini}")
        message(WARNING "Octarine: shipping build with no ${_ini} — using fallback identity ('${PKG_NAME}' ${PKG_VER}). Add a project.ini for a proper package identity.")
        return()
    endif ()
    set(_errors "")
    if ("${PKG_NAME}" STREQUAL "")
        list(APPEND _errors "missing required key: name")
    endif ()
    if ("${PKG_VER}" STREQUAL "")
        list(APPEND _errors "missing required key: version_name")
    endif ()
    if ("${PKG_ID}" STREQUAL "")
        list(APPEND _errors "missing required key: package_id (Android bundle id)")
    elseif (NOT "${PKG_ID}" MATCHES "^[a-z][a-z0-9_]*(\\.[a-z][a-z0-9_]*)+$")
        list(APPEND _errors "package_id '${PKG_ID}' must be reverse-DNS (e.g. com.studio.mygame)")
    endif ()
    if (_errors)
        string(REPLACE ";" "\n  - " _err_lines "${_errors}")
        message(FATAL_ERROR "Octarine: project.ini validation failed (${_ini}):\n  - ${_err_lines}")
    endif ()
endfunction()

# iOS bundle helper, orientation/permission Info.plist mappers, and Settings.bundle wiring are
# parked on the defer/ios branch pending an Apple Developer account; see ai/iOSDeferralPlan.md.

# Generate per-OS icon files (.ico/.icns/.png) under ${OUT_DIR}/desktop. Sets ${ICO_VAR},
# ${ICNS_VAR}, ${PNG_VAR} in the caller's scope to the conventional output paths (callers test
# EXISTS before consuming, since the script skip-warns when project.ini has no `icon=`).
function(_octarine_generate_desktop_icons PROJECT_DIR OUT_DIR OS_NAME ICO_VAR ICNS_VAR PNG_VAR)
    execute_process(
            COMMAND "${CMAKE_COMMAND}"
                    "-DOCTARINE_ICON_PROJECT=${PROJECT_DIR}"
                    "-DOCTARINE_ICON_PLATFORM=desktop"
                    "-DOCTARINE_ICON_DESKTOP_OS=${OS_NAME}"
                    "-DOCTARINE_ICON_OUT_DIR=${OUT_DIR}"
                    -P "${_OCTARINE_ICON_SCRIPT}"
            RESULT_VARIABLE _icon_rc)
    if (NOT _icon_rc EQUAL 0)
        message(FATAL_ERROR "octarine_package: desktop icon generation failed (rc=${_icon_rc})")
    endif ()
    set(${ICO_VAR}  "${OUT_DIR}/desktop/octarine_icon.ico"  PARENT_SCOPE)
    set(${ICNS_VAR} "${OUT_DIR}/desktop/octarine_icon.icns" PARENT_SCOPE)
    set(${PNG_VAR}  "${OUT_DIR}/desktop/octarine_icon.png"  PARENT_SCOPE)
endfunction()

# Desktop deliverable layout + install rules:
#   - install(TARGETS) puts the binary at the package root (or .app/Contents/MacOS for macOS).
#   - install(CODE ...) runs the just-built binary in bake mode to produce asset_manifest.lua — the
#     CI gate: a broken asset reference exits nonzero and aborts the install.
#   - install(DIRECTORY) stages the project (now including the baked manifest) next to the binary.
function(_octarine_setup_desktop_install TARGET PROJECT_DIR RUNTIME_DEST DATA_DEST)
    install(TARGETS ${TARGET}
            RUNTIME DESTINATION "${RUNTIME_DEST}"
            BUNDLE DESTINATION "${RUNTIME_DEST}"
    )

    # Aggregate third-party license text at configure time. Desktop convention:
    #   - macOS .app: file lands in Contents/Resources/ (DATA_DEST) so SDL_GetBasePath() can find
    #     it from the running binary if a future about-screen surfaces it.
    #   - Windows/Linux: file lands at the package root beside the binary (DATA_DEST == ".").
    # Repo-root LICENSE is also installed at the package root as a stand-alone file — redundant
    # with the aggregated copy but expected by users / store reviewers.
    set(_licenses_out "${CMAKE_BINARY_DIR}/THIRD_PARTY_LICENSES.txt")
    set(_project_license_drops "${PROJECT_DIR}/THIRD_PARTY_LICENSES.d")
    set(_extra_dirs "")
    if (IS_DIRECTORY "${_project_license_drops}")
        list(APPEND _extra_dirs "${_project_license_drops}")
    endif ()
    # Editor-only assets (Roboto-Medium.ttf, Apache-2.0) are embedded into the binary when
    # OCTARINE_WITH_EDITOR=ON. ship-release forces it off, so the shipped aggregate carries no
    # Roboto section then — only editor-* desktop packages ship the binary that needs the notice.
    set(_editor_fonts_arg "")
    if (OCTARINE_WITH_EDITOR)
        set(_editor_fonts_arg INCLUDE_EDITOR_FONTS)
    endif ()
    octarine_collect_licenses("${_licenses_out}" EXTRA_DIRS ${_extra_dirs} ${_editor_fonts_arg})
    install(FILES "${_licenses_out}" DESTINATION "${DATA_DEST}")
    set(_engine_license "${_OCTARINE_PACKAGE_DIR}/../LICENSE")
    if (EXISTS "${_engine_license}")
        install(FILES "${_engine_license}" DESTINATION "${RUNTIME_DEST}")
    endif ()

    # install() steps run in order, so the bake CODE precedes the DIRECTORY copy below.
    install(CODE "
        message(STATUS \"Octarine: baking asset manifest for package...\")
        execute_process(
            COMMAND \"$<TARGET_FILE:${TARGET}>\" \"${PROJECT_DIR}\" -m bake
            RESULT_VARIABLE _bake_rc)
        if (NOT _bake_rc EQUAL 0)
            message(FATAL_ERROR \"Octarine: asset bake failed (rc=\${_bake_rc}); aborting package\")
        endif ()
    ")

    # Stage the whole project dir beside the binary. Exclude editor/runtime scratch that isn't part
    # of the shipped game (user prefs, imgui layout). asset_manifest.lua is produced by the bake
    # above and lives in PROJECT_DIR, so it rides along here.
    install(DIRECTORY "${PROJECT_DIR}/"
            DESTINATION "${DATA_DEST}"
            PATTERN ".git" EXCLUDE
            PATTERN ".gitignore" EXCLUDE
            PATTERN "*.meta" EXCLUDE
            PATTERN "editor_prefs.ini" EXCLUDE
            PATTERN "preferences.ini" EXCLUDE
            PATTERN "imgui.ini" EXCLUDE
    )
endfunction()

# Bundle vcpkg runtime libs + C/C++ runtime beside the binary.
#
# vcpkg runtime libs: default vcpkg triplets on Linux/macOS (x64-linux, *-osx) are static, so the
# glob hits nothing and the step is a no-op. A dynamic triplet or SHARED build of SDL3 drops
# .so/.dylib/.dll alongside the build-tree binary; those need to ship with the package. We glob
# TARGET_FILE_DIR rather than $<TARGET_RUNTIME_DLLS> because vcpkg's app-local copy includes the
# FULL transitive set (freetype, libpng, jpeg, zlib, ...) and the genex only lists direct imports
# — missing a transitive dep yields a load-time failure (0xC0000135 on Windows; ENOENT/dyld at
# launch on Linux/macOS). FOLLOW_SYMLINK_CHAIN copies the libfoo.so → libfoo.so.1 → libfoo.so.1.2.3
# versioned chain whole rather than dropping a dangling symlink.
#
# C/C++ runtime: MSVC ships from the COMPILER'S OWN redist folder, not InstallRequiredSystemLibraries
# (whose redist finder can latch onto a different VS install — e.g. a leftover VS2019 — bundling
# an older MSVCP140.dll that access-violates against a binary built by a newer toolset; the app-dir
# DLL shadows the matching System32 one at launch and the exe dies before its first log line).
# Layout: <VC>/Tools/MSVC/<ver>/bin/Host<a>/<a>/cl.exe -> <VC>/Redist/MSVC/<rver>/<arch>/Microsoft.VC<toolset>.CRT/*.dll
# (7 levels up from cl.exe reaches the VC dir: x64/Host<a>/bin/<ver>/MSVC/Tools/VC).
function(_octarine_bundle_runtime_libs TARGET RUNTIME_DEST FRAMEWORK_DEST)
    if (WIN32)
        install(CODE "
            file(GLOB _octarine_runtime_dlls \"$<TARGET_FILE_DIR:${TARGET}>/*.dll\")
            if (_octarine_runtime_dlls)
                file(INSTALL \${_octarine_runtime_dlls} DESTINATION \"\${CMAKE_INSTALL_PREFIX}/${RUNTIME_DEST}\")
            endif ()
        ")
    elseif (APPLE)
        # INSTALL_RPATH on the binary points the loader at Frameworks/. vcpkg dylibs use
        # @rpath/... install_names, so the rpath rewrite alone is enough — no install_name_tool
        # dance needed unless a future dep ships with an absolute install_name (at which point
        # swap this for BundleUtilities::fixup_bundle).
        set_target_properties(${TARGET} PROPERTIES INSTALL_RPATH "@executable_path/../Frameworks")
        install(CODE "
            file(GLOB _octarine_runtime_libs \"$<TARGET_FILE_DIR:${TARGET}>/*.dylib\")
            if (_octarine_runtime_libs)
                file(INSTALL \${_octarine_runtime_libs}
                     DESTINATION \"\${CMAKE_INSTALL_PREFIX}/${FRAMEWORK_DEST}\"
                     FOLLOW_SYMLINK_CHAIN)
            endif ()
        ")
    else ()
        # Linux: libs go beside the binary; $ORIGIN tells the dynamic linker to look there.
        set_target_properties(${TARGET} PROPERTIES INSTALL_RPATH "$ORIGIN")
        install(CODE "
            file(GLOB _octarine_runtime_libs
                 \"$<TARGET_FILE_DIR:${TARGET}>/*.so\"
                 \"$<TARGET_FILE_DIR:${TARGET}>/*.so.*\")
            if (_octarine_runtime_libs)
                file(INSTALL \${_octarine_runtime_libs}
                     DESTINATION \"\${CMAKE_INSTALL_PREFIX}/${RUNTIME_DEST}\"
                     FOLLOW_SYMLINK_CHAIN)
            endif ()
        ")
    endif ()

    if (MSVC)
        get_filename_component(_vc_root "${CMAKE_CXX_COMPILER}/../../../../../../.." ABSOLUTE)
        file(GLOB _crt_dirs "${_vc_root}/Redist/MSVC/*/x64/Microsoft.VC${MSVC_TOOLSET_VERSION}.CRT")
        list(SORT _crt_dirs)
        list(REVERSE _crt_dirs)   # newest redist version first
        set(_crt_found OFF)
        foreach (_crt IN LISTS _crt_dirs)
            file(GLOB _crt_dlls "${_crt}/*.dll")
            if (_crt_dlls)
                message(STATUS "Octarine: bundling MSVC CRT from ${_crt}")
                install(PROGRAMS ${_crt_dlls} DESTINATION "${RUNTIME_DEST}")
                set(_crt_found ON)
                break()
            endif ()
        endforeach ()
        if (NOT _crt_found)
            message(WARNING "Octarine: no MSVC CRT redist found under ${_vc_root}/Redist/MSVC — "
                            "the package will depend on the VC++ Redistributable being installed.")
        endif ()
    else ()
        # Non-MSVC (Linux libgcc/libstdc++ when present). SKIP suppresses the module's own bin/ rule
        # so we place the libs at the deliverable root.
        set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP ON)
        include(InstallRequiredSystemLibraries)
        if (CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS)
            install(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION "${RUNTIME_DEST}")
        endif ()
    endif ()
endfunction()

# CPack identity + generator pick + include. CPACK_* are plain (non-cache) vars consumed by the
# CPack module at include time; setting them here in the helper's scope works because include()
# evaluates in the calling scope. NSIS installer/uninstaller icons land here too (the .ico path
# must use forward slashes — NSIS barfs on backslashes inside CPACK_NSIS_*).
function(_octarine_setup_cpack PKG_NAME PKG_VERSION PKG_DESCRIPTION PKG_VENDOR DESKTOP_ICO)
    set(CPACK_PACKAGE_NAME "${PKG_NAME}")
    set(CPACK_PACKAGE_VERSION "${PKG_VERSION}")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PKG_DESCRIPTION}")
    set(CPACK_PACKAGE_VENDOR "${PKG_VENDOR}")
    message(STATUS "Octarine: package identity '${PKG_NAME}' ${PKG_VERSION} (vendor: ${PKG_VENDOR})")
    # Keep the default single top-level <name>-<version>/ folder: unzip yields one self-contained dir.

    if (WIN32)
        # ZIP is the portable single-folder drop; NSIS installer is opt-in (see option above).
        set(CPACK_GENERATOR "ZIP")
        if (OCTARINE_PACKAGE_NSIS)
            list(APPEND CPACK_GENERATOR "NSIS")
            if (EXISTS "${DESKTOP_ICO}")
                file(TO_CMAKE_PATH "${DESKTOP_ICO}" _ico_fwd)
                set(CPACK_NSIS_MUI_ICON "${_ico_fwd}")
                set(CPACK_NSIS_MUI_UNIICON "${_ico_fwd}")
                set(CPACK_PACKAGE_ICON "${_ico_fwd}")
            endif ()
        endif ()
    elseif (APPLE)
        # .app drag-installer.
        set(CPACK_GENERATOR "DragNDrop")
    else ()
        # Portable tarball; AppImage is a later opt-in.
        set(CPACK_GENERATOR "TGZ")
    endif ()

    include(CPack)
endfunction()

function(octarine_package TARGET)
    set(options "")
    set(oneValueArgs PROJECT NAME)
    set(multiValueArgs "")
    cmake_parse_arguments(OP "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT OP_PROJECT)
        message(FATAL_ERROR "octarine_package: PROJECT <game-project-dir> is required")
    endif ()
    if (NOT IS_DIRECTORY "${OP_PROJECT}")
        message(FATAL_ERROR "octarine_package: PROJECT dir does not exist: ${OP_PROJECT}")
    endif ()
    if (NOT OP_NAME)
        set(OP_NAME "${TARGET}")
    endif ()

    # A packaged binary MUST load the baked manifest — it has no real FS to scan inside a bundle.
    # The canonical entry is the `ship-release` CMake preset (carries OCTARINE_SHIPPED=ON); this
    # force is the guardrail so a package configured from a non-ship preset still can't silently
    # ship a binary that scans and dies at launch. Belt-and-suspenders with the preset.
    target_compile_definitions(${TARGET} PRIVATE OCTARINE_SHIPPED)
    message(STATUS "Octarine: packaging '${OP_NAME}' from project '${OP_PROJECT}' (OCTARINE_SHIPPED forced ON)")

    # ---- Project identity ---------------------------------------------------------------------
    # Precedence: CLI cache override > project.ini > default.
    octarine_read_project_ini("${OP_PROJECT}" _pi)
    _octarine_resolve_identity(_pkg_name        "${OCTARINE_PACKAGE_NAME}"         "${_pi_name}"         "${OP_NAME}")
    _octarine_resolve_identity(_pkg_version     "${OCTARINE_PACKAGE_VERSION_NAME}" "${_pi_version_name}" "${PROJECT_VERSION}")
    _octarine_resolve_identity(_pkg_version_code "${OCTARINE_PACKAGE_VERSION_CODE}" "${_pi_version_code}" "1")
    _octarine_resolve_identity(_pkg_vendor      "${OCTARINE_PACKAGE_VENDOR}"       "${_pi_vendor}"       "Octarine")
    _octarine_resolve_identity(_pkg_description "${OCTARINE_PACKAGE_DESCRIPTION}"  "${_pi_description}"  "${PROJECT_DESCRIPTION}")
    _octarine_resolve_identity(_pkg_id          "${OCTARINE_PACKAGE_ID}"           "${_pi_package_id}"   "")
    _octarine_validate_identity("${OP_PROJECT}" "${_pkg_name}" "${_pkg_id}" "${_pkg_version}" ON)

    # ---- Per-project platform knobs ------------------------------------------------------------
    # Same CLI > project.ini > default precedence as identity. Resolved here so the desktop branch
    # (orientation/fullscreen reserved for downstream consumers) and the Android Gradle host
    # (which reads project.ini directly via identityProp) stay in sync on key names. The iOS
    # bundle helper that consumed min_ios/permissions/category is parked on defer/ios.
    _octarine_resolve_identity(_pkg_orientation "${OCTARINE_PACKAGE_ORIENTATION}"  "${_pi_orientation}"  "")
    _octarine_resolve_identity(_pkg_fullscreen  "${OCTARINE_PACKAGE_FULLSCREEN}"   "${_pi_fullscreen}"   "")
    _octarine_resolve_identity(_pkg_permissions "${OCTARINE_PACKAGE_PERMISSIONS}"  "${_pi_permissions}"  "")
    _octarine_resolve_identity(_pkg_category    "${OCTARINE_PACKAGE_CATEGORY}"     "${_pi_category}"     "")

    # ---- Desktop ------------------------------------------------------------------------------
    # macOS: a .app bundle; binary in Contents/MacOS, project files in Contents/Resources.
    # Windows/Linux: flat — binary + project files share the package root so base_path_ finds them.
    if (WIN32)
        set(_octarine_desktop_os "windows")
    elseif (APPLE)
        set(_octarine_desktop_os "macos")
    else ()
        set(_octarine_desktop_os "linux")
    endif ()
    _octarine_generate_desktop_icons("${OP_PROJECT}"
            "${CMAKE_BINARY_DIR}/_octarine_desktop_icons"
            "${_octarine_desktop_os}"
            _desktop_ico _desktop_icns _desktop_png)

    if (APPLE)
        set_target_properties(${TARGET} PROPERTIES MACOSX_BUNDLE ON)
        set(_runtime_dest ".")
        set(_data_dest "${TARGET}.app/Contents/Resources")
        set(_framework_dest "${TARGET}.app/Contents/Frameworks")
        # MACOSX_BUNDLE_ICON_FILE writes CFBundleIconFile into Info.plist; the .icns must land in
        # Contents/Resources/ with the matching name. Both pieces only kick in when the generator
        # produced an .icns (project supplied an icon).
        if (EXISTS "${_desktop_icns}")
            get_filename_component(_icns_name "${_desktop_icns}" NAME)
            set_target_properties(${TARGET} PROPERTIES MACOSX_BUNDLE_ICON_FILE "${_icns_name}")
            install(FILES "${_desktop_icns}" DESTINATION "${_data_dest}")
        endif ()
    else ()
        set(_runtime_dest ".")
        set(_data_dest ".")
        set(_framework_dest ".")   # unused on non-APPLE; bundle helper branches on platform
        # Linux: ship the PNG beside the binary so a future .desktop file (or AppImage) can
        # reference it via Icon=octarine_icon. Windows: nothing here; CPack NSIS picks the .ico
        # through CPACK_NSIS_MUI_ICON below.
        if (NOT WIN32 AND EXISTS "${_desktop_png}")
            install(FILES "${_desktop_png}" DESTINATION "${_runtime_dest}")
        endif ()
    endif ()

    _octarine_setup_desktop_install(${TARGET} "${OP_PROJECT}" "${_runtime_dest}" "${_data_dest}")
    _octarine_bundle_runtime_libs(${TARGET} "${_runtime_dest}" "${_framework_dest}")
    _octarine_setup_cpack("${_pkg_name}" "${_pkg_version}" "${_pkg_description}" "${_pkg_vendor}" "${_desktop_ico}")
endfunction()
