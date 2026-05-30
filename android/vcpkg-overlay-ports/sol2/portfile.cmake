set(VCPKG_BUILD_TYPE release) # header-only

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO ThePhD/sol2
    REF "v${VERSION}"
    SHA512 5a6ec7e16dae05ad6abea02842f62db8f64935eda438d67b2c264cbee80cee6d82200bd060387c6df837fe9f212dbe22b2772af34df1ce8bd43296dd9429558d
    HEAD_REF develop
    PATCHES
        header-only.patch
        lua-5.5.diff # variation of https://github.com/ThePhD/sol2/pull/1723
        pkgconfig.diff
)

# Octarine overlay: work around LLVM #91362 (sol2 #1678 / #1581). The Android NDK r28 clang (19.0.1)
# fails to take the address of sol2's `call(lua_State*) noexcept(std::is_nothrow_copy_assignable_v<T>)`
# static member templates when binding usertype member variables, so the whole scripting surface
# won't compile. The noexcept-specifier there is only an optimization hint, so strip it. String
# replace (not a line-numbered patch) so it survives minor sol2 version drift.
foreach(_sol_hdr ebco.hpp function_types_stateful.hpp function_types_stateless.hpp)
    set(_sol_p "${SOURCE_PATH}/include/sol/${_sol_hdr}")
    file(READ "${_sol_p}" _sol_c)
    string(REPLACE " noexcept(std::is_nothrow_copy_assignable_v<T>)" "" _sol_c "${_sol_c}")
    file(WRITE "${_sol_p}" "${_sol_c}")
endforeach()

vcpkg_cmake_configure(SOURCE_PATH "${SOURCE_PATH}")
vcpkg_cmake_install()
vcpkg_cmake_config_fixup(CONFIG_PATH share/cmake/sol2)
vcpkg_fixup_pkgconfig()

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE.txt")
