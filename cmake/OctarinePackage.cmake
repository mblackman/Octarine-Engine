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

    # ---- Deliverable layout -------------------------------------------------------------------
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

    # Bundle the runtime DLLs (dynamic vcpkg triplet). vcpkg app-local-deploys the FULL transitive
    # set beside the built binary (SDL + its private deps: freetype, libpng, jpeg, zlib, ...), so we
    # copy whatever landed there rather than $<TARGET_RUNTIME_DLLS> — that genex only lists directly
    # linked imports and would miss the transitive ones, yielding a 0xC0000135 at launch. A static
    # triplet (x64-windows-static) leaves no DLLs and makes this a no-op. (Linux/macOS .so/.dylib
    # bundling differs — typically system libs or static linking; not wired here yet.)
    if (WIN32)
        install(CODE "
            file(GLOB _octarine_runtime_dlls \"$<TARGET_FILE_DIR:${TARGET}>/*.dll\")
            if (_octarine_runtime_dlls)
                file(INSTALL \${_octarine_runtime_dlls} DESTINATION \"\${CMAKE_INSTALL_PREFIX}/${_runtime_dest}\")
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
    #
    # Project identity sources, in precedence order: CLI cache override > project.ini > built-in
    # default (the latter falls back to engine metadata + OP_NAME so a project with no project.ini
    # still packages — just under the engine's identity, which is fine for the example today).
    octarine_read_project_ini("${OP_PROJECT}" _pi)
    _octarine_resolve_identity(_pkg_name        "${OCTARINE_PACKAGE_NAME}"         "${_pi_name}"         "${OP_NAME}")
    _octarine_resolve_identity(_pkg_version     "${OCTARINE_PACKAGE_VERSION_NAME}" "${_pi_version_name}" "${PROJECT_VERSION}")
    _octarine_resolve_identity(_pkg_vendor      "${OCTARINE_PACKAGE_VENDOR}"       "${_pi_vendor}"       "Octarine")
    _octarine_resolve_identity(_pkg_description "${OCTARINE_PACKAGE_DESCRIPTION}"  "${_pi_description}"  "${PROJECT_DESCRIPTION}")
    # package_id isn't consumed by CPack (desktop has no native bundle id), but we resolve and
    # validate it here so a misconfig is caught before it bites Android with the same project.ini.
    _octarine_resolve_identity(_pkg_id          "${OCTARINE_PACKAGE_ID}"           "${_pi_package_id}"   "")
    _octarine_validate_identity("${OP_PROJECT}" "${_pkg_name}" "${_pkg_id}" "${_pkg_version}" ON)
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
