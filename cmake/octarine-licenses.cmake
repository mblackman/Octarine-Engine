# octarine-licenses.cmake — third-party license aggregator.
#
# Walks `${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/share/*/copyright` and concatenates every
# port's license text into one UTF-8 file, prepended by the engine's own LICENSE and any vendored
# libs under `libs/*/LICENSE*`. The function takes an optional EXTRA_DIRS list — each given dir is
# globbed for `*.txt`/`LICENSE*` so a downstream title can drop its own attributions (fonts, audio
# packs, etc.) without touching engine code.
#
# Pure CMake; runs the same way under desktop CPack, the iOS bundle helper, and the Android
# `cmake -P` thin-wrapper. No Python, no external tools.
#
# Usage:
#   include(cmake/octarine-licenses.cmake)
#   octarine_collect_licenses("${CMAKE_BINARY_DIR}/THIRD_PARTY_LICENSES.txt"
#       [TRIPLET <triplet>]            # default: ${VCPKG_TARGET_TRIPLET}
#       [INSTALLED_DIR <dir>]          # default: ${VCPKG_INSTALLED_DIR} or ${CMAKE_BINARY_DIR}/vcpkg_installed
#       [ENGINE_ROOT <dir>]            # default: directory two up from this script (repo root)
#       [REQUIRED_PORTS port1;port2]   # ports that MUST appear in the glob (FATAL_ERROR otherwise)
#       [EXTRA_DIRS dir1;dir2]         # project-side drop-in dirs, each section keyed by filename stem
#       [INCLUDE_EDITOR_FONTS])        # emit src/Editor/Fonts/LICENSE* (Apache-2.0 Roboto, editor builds)

if (DEFINED _OCTARINE_LICENSES_INCLUDED)
    return()
endif ()
set(_OCTARINE_LICENSES_INCLUDED ON)

set(_OCTARINE_LICENSES_SCRIPT_DIR "${CMAKE_CURRENT_LIST_DIR}")

# Default required-port list. Matches the engine's vcpkg.json dep tree (transitively pulls
# libpng + libjpeg-turbo + zlib via sdl3-image and freetype). harfbuzz/libogg/libvorbis are
# optional sdl3-ttf / sdl3-mixer features the engine doesn't currently use — not required.
set(_OCTARINE_LICENSES_DEFAULT_REQUIRED
        freetype
        glm
        imgui
        libjpeg-turbo
        libpng
        lua
        sdl3
        sdl3-image
        sdl3-mixer
        sdl3-ttf
        sol2
        spdlog
        zlib)

# Section heading written as a horizontal-rule sandwich, ASCII-only so any text viewer renders it.
function(_octarine_licenses_append_section OUT_FILE TITLE BODY_FILE)
    file(APPEND "${OUT_FILE}" "\n================================================================================\n")
    file(APPEND "${OUT_FILE}" "${TITLE}\n")
    file(APPEND "${OUT_FILE}" "================================================================================\n\n")
    if (EXISTS "${BODY_FILE}")
        file(READ "${BODY_FILE}" _body)
        file(APPEND "${OUT_FILE}" "${_body}")
        # Ensure trailing newline so the next section's rule starts on a fresh line.
        if (NOT _body MATCHES "\n$")
            file(APPEND "${OUT_FILE}" "\n")
        endif ()
    else ()
        file(APPEND "${OUT_FILE}" "(license text missing on disk: ${BODY_FILE})\n")
    endif ()
endfunction()

function(octarine_collect_licenses OUT_FILE)
    set(options INCLUDE_EDITOR_FONTS)
    set(oneValueArgs TRIPLET INSTALLED_DIR ENGINE_ROOT)
    set(multiValueArgs REQUIRED_PORTS EXTRA_DIRS)
    cmake_parse_arguments(OCL "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT OCL_TRIPLET)
        set(OCL_TRIPLET "${VCPKG_TARGET_TRIPLET}")
    endif ()
    if (NOT OCL_INSTALLED_DIR)
        if (DEFINED VCPKG_INSTALLED_DIR AND VCPKG_INSTALLED_DIR)
            set(OCL_INSTALLED_DIR "${VCPKG_INSTALLED_DIR}")
        else ()
            set(OCL_INSTALLED_DIR "${CMAKE_BINARY_DIR}/vcpkg_installed")
        endif ()
    endif ()
    if (NOT OCL_ENGINE_ROOT)
        get_filename_component(OCL_ENGINE_ROOT "${_OCTARINE_LICENSES_SCRIPT_DIR}/.." ABSOLUTE)
    endif ()
    if (NOT OCL_REQUIRED_PORTS)
        set(OCL_REQUIRED_PORTS ${_OCTARINE_LICENSES_DEFAULT_REQUIRED})
    endif ()

    if (NOT OCL_TRIPLET)
        message(FATAL_ERROR "octarine_collect_licenses: TRIPLET unset and VCPKG_TARGET_TRIPLET undefined.")
    endif ()

    set(_share_root "${OCL_INSTALLED_DIR}/${OCL_TRIPLET}/share")
    if (NOT IS_DIRECTORY "${_share_root}")
        message(FATAL_ERROR "octarine_collect_licenses: vcpkg share dir missing: ${_share_root} "
                "(triplet '${OCL_TRIPLET}', installed dir '${OCL_INSTALLED_DIR}').")
    endif ()

    # Fresh file every run so a stale aggregation can't survive a dep removal.
    get_filename_component(_out_dir "${OUT_FILE}" DIRECTORY)
    if (_out_dir)
        file(MAKE_DIRECTORY "${_out_dir}")
    endif ()
    string(TIMESTAMP _ts "%Y-%m-%dT%H:%M:%SZ" UTC)
    file(WRITE "${OUT_FILE}"
            "Octarine Engine — Third Party Licenses\n"
            "Generated at build time (${_ts}) for triplet ${OCL_TRIPLET}. Do not edit.\n")

    # 1. Engine LICENSE.
    set(_engine_license "${OCL_ENGINE_ROOT}/LICENSE")
    if (EXISTS "${_engine_license}")
        _octarine_licenses_append_section("${OUT_FILE}" "Octarine Engine" "${_engine_license}")
    else ()
        message(WARNING "octarine_collect_licenses: no repo-root LICENSE at ${_engine_license}")
    endif ()

    # 2. vcpkg ports — glob copyright files, dedupe by lowercased port name. Some vcpkg ports
    # publish multiple share/<cap_dir>/ for cmake configs; only the actual port dir carries a
    # `copyright` file, so the glob naturally filters those out.
    file(GLOB _copyright_paths "${_share_root}/*/copyright")
    set(_port_entries "")    # "<lowername>|<title>|<path>" tuples for sort.
    foreach (_copy IN LISTS _copyright_paths)
        get_filename_component(_port_dir "${_copy}" DIRECTORY)
        get_filename_component(_port_name "${_port_dir}" NAME)
        string(TOLOWER "${_port_name}" _port_lc)
        # Skip ports that aren't linked into shipped binaries:
        #   - vcpkg-cmake* — vcpkg's own build helpers.
        #   - benchmark/googlebenchmark — only consumed by the optional OctarineLuaApiTest target.
        # Keeping them out avoids legal noise about deps the user never receives.
        if (_port_lc MATCHES "^vcpkg-" OR _port_lc STREQUAL "benchmark" OR _port_lc STREQUAL "googlebenchmark")
            continue()
        endif ()
        list(APPEND _port_entries "${_port_lc}|${_port_name}|${_copy}")
    endforeach ()
    list(SORT _port_entries)
    list(REMOVE_DUPLICATES _port_entries)

    set(_found_ports "")
    foreach (_entry IN LISTS _port_entries)
        string(REPLACE "|" ";" _parts "${_entry}")
        list(GET _parts 0 _port_lc)
        list(GET _parts 1 _port_name)
        list(GET _parts 2 _path)
        list(APPEND _found_ports "${_port_lc}")
        _octarine_licenses_append_section("${OUT_FILE}" "${_port_name}" "${_path}")
    endforeach ()

    # Required-port whitelist — catches a misconfigured triplet that silently drops a dep.
    set(_missing "")
    foreach (_req IN LISTS OCL_REQUIRED_PORTS)
        string(TOLOWER "${_req}" _req_lc)
        if (NOT _req_lc IN_LIST _found_ports)
            list(APPEND _missing "${_req}")
        endif ()
    endforeach ()
    if (_missing)
        string(REPLACE ";" ", " _missing_str "${_missing}")
        message(FATAL_ERROR "octarine_collect_licenses: required port(s) missing from ${_share_root}: "
                "${_missing_str}. Check the triplet's installed dep tree.")
    endif ()

    # 3. Vendored libs under libs/*/LICENSE*. Section title = directory name.
    file(GLOB _vendored_dirs RELATIVE "${OCL_ENGINE_ROOT}/libs" "${OCL_ENGINE_ROOT}/libs/*")
    list(SORT _vendored_dirs)
    foreach (_vd IN LISTS _vendored_dirs)
        set(_full "${OCL_ENGINE_ROOT}/libs/${_vd}")
        if (NOT IS_DIRECTORY "${_full}")
            continue()
        endif ()
        file(GLOB _vlic "${_full}/LICENSE" "${_full}/LICENSE.txt" "${_full}/LICENSE.md")
        foreach (_lp IN LISTS _vlic)
            _octarine_licenses_append_section("${OUT_FILE}" "${_vd} (vendored)" "${_lp}")
        endforeach ()
    endforeach ()

    # 3b. Editor-only embedded assets. Roboto-Medium.ttf is compiled into Roboto_Medium.h and
    # ships only when OCTARINE_WITH_EDITOR=ON (Android/iOS/ship-release force it off). Caller
    # passes INCLUDE_EDITOR_FONTS in that case so the Apache-2.0 attribution rides along; ship
    # paths leave it off so we don't claim an attribution for a binary that doesn't contain it.
    if (OCL_INCLUDE_EDITOR_FONTS)
        file(GLOB _editor_font_licenses
                "${OCL_ENGINE_ROOT}/src/Editor/Fonts/LICENSE"
                "${OCL_ENGINE_ROOT}/src/Editor/Fonts/LICENSE.txt"
                "${OCL_ENGINE_ROOT}/src/Editor/Fonts/LICENSE.md")
        list(SORT _editor_font_licenses)
        foreach (_efl IN LISTS _editor_font_licenses)
            _octarine_licenses_append_section("${OUT_FILE}" "Editor Fonts (embedded)" "${_efl}")
        endforeach ()
    endif ()

    # 4. Downstream extras. Each EXTRA_DIRS entry is globbed for *.txt and LICENSE*; section
    # title = filename stem so the project author controls the heading by naming the file.
    foreach (_xdir IN LISTS OCL_EXTRA_DIRS)
        if (NOT IS_DIRECTORY "${_xdir}")
            continue()
        endif ()
        file(GLOB _xfiles "${_xdir}/*.txt" "${_xdir}/LICENSE*")
        list(SORT _xfiles)
        foreach (_xf IN LISTS _xfiles)
            get_filename_component(_xf_name "${_xf}" NAME)
            # Skip the convention README that explains the drop-in format.
            if (_xf_name STREQUAL "README.txt")
                continue()
            endif ()
            get_filename_component(_xf_stem "${_xf}" NAME_WE)
            _octarine_licenses_append_section("${OUT_FILE}" "${_xf_stem} (project)" "${_xf}")
        endforeach ()
    endforeach ()

    # 5. Footer — build metadata + sol2 overlay note (legal transparency, not requirement).
    file(APPEND "${OUT_FILE}"
            "\n================================================================================\n"
            "Footer\n"
            "================================================================================\n\n"
            "Triplet: ${OCL_TRIPLET}\n"
            "Installed: ${OCL_INSTALLED_DIR}\n"
            "Engine root: ${OCL_ENGINE_ROOT}\n"
            "Ports aggregated: ${_found_ports}\n"
            "\n"
            "sol2 on Android (NDK r28) ships from android/vcpkg-overlay-ports/sol2 with noexcept(...)\n"
            "stripped at 10 call sites to work around LLVM #91362. License is unchanged upstream MIT.\n")

    message(STATUS "Octarine: wrote third-party license aggregate to ${OUT_FILE}")
endfunction()
