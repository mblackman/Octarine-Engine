# OctarinePackage.cmake — desktop packaging.
#
# Produces a self-contained, double-clickable artifact per desktop OS that launches with NO `-p`
# argument: the engine binary sits beside a baked game project (assets + lua + config +
# asset_manifest.lua), and `SDL_GetBasePath()` resolves the project dir at the exe/bundle location.
# The baked manifest is what removes the runtime filesystem scan, so the same code works inside a
# read-only bundle.
#
# Usage (from the top-level CMakeLists, behind an opt-in):
#   include(cmake/OctarinePackage.cmake)
#   octarine_package(OctarineEngine PROJECT <game-project-dir> [NAME <pkg-name>])
#
# The engine resolves all of fonts/images/sounds/scripts/etc. by recursively scanning a single base
# dir, so we stage the whole PROJECT directory rather than separate assets/ + lua/ trees. PROJECT
# must contain the project's `config` and startup script; the bake step writes asset_manifest.lua
# into the staged copy.

# Guard against double-include.
if (DEFINED _OCTARINE_PACKAGE_INCLUDED)
    return()
endif ()
set(_OCTARINE_PACKAGE_INCLUDED ON)

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
    # Resolved up front so both the iOS Info.plist branch below and the desktop CPack block at the
    # end of this function can consume it. Precedence: CLI cache override > project.ini > default.
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
    # SDL_GetBasePath(). xcodebuild handles signing/install/.ipa archiving, so this helper only
    # owns build-time wiring: Info.plist identity, asset staging, and the POST_BUILD copy of the
    # baked project into the bundle. CPack is irrelevant here — `.ipa` is xcodebuild's output.
    #
    # Bake is NOT invoked here: a cross-compiled iOS binary can't execute on the build host. The
    # project must ship a baked asset_manifest.lua (the Android contract, see CLAUDE.md). To
    # automate the bake at build time, point OCTARINE_HOST_BAKE_EXE at a host-native build of
    # OctarineEngine and we POST_BUILD-run it before the copy.
    if (CMAKE_SYSTEM_NAME STREQUAL "iOS")
        set_target_properties(${TARGET} PROPERTIES
                MACOSX_BUNDLE                       ON
                MACOSX_BUNDLE_BUNDLE_NAME           "${_pkg_name}"
                MACOSX_BUNDLE_GUI_IDENTIFIER        "${_pkg_id}"
                MACOSX_BUNDLE_SHORT_VERSION_STRING  "${_pkg_version}"
                MACOSX_BUNDLE_BUNDLE_VERSION        "${_pkg_version_code}"
                MACOSX_BUNDLE_INFO_STRING           "${_pkg_description}"
                MACOSX_BUNDLE_COPYRIGHT             "${_pkg_vendor}"
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
        file(COPY "${OP_PROJECT}/"
                DESTINATION "${_ios_staged}"
                PATTERN ".git" EXCLUDE
                PATTERN ".gitignore" EXCLUDE
                PATTERN "*.meta" EXCLUDE
                PATTERN "editor_prefs.ini" EXCLUDE
                PATTERN "preferences.ini" EXCLUDE
                PATTERN "imgui.ini" EXCLUDE
                PATTERN "project.ini" EXCLUDE)

        # Optional host-bake step. Cross-compiled binary can't run on the build host, so the bake
        # needs a host-native OctarineEngine. Matches the Android `-Poctarine.bakeExe` contract.
        if (DEFINED CACHE{OCTARINE_HOST_BAKE_EXE} AND OCTARINE_HOST_BAKE_EXE)
            add_custom_command(TARGET ${TARGET} PRE_BUILD
                    COMMAND "${OCTARINE_HOST_BAKE_EXE}" "${_ios_staged}" -m bake
                    COMMENT "Octarine: baking asset manifest into staged project (iOS)"
                    VERBATIM)
        else ()
            if (NOT EXISTS "${_ios_staged}/asset_manifest.lua")
                message(WARNING "Octarine iOS: no asset_manifest.lua in project and OCTARINE_HOST_BAKE_EXE is unset — "
                                "the shipped bundle will fail to load assets at launch. Either commit a baked manifest "
                                "to ${OP_PROJECT} or set -DOCTARINE_HOST_BAKE_EXE=<path/to/desktop/OctarineEngine>.")
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

        message(STATUS "Octarine: iOS bundle '${_pkg_name}' ${_pkg_version} id=${_pkg_id}")
        return()
    endif ()

    # ---- Deliverable layout (desktop) ---------------------------------------------------------
    # macOS: a .app bundle; binary in Contents/MacOS, project files in Contents/Resources.
    # Windows/Linux: flat — binary + project files share the package root so base_path_ finds them.
    if (APPLE)
        set_target_properties(${TARGET} PROPERTIES MACOSX_BUNDLE ON)
        set(_runtime_dest ".")
        set(_data_dest "${TARGET}.app/Contents/Resources")
    else ()
        set(_runtime_dest ".")
        set(_data_dest ".")
    endif ()

    install(TARGETS ${TARGET}
            RUNTIME DESTINATION "${_runtime_dest}"
            BUNDLE DESTINATION "${_runtime_dest}"
    )

    # Bake the asset manifest with the just-built binary, then stage the project dir (manifest
    # included). install() steps run in order, so the bake CODE precedes the DIRECTORY copy. The
    # bake is the CI gate too: a broken asset reference exits nonzero and aborts the install.
    install(CODE "
        message(STATUS \"Octarine: baking asset manifest for package...\")
        execute_process(
            COMMAND \"$<TARGET_FILE:${TARGET}>\" \"${OP_PROJECT}\" -m bake
            RESULT_VARIABLE _bake_rc)
        if (NOT _bake_rc EQUAL 0)
            message(FATAL_ERROR \"Octarine: asset bake failed (rc=\${_bake_rc}); aborting package\")
        endif ()
    ")

    # Stage the whole project dir beside the binary. Exclude editor/runtime scratch that isn't part
    # of the shipped game (user prefs, imgui layout). asset_manifest.lua is produced by the bake
    # above and lives in OP_PROJECT, so it rides along here.
    install(DIRECTORY "${OP_PROJECT}/"
            DESTINATION "${_data_dest}"
            PATTERN ".git" EXCLUDE
            PATTERN ".gitignore" EXCLUDE
            PATTERN "*.meta" EXCLUDE
            PATTERN "editor_prefs.ini" EXCLUDE
            PATTERN "preferences.ini" EXCLUDE
            PATTERN "imgui.ini" EXCLUDE
    )

    # Bundle vcpkg runtime libs beside the binary. The default vcpkg triplets on Linux/macOS
    # (x64-linux, *-osx) are static — the glob hits nothing and the step is a no-op. A dynamic
    # triplet or SHARED build of SDL3 drops .so/.dylib alongside the build-tree binary; those need
    # to ship with the package. We glob TARGET_FILE_DIR rather than $<TARGET_RUNTIME_DLLS> for the
    # same reason as Windows: vcpkg's app-local copy includes the FULL transitive set (freetype,
    # libpng, jpeg, zlib, ...), and the genex only lists direct imports — missing a transitive dep
    # yields a load-time failure (0xC0000135 on Windows; ENOENT/dyld at launch on Linux/macOS).
    # FOLLOW_SYMLINK_CHAIN copies the libfoo.so → libfoo.so.1 → libfoo.so.1.2.3 versioned chain
    # whole rather than dropping a dangling symlink.
    if (WIN32)
        install(CODE "
            file(GLOB _octarine_runtime_dlls \"$<TARGET_FILE_DIR:${TARGET}>/*.dll\")
            if (_octarine_runtime_dlls)
                file(INSTALL \${_octarine_runtime_dlls} DESTINATION \"\${CMAKE_INSTALL_PREFIX}/${_runtime_dest}\")
            endif ()
        ")
    elseif (APPLE)
        # Bundle into Contents/Frameworks; INSTALL_RPATH on the binary points the loader there.
        # vcpkg dylibs use @rpath/... install_names, so the rpath rewrite alone is enough — no
        # install_name_tool dance needed unless a future dep ships with an absolute install_name
        # (at which point swap this for BundleUtilities::fixup_bundle).
        set(_octarine_fw_dest "${TARGET}.app/Contents/Frameworks")
        set_target_properties(${TARGET} PROPERTIES INSTALL_RPATH "@executable_path/../Frameworks")
        install(CODE "
            file(GLOB _octarine_runtime_libs \"$<TARGET_FILE_DIR:${TARGET}>/*.dylib\")
            if (_octarine_runtime_libs)
                file(INSTALL \${_octarine_runtime_libs}
                     DESTINATION \"\${CMAKE_INSTALL_PREFIX}/${_octarine_fw_dest}\"
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
                     DESTINATION \"\${CMAKE_INSTALL_PREFIX}/${_runtime_dest}\"
                     FOLLOW_SYMLINK_CHAIN)
            endif ()
        ")
    endif ()

    # Bundle the C/C++ runtime so the artifact is self-contained on a clean machine.
    #
    # On MSVC we install the CRT DLLs straight from the COMPILER'S OWN redist folder rather than
    # relying on InstallRequiredSystemLibraries. That module's redist finder can latch onto a
    # different VS install present on the machine (e.g. a leftover VS2019), bundling an older
    # MSVCP140.dll that access-violates (0xC0000005) against a binary built by a newer toolset — the
    # app-dir DLL shadows the matching System32 one at launch and the exe dies before its first log
    # line. Deriving the redist from CMAKE_CXX_COMPILER guarantees the shipped CRT matches the bytes
    # we compiled. Layout: <VC>/Tools/MSVC/<ver>/bin/Host<a>/<a>/cl.exe -> <VC>/Redist/MSVC/<rver>/
    # <arch>/Microsoft.VC<toolset>.CRT/*.dll  (7 levels up from cl.exe reaches the VC dir:
    # x64/Host<a>/bin/<ver>/MSVC/Tools/VC).
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
                install(PROGRAMS ${_crt_dlls} DESTINATION "${_runtime_dest}")
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
            install(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION "${_runtime_dest}")
        endif ()
    endif ()

    # ---- CPack --------------------------------------------------------------------------------
    # include(CPack) below captures these from THIS scope, so set them plainly (no PARENT_SCOPE).
    # Identity was resolved up front (see top of function) so iOS could share it; reuse those vars.
    set(CPACK_PACKAGE_NAME "${_pkg_name}")
    set(CPACK_PACKAGE_VERSION "${_pkg_version}")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${_pkg_description}")
    set(CPACK_PACKAGE_VENDOR "${_pkg_vendor}")
    message(STATUS "Octarine: package identity '${_pkg_name}' ${_pkg_version} (vendor: ${_pkg_vendor})")
    # Keep the default single top-level <name>-<version>/ folder: unzip yields one self-contained dir.

    if (WIN32)
        # ZIP is the portable single-folder drop; NSIS installer is opt-in (see option above).
        set(CPACK_GENERATOR "ZIP")
        if (OCTARINE_PACKAGE_NSIS)
            list(APPEND CPACK_GENERATOR "NSIS")
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
