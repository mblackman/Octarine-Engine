# Thin wrapper so non-CMake drivers (Gradle, shell) can invoke the license aggregator via
# `cmake -P scripts/run-licenses.cmake -DOCTARINE_LICENSES_OUT=<file> -DVCPKG_TARGET_TRIPLET=<t>
#   -DVCPKG_INSTALLED_DIR=<dir> [-DOCTARINE_LICENSES_EXTRA_DIRS=dir1;dir2]`.
#
# `-P` does NOT process top-level set()/include() args before script execution, but it DOES define
# -D cache vars before running the script (CMake quirk: in -P mode the cache is in-memory, accessible
# as plain variables). So this script reads OCTARINE_LICENSES_* / VCPKG_* as ordinary vars.

cmake_minimum_required(VERSION 3.20)

if (NOT DEFINED OCTARINE_LICENSES_OUT OR OCTARINE_LICENSES_OUT STREQUAL "")
    message(FATAL_ERROR "run-licenses.cmake: pass -DOCTARINE_LICENSES_OUT=<output-file>.")
endif ()
if (NOT DEFINED VCPKG_TARGET_TRIPLET OR VCPKG_TARGET_TRIPLET STREQUAL "")
    message(FATAL_ERROR "run-licenses.cmake: pass -DVCPKG_TARGET_TRIPLET=<triplet>.")
endif ()
if (NOT DEFINED VCPKG_INSTALLED_DIR OR VCPKG_INSTALLED_DIR STREQUAL "")
    message(FATAL_ERROR "run-licenses.cmake: pass -DVCPKG_INSTALLED_DIR=<dir>.")
endif ()

include("${CMAKE_CURRENT_LIST_DIR}/../cmake/octarine-licenses.cmake")

set(_extra "")
if (DEFINED OCTARINE_LICENSES_EXTRA_DIRS AND NOT OCTARINE_LICENSES_EXTRA_DIRS STREQUAL "")
    set(_extra ${OCTARINE_LICENSES_EXTRA_DIRS})
endif ()

octarine_collect_licenses("${OCTARINE_LICENSES_OUT}"
        TRIPLET "${VCPKG_TARGET_TRIPLET}"
        INSTALLED_DIR "${VCPKG_INSTALLED_DIR}"
        EXTRA_DIRS ${_extra})
