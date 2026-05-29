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
    # Force the gate ON for the packaged engine target regardless of the cache option, so a package
    # configured with OCTARINE_SHIPPED left OFF can't silently ship a binary that scans and dies at
    # launch.
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
    set(CPACK_PACKAGE_NAME "${OP_NAME}")
    set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
    set(CPACK_PACKAGE_VENDOR "Octarine")
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
