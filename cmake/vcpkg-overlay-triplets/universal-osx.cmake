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

# Force libpng's PNG_ARM_NEON off: when the triplet builds the universal slice, the x86_64
# half of the fat compile still picks up PNG_ARM_NEON_OPT=2 from libpng's CMakeLists ARM
# detection (which looks at the *first* slice listed, arm64) and fails on x86_64 with
# "NEON intrinsics not available with the soft-float ABI" because arm_neon.h doesn't apply.
# Forcing PNG_ARM_NEON=off skips the NEON code path entirely on both slices — minor decode
# perf cost on Apple-silicon Macs, but the universal binary then actually builds.
set(VCPKG_CMAKE_CONFIGURE_OPTIONS "-DPNG_ARM_NEON=off")
