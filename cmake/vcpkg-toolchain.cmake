# Wrapper that chains to vcpkg's toolchain file.
# Checks env vars, CLion's bundled vcpkg, and other common locations.
foreach(_candidate
        "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        "$ENV{VCPKG_INSTALLATION_ROOT}/scripts/buildsystems/vcpkg.cmake"
        "$ENV{HOME}/.vcpkg-clion/vcpkg/scripts/buildsystems/vcpkg.cmake"
        "$ENV{USERPROFILE}/.vcpkg-clion/vcpkg/scripts/buildsystems/vcpkg.cmake"
        "$ENV{HOME}/vcpkg/scripts/buildsystems/vcpkg.cmake")
    if (EXISTS "${_candidate}")
        include("${_candidate}")
        return()
    endif ()
endforeach()

# CLion on Linux sets VCPKG_ROOT to the vcpkg binary — check its parent directory.
if (DEFINED ENV{VCPKG_ROOT})
    get_filename_component(_parent "$ENV{VCPKG_ROOT}" DIRECTORY)
    if (EXISTS "${_parent}/scripts/buildsystems/vcpkg.cmake")
        include("${_parent}/scripts/buildsystems/vcpkg.cmake")
    endif ()
    unset(_parent)
endif ()
