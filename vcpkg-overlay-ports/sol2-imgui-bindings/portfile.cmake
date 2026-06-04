# Octarine overlay port: the engine's own sol2 ImGui bindings, consumed as a
# prebuilt (binary-cached) static lib instead of an in-tree submodule + add_subdirectory.
# Pinned to a tagged release (vX.Y.Z-<rev>; see the bindings repo CONTRIBUTING.md).
# To bump: change REF to the new tag and update SHA512 to the matching
# https://github.com/mblackman/sol2_ImGui_Bindings/archive/<tag>.tar.gz hash
# (run `vcpkg install` once with a wrong hash and paste the "Actual hash" it prints).
vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO mblackman/sol2_ImGui_Bindings
    REF v1.92.7-1   # X.Y.Z = targeted Dear ImGui version (== this port's version); -<rev> = binding revision
    SHA512 12bda9c9c05d9eca1eab329f37835cc2ef73b2b867e1e140238f3b9c4c6fe08548c6febcb8efa34e159dc428d77d4e0721e1c14fff87bc4a04285b805d0cd1d1
    HEAD_REF main
)

# The bindings' CMake ships install() + EXPORT + a find_package config (gated by
# SOL2_IMGUI_BINDINGS_INSTALL, on by default when standalone). Tests stay off.
vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DSOL2_IMGUI_BINDINGS_INSTALL=ON
        -DSOL2_IMGUI_BINDINGS_BUILD_TESTS=OFF
)
vcpkg_cmake_install()

# Bindings install their config to lib/cmake/sol2_ImGui_Bindings; relocate to share/.
vcpkg_cmake_config_fixup(PACKAGE_NAME sol2_ImGui_Bindings CONFIG_PATH lib/cmake/sol2_ImGui_Bindings)

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
