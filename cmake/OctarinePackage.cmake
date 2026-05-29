# OctarinePackage.cmake — desktop + iOS packaging.
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
#   _octarine_setup_ios_bundle        — iOS .app: Info.plist + icons + project staging + POST_BUILD copy
#   _octarine_generate_desktop_icons  — runs scripts/octarine-icons.cmake for the host OS
#   _octarine_setup_desktop_install   — install(TARGETS) + bake CODE step + project DIRECTORY copy
#   _octarine_bundle_runtime_libs     — vcpkg dylibs/sos/DLLs + MSVC CRT redist beside the binary
#   _octarine_setup_cpack             — CPACK_* vars + generator pick + include(CPack)

# Guard against double-include.
if (DEFINED _OCTARINE_PACKAGE_INCLUDED)
    return()
endif ()
set(_OCTARINE_PACKAGE_INCLUDED ON)

# Path to the shared icon/splash generator. Same script Gradle runs on Android — single source of
# truth for sizes/manifests so iOS/desktop/Android can't drift. CMAKE_CURRENT_LIST_DIR resolves to
# the directory of THIS file (cmake/) regardless of which CMakeLists.txt includes it.
set(_OCTARINE_ICON_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/../scripts/octarine-icons.cmake")

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
set(OCTARINE_PACKAGE_ID           "" CACHE STRING "Override project.ini: package_id (Android/iOS bundle id)")

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
        list(APPEND _errors "missing required key: package_id (Android/iOS bundle id)")
    elseif (NOT "${PKG_ID}" MATCHES "^[a-z][a-z0-9_]*(\\.[a-z][a-z0-9_]*)+$")
        list(APPEND _errors "package_id '${PKG_ID}' must be reverse-DNS (e.g. com.studio.mygame)")
    endif ()
    if (_errors)
        string(REPLACE ";" "\n  - " _err_lines "${_errors}")
        message(FATAL_ERROR "Octarine: project.ini validation failed (${_ini}):\n  - ${_err_lines}")
    endif ()
endfunction()

# iOS .app: Info.plist identity, optional codesign passthrough, staged project tree, icon/splash
# wiring, optional host-bake step, POST_BUILD copy into the bundle root. xcodebuild handles signing
# + .ipa archiving — this helper only owns build-time bundle setup. CPack is irrelevant here.
function(_octarine_setup_ios_bundle TARGET PROJECT_DIR PKG_NAME PKG_ID PKG_VERSION PKG_VERSION_CODE PKG_DESCRIPTION PKG_VENDOR)
    set_target_properties(${TARGET} PROPERTIES
            MACOSX_BUNDLE                       ON
            MACOSX_BUNDLE_BUNDLE_NAME           "${PKG_NAME}"
            MACOSX_BUNDLE_GUI_IDENTIFIER        "${PKG_ID}"
            MACOSX_BUNDLE_SHORT_VERSION_STRING  "${PKG_VERSION}"
            MACOSX_BUNDLE_BUNDLE_VERSION        "${PKG_VERSION_CODE}"
            MACOSX_BUNDLE_INFO_STRING           "${PKG_DESCRIPTION}"
            MACOSX_BUNDLE_COPYRIGHT             "${PKG_VENDOR}"
            # CMake-generated Xcode targets default to SKIP_INSTALL=YES, which makes
            # `xcodebuild archive` succeed but leave the .xcarchive's Products/Applications/
            # directory empty (the .app lands in UninstalledProducts/ instead). Both the
            # signed `xcodebuild -exportArchive` path and the unsigned-archive Payload zip
            # in scripts/build-ios-ipa.sh need the .app under Products/Applications/, so
            # flip SKIP_INSTALL off and point INSTALL_PATH at /Applications (the iOS app
            # install root, mirroring Xcode's default iOS-app target).
            XCODE_ATTRIBUTE_SKIP_INSTALL        "NO"
            XCODE_ATTRIBUTE_INSTALL_PATH        "/Applications"
    )

    # Optional codesign passthrough. If unset Xcode falls back to its own signing settings
    # (automatic team if configured, or unsigned for the simulator). The dev team / identity
    # are deployment concerns, not source-tree concerns — keep them out of the preset.
    if (DEFINED CACHE{OCTARINE_IOS_DEVELOPMENT_TEAM} AND OCTARINE_IOS_DEVELOPMENT_TEAM)
        set_target_properties(${TARGET} PROPERTIES
                XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "${OCTARINE_IOS_DEVELOPMENT_TEAM}")
    endif ()
    if (DEFINED CACHE{OCTARINE_IOS_CODE_SIGN_IDENTITY} AND OCTARINE_IOS_CODE_SIGN_IDENTITY)
        set_target_properties(${TARGET} PROPERTIES
                XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "${OCTARINE_IOS_CODE_SIGN_IDENTITY}")
    endif ()

    # Stage the project to a clean dir at configure time so the POST_BUILD copy is a single
    # cmake -E copy_directory (which has no exclude flag). file(COPY ... PATTERN EXCLUDE)
    # handles the scratch filtering; same exclusion list as the desktop install(DIRECTORY).
    set(_ios_staged "${CMAKE_BINARY_DIR}/_octarine_ios_staged_project")
    file(REMOVE_RECURSE "${_ios_staged}")
    file(COPY "${PROJECT_DIR}/"
            DESTINATION "${_ios_staged}"
            PATTERN ".git" EXCLUDE
            PATTERN ".gitignore" EXCLUDE
            PATTERN "*.meta" EXCLUDE
            PATTERN "editor_prefs.ini" EXCLUDE
            PATTERN "preferences.ini" EXCLUDE
            PATTERN "imgui.ini" EXCLUDE
            PATTERN "project.ini" EXCLUDE)

    # Generate Assets.xcassets/AppIcon.appiconset + LaunchScreen.storyboard from the project's
    # `icon=` / `splash_color=`. Configure-time so xcodebuild sees the files when it builds the
    # target; rerunning configure rebuilds them. Skipped silently if the project has no icon=
    # or ImageMagick is absent — the bundle then ships with no icon (Xcode warns) and the
    # default black launch screen, which is fine for early bootstrap projects.
    set(_ios_icons_root "${CMAKE_BINARY_DIR}/_octarine_ios_icons")
    execute_process(
            COMMAND "${CMAKE_COMMAND}"
                    "-DOCTARINE_ICON_PROJECT=${PROJECT_DIR}"
                    "-DOCTARINE_ICON_PLATFORM=ios"
                    "-DOCTARINE_ICON_OUT_DIR=${_ios_icons_root}"
                    -P "${_OCTARINE_ICON_SCRIPT}"
            RESULT_VARIABLE _icon_rc)
    if (NOT _icon_rc EQUAL 0)
        message(FATAL_ERROR "octarine_package: iOS icon generation failed (rc=${_icon_rc})")
    endif ()
    set(_xcassets "${_ios_icons_root}/ios/Assets.xcassets")
    set(_launchscreen "${_ios_icons_root}/ios/LaunchScreen.storyboard")
    if (EXISTS "${_xcassets}" AND EXISTS "${_launchscreen}")
        # xcassets gets compiled by actool, storyboard by ibtool — both kick in automatically
        # when these land as target sources with MACOSX_PACKAGE_LOCATION=Resources. INFOPLIST_KEY_*
        # flows through Xcode into the generated Info.plist (requires CMake 3.25+ for the
        # property mode; we're on 3.20+ for the script and a newer build is required to
        # generate Xcode projects, so this is safe).
        target_sources(${TARGET} PRIVATE "${_xcassets}" "${_launchscreen}")
        set_source_files_properties("${_xcassets}" "${_launchscreen}"
                PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
        set_target_properties(${TARGET} PROPERTIES
                XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon"
                XCODE_ATTRIBUTE_INFOPLIST_KEY_CFBundleIconName "AppIcon"
                XCODE_ATTRIBUTE_INFOPLIST_KEY_UILaunchStoryboardName "LaunchScreen")
        message(STATUS "Octarine: iOS AppIcon + LaunchScreen wired (${_xcassets})")
    else ()
        message(STATUS "Octarine: iOS shipping with default icon/launchscreen (no `icon=` in project.ini "
                        "or ImageMagick missing).")
    endif ()

    # Optional host-bake step. Cross-compiled binary can't run on the build host, so the bake
    # needs a host-native OctarineEngine. Matches the Android `-Poctarine.bakeExe` contract.
    if (DEFINED CACHE{OCTARINE_HOST_BAKE_EXE} AND OCTARINE_HOST_BAKE_EXE)
        add_custom_command(TARGET ${TARGET} PRE_BUILD
                COMMAND "${OCTARINE_HOST_BAKE_EXE}" "${_ios_staged}" -m bake
                COMMENT "Octarine: baking asset manifest into staged project (iOS)"
                VERBATIM)
    else ()
        if (NOT EXISTS "${_ios_staged}/asset_manifest.lua")
            # Shipping iOS without a manifest produces a .app that dies at first scene load —
            # OCTARINE_SHIPPED is forced ON above, so the catalog hits its manifest-load branch
            # against a missing file. Fail at configure rather than letting the bad bundle
            # reach install/TestFlight.
            message(FATAL_ERROR "Octarine iOS: no asset_manifest.lua in project and OCTARINE_HOST_BAKE_EXE is unset. "
                            "Either commit a baked manifest to ${PROJECT_DIR} or set "
                            "-DOCTARINE_HOST_BAKE_EXE=<path/to/desktop/OctarineEngine>.")
        endif ()
    endif ()

    # Copy staged project into the .app at the bundle root (where SDL_GetBasePath() points on
    # iOS). $<TARGET_BUNDLE_DIR> resolves to .../<config>/<TARGET>.app for the Xcode generator.
    add_custom_command(TARGET ${TARGET} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_directory
                    "${_ios_staged}"
                    "$<TARGET_BUNDLE_DIR:${TARGET}>"
            COMMENT "Octarine: staging project files into ${TARGET}.app"
            VERBATIM)

    message(STATUS "Octarine: iOS bundle '${PKG_NAME}' ${PKG_VERSION} id=${PKG_ID}")
endfunction()

# Generate per-OS icon files (.ico/.icns/.png) under ${OUT_DIR}/desktop. Sets ${ICO_VAR},
# ${ICNS_VAR}, ${PNG_VAR} in the caller's scope to the conventional output paths (callers test
# EXISTS before consuming, since the script skip-warns when project.ini has no `icon=` or
# ImageMagick is absent).
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
    # Resolved up front so both the iOS bundle helper and the desktop CPack helper consume the
    # same values. Precedence: CLI cache override > project.ini > default.
    octarine_read_project_ini("${OP_PROJECT}" _pi)
    _octarine_resolve_identity(_pkg_name        "${OCTARINE_PACKAGE_NAME}"         "${_pi_name}"         "${OP_NAME}")
    _octarine_resolve_identity(_pkg_version     "${OCTARINE_PACKAGE_VERSION_NAME}" "${_pi_version_name}" "${PROJECT_VERSION}")
    _octarine_resolve_identity(_pkg_version_code "${OCTARINE_PACKAGE_VERSION_CODE}" "${_pi_version_code}" "1")
    _octarine_resolve_identity(_pkg_vendor      "${OCTARINE_PACKAGE_VENDOR}"       "${_pi_vendor}"       "Octarine")
    _octarine_resolve_identity(_pkg_description "${OCTARINE_PACKAGE_DESCRIPTION}"  "${_pi_description}"  "${PROJECT_DESCRIPTION}")
    _octarine_resolve_identity(_pkg_id          "${OCTARINE_PACKAGE_ID}"           "${_pi_package_id}"   "")
    _octarine_validate_identity("${OP_PROJECT}" "${_pkg_name}" "${_pkg_id}" "${_pkg_version}" ON)

    # ---- iOS bundle ---------------------------------------------------------------------------
    # iOS .app is a flat bundle: binary + Resources sit at the bundle root, reached by
    # SDL_GetBasePath(). Bake is NOT invoked here: a cross-compiled iOS binary can't execute on
    # the build host. The project must ship a baked asset_manifest.lua (the Android contract,
    # see CLAUDE.md), or point OCTARINE_HOST_BAKE_EXE at a host-native build of OctarineEngine.
    if (CMAKE_SYSTEM_NAME STREQUAL "iOS")
        _octarine_setup_ios_bundle(${TARGET} "${OP_PROJECT}"
                "${_pkg_name}" "${_pkg_id}" "${_pkg_version}" "${_pkg_version_code}"
                "${_pkg_description}" "${_pkg_vendor}")
        return()
    endif ()

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
