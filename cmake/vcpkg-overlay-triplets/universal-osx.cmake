# vcpkg triplet for a macOS universal binary (arm64 + x86_64 in a single fat Mach-O).
#
# vcpkg's stock x64-osx / arm64-osx triplets compile a single arch each, which is fine when shipping
# per-arch downloads. For a single .dmg that runs on both Apple silicon and Intel, every dep needs
# to be a fat lib too — the engine's universal link step otherwise drops the missing-arch slices.
# Setting VCPKG_OSX_ARCHITECTURES to a list propagates through vcpkg's own toolchain into each
# port's CMAKE_OSX_ARCHITECTURES, and the resulting .a / .dylib ship lipo'd binaries.
#
# Activate via:
#   -DVCPKG_OVERLAY_TRIPLETS=<repo>/cmake/vcpkg-overlay-triplets
#   -DVCPKG_TARGET_TRIPLET=universal-osx
# (the ship-mac-universal CMake preset already wires both).
#
# A handful of vcpkg ports (rare CPU-specific assembly) will fail to produce both slices and need
# port-level patches; if a new dep regresses here, the failure shows as a "missing required
# architecture" linker error at engine link time, NOT at port install time.

set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE static)
set(VCPKG_CMAKE_SYSTEM_NAME Darwin)
set(VCPKG_OSX_ARCHITECTURES "arm64;x86_64")
