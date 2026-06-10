cmake_minimum_required(VERSION 3.15)

# cmake/vcpkg-toolchain.cmake (set via CMakePresets.json) handles toolchain discovery.
# This block is a fallback for non-preset invocations only.

# Project name and language
project(OctarineEngine
        VERSION 0.1.0
        DESCRIPTION "A lightweight 2D game engine of a yellowish-purple color"
        LANGUAGES CXX
)


# Global settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler cache: dramatically speeds the edit -> rebuild -> verify loop agents run constantly.
# Honors an already-set CMAKE_CXX_COMPILER_LAUNCHER. Prefers ccache; falls back to sccache (better
# MSVC support). Opt out with -DOCTARINE_USE_CCACHE=OFF.
option(OCTARINE_USE_CCACHE "Use ccache/sccache as the compiler launcher when available" ON)
if (OCTARINE_USE_CCACHE AND NOT CMAKE_CXX_COMPILER_LAUNCHER)
    find_program(OCTARINE_COMPILER_CACHE NAMES ccache sccache)
    if (OCTARINE_COMPILER_CACHE)
        set(CMAKE_CXX_COMPILER_LAUNCHER "${OCTARINE_COMPILER_CACHE}")
        set(CMAKE_C_COMPILER_LAUNCHER "${OCTARINE_COMPILER_CACHE}")
        message(STATUS "Octarine: compiler cache ENABLED (${OCTARINE_COMPILER_CACHE})")
    endif ()
endif ()

# Architecture tuning for optimized (non-Debug) GCC/Clang builds. -march=native pins codegen to the
# BUILD machine's exact CPU, which is unsafe two ways: (1) a ccache object built on one CI runner and
# reused on another with fewer instructions crashes at runtime via SIGILL, and (2) shipping artifacts
# won't run on older end-user CPUs than the build box. Default to a portable baseline (x86-64-v3 ~ the
# Windows /arch:AVX2 floor: AVX2 + BMI + FMA, Haswell-era 2013+). Opt into -march=native locally with
# -DOCTARINE_NATIVE_ARCH=ON where build and run are the same machine. MSVC uses /arch:AVX2 below and
# ignores this; Android gets its arch from the NDK toolchain (this var is empty there).
option(OCTARINE_NATIVE_ARCH "Tune optimized builds for the local CPU (-march=native); not portable, keep off for CI/shipping" OFF)
if (OCTARINE_NATIVE_ARCH)
    set(OCTARINE_ARCH_FLAG "-march=native")
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64|amd64)$")
    set(OCTARINE_ARCH_FLAG "-march=x86-64-v3")
else ()
    set(OCTARINE_ARCH_FLAG "")  # non-x86 (e.g. Apple Silicon): leave the compiler's portable default
endif ()

# LTO for non-Debug builds. Combines with --gc-sections to drop unreferenced
# code/data from statically linked third-party libs (SDL3, Lua, etc).
include(CheckIPOSupported)
check_ipo_supported(RESULT OCTARINE_IPO_SUPPORTED OUTPUT OCTARINE_IPO_ERROR)
if (OCTARINE_IPO_SUPPORTED)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO ON)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_MINSIZEREL ON)
endif ()

# Sanitizer instrumentation (GCC/Clang only — MSVC's ASan needs different flags and has no UBSan).
# `asan` layers AddressSanitizer + UBSan (they combine cleanly and share one CI job); `tsan` is a
# separate value because ThreadSanitizer cannot be combined with ASan. Flags must reach both
# compile and link lines, and apply to every target (engine, tests) so the interceptors see all
# allocations.
set(OCTARINE_SANITIZE "OFF" CACHE STRING "Sanitizer instrumentation: OFF, asan (ASan+UBSan), or tsan")
set_property(CACHE OCTARINE_SANITIZE PROPERTY STRINGS OFF asan tsan)
if (NOT OCTARINE_SANITIZE STREQUAL "OFF")
    if (NOT (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang"))
        message(FATAL_ERROR "OCTARINE_SANITIZE requires GCC or Clang (got ${CMAKE_CXX_COMPILER_ID})")
    endif ()
    if (OCTARINE_SANITIZE STREQUAL "asan")
        set(OCTARINE_SANITIZE_FLAGS -fsanitize=address,undefined -fno-omit-frame-pointer)
    elseif (OCTARINE_SANITIZE STREQUAL "tsan")
        set(OCTARINE_SANITIZE_FLAGS -fsanitize=thread -fno-omit-frame-pointer)
    else ()
        message(FATAL_ERROR "OCTARINE_SANITIZE must be OFF, asan, or tsan (got ${OCTARINE_SANITIZE})")
    endif ()
    add_compile_options(${OCTARINE_SANITIZE_FLAGS})
    add_link_options(${OCTARINE_SANITIZE_FLAGS})
    message(STATUS "Octarine: sanitizers ENABLED (${OCTARINE_SANITIZE})")
endif ()

# Profiling option — when ON, enables TIMER/ACCUM logging in PerfUtils.h.
# OFF by default so Debug and Release builds have zero profiling overhead.
option(OCTARINE_ENABLE_PROFILING "Enable per-system performance instrumentation" OFF)
option(OCTARINE_WITH_IMGUI "Build with ImGui debug tools" ON)
option(OCTARINE_WITH_EDITOR "Build with the yellowish-purple editor environment" ON)

# Dev-vs-shipped asset-catalog gate. OFF (dev): the catalog always live-scans the asset dir, so a
# stray asset_manifest.lua left beside source assets can't freeze the scan on a stale catalog. ON
# (every packaged target — desktop package, Android): the catalog loads the baked manifest and
# skips the filesystem walk, which is what lets it work inside a read-only APK bundle. The
# packaging path (octarine_package / CPack) forces this ON; a packaged build that leaves it OFF would
# try to scan a read-only FS and fail at launch.
option(OCTARINE_SHIPPED "Build a shipped artifact: load the baked asset manifest instead of scanning" OFF)

# Desktop packaging. Set to a game-project dir to wire install rules + CPack that emit a
# self-contained artifact (engine + that project's assets/lua/config + baked manifest), launchable
# with no `-p`. Empty (default) leaves normal dev/editor builds completely untouched. The packaging
# path forces OCTARINE_SHIPPED ON on the engine target regardless of the option above.
set(OCTARINE_PACKAGE_PROJECT "" CACHE PATH "Game project dir to bundle into a packaged artifact (empty = no packaging)")

# Android target. The NDK toolchain sets ANDROID/CMAKE_SYSTEM_NAME=Android. The engine cross-compiles
# to a shared lib (libmain.so) that SDLActivity loads; there is no real filesystem to scan inside an
# APK, so this is always a shipping config (manifest-load, no scan) with the desktop-only editor/ImGui
# compiled out. Forced here, before the editor-forces-ImGui rule below.
#
# Semantically equivalent to the `ship-release` CMake preset (desktop shipping config). AGP can't
# consume `cmake --preset`, so Android reaches the same OCTARINE_SHIPPED=ON + editor/ImGui-off state
# through this branch instead of the preset. Keep the two in sync if either set of forced flags changes.
if (ANDROID)
    set(OCTARINE_WITH_EDITOR OFF CACHE BOOL "Disabled on Android" FORCE)
    set(OCTARINE_WITH_IMGUI OFF CACHE BOOL "Disabled on Android" FORCE)
    set(OCTARINE_SHIPPED ON CACHE BOOL "Forced ON on Android (manifest-load, no FS scan)" FORCE)
endif ()

# iOS build target is parked on the defer/ios branch pending an Apple Developer account.

if (OCTARINE_WITH_EDITOR)
    set(OCTARINE_WITH_IMGUI ON CACHE BOOL "Forced ON by Editor" FORCE)
endif ()

# Set output directories for all build types
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/release")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/bin/relwithdebinfo")

# Dependencies
find_package(glm CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
find_package(SDL3_mixer CONFIG REQUIRED)
find_package(Lua REQUIRED)
find_package(sol2 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
if (OCTARINE_WITH_IMGUI)
    find_package(imgui CONFIG REQUIRED)
endif ()
find_package(Threads REQUIRED)

# Engine log level (compile-time default baked into the binary). Policy when OCTARINE_LOG_LEVEL is
# empty (the default):
#   SHIPPED build  → warn   (production: errors + warnings only)
#   Debug build    → debug  (dev: noisy)
#   else           → info   (player-release / RelWithDebInfo)
# Runtime can lower the bar with `LogLevel=` in config.ini — read at Game::Setup, applied via
# Logger::SetLevel on top of this default. See src/General/Logger.cpp.
set(OCTARINE_LOG_LEVEL "" CACHE STRING "Compile-time default engine log level (trace|debug|info|warn|error|critical|off); empty = auto-pick from build type / OCTARINE_SHIPPED")
if (NOT OCTARINE_LOG_LEVEL)
    if (OCTARINE_SHIPPED)
        set(_octarine_default_log_level "warn")
    elseif (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "")
        set(_octarine_default_log_level "debug")
    else ()
        set(_octarine_default_log_level "info")
    endif ()
else ()
    set(_octarine_default_log_level "${OCTARINE_LOG_LEVEL}")
endif ()
message(STATUS "Octarine: default log level = ${_octarine_default_log_level}")

if (OCTARINE_ENABLE_PROFILING)
    message(STATUS "Octarine: Profiling ENABLED")
endif ()
if (OCTARINE_WITH_IMGUI)
    message(STATUS "Octarine: ImGui Debug UI ENABLED")
endif ()
if (OCTARINE_WITH_EDITOR)
    message(STATUS "Octarine: Editor ENABLED")
endif ()
if (OCTARINE_SHIPPED)
    message(STATUS "Octarine: SHIPPED build — asset catalog loads baked manifest (no scan)")
endif ()

# sol2 ImGui binding lib: consumed as a prebuilt, binary-cached vcpkg port
# (vcpkg-overlay-ports/sol2-imgui-bindings) instead of an in-tree submodule + add_subdirectory, so
# a clean build restores it from the vcpkg binary cache rather than recompiling the heavy generated
# TUs. Only meaningful when ImGui is on; pulled in PUBLIC by octarine_editor.
if (OCTARINE_WITH_IMGUI)
    find_package(sol2_ImGui_Bindings CONFIG REQUIRED)
endif ()

# Standard compile flags + warnings, packaged as one helper so every static lib + the exe pick up
# the same set. Replaces the per-target blocks that used to live in this file.
include(cmake/octarine_library.cmake)

# Per-layer STATIC libraries: octarine_{core,assets,renderer,lua,systems,editor,engine}.
# octarine_engine is the top-level coordinator that depends on every other lib transitively.
add_subdirectory(src)

# Final OctarineEngine artifact. On Android, SDLActivity dlopens libmain.so and jumps to SDL_main,
# so the entry point must live in a shared lib (OUTPUT_NAME main → libmain.so). Everywhere else
# it is a plain executable.
#
# Desktop player presets (player-debug / player-profile / player-release) rename the binary to
# `OctarineEngine-player` so it can coexist with the editor (also `OctarineEngine`) in the same
# dev tree — the editor's Run Player toolbar action resolves the player by that fixed name.
# Shipping configs (ship-release / packaging) keep the canonical `OctarineEngine` name —
# downstream packaging owns the shipped artifact's filename.
if (ANDROID)
    add_library(${PROJECT_NAME} SHARED src/Main.cpp)
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME main)
else ()
    add_executable(${PROJECT_NAME} src/Main.cpp)
    if (NOT OCTARINE_WITH_EDITOR AND NOT OCTARINE_SHIPPED)
        set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME OctarineEngine-player)
    endif ()
endif ()

set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)

target_link_libraries(${PROJECT_NAME} PRIVATE octarine_engine)

# Android NDK system libs — linker needs them named explicitly when SDL3 is statically linked from
# the arm64-android vcpkg triplet.
if (ANDROID)
    target_link_libraries(${PROJECT_NAME} PRIVATE android log)
endif ()

# Compile flags for the final exe match the per-lib helper's settings (the exe TU is just
# Main.cpp but consistency makes the link-line warnings predictable).
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
            /W4 /permissive- /MP /bigobj
            /w14242 /w14244 /w14254 /w44456 /w44457 /w44458 /w44459 /wd4201 /wd5321
    )
    target_compile_options(${PROJECT_NAME} PRIVATE
            $<IF:$<CONFIG:Debug>,,-WX /arch:AVX2>
    )
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE
            -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wfatal-errors
    )
    if (ANDROID)
        target_compile_options(${PROJECT_NAME} PRIVATE
                $<IF:$<CONFIG:Debug>,,-ffast-math>
        )
    else ()
        target_compile_options(${PROJECT_NAME} PRIVATE
                $<IF:$<CONFIG:Debug>,,-Werror ${OCTARINE_ARCH_FLAG} -ffast-math>
        )
    endif ()
    target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<NOT:$<CONFIG:Debug>>:-ffunction-sections>
            $<$<NOT:$<CONFIG:Debug>>:-fdata-sections>
    )
    if (APPLE)
        target_link_options(${PROJECT_NAME} PRIVATE
                $<$<NOT:$<CONFIG:Debug>>:-Wl,-dead_strip>
                $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>>:-Wl,-x>
        )
    else ()
        target_link_options(${PROJECT_NAME} PRIVATE
                $<$<NOT:$<CONFIG:Debug>>:-Wl,--gc-sections>
                $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>>:-s>
        )
    endif ()
endif ()

# Install rules (optional). Skipped under packaging — octarine_package owns the deliverable layout
# (binary + project files at the package root) instead of the bin/lib split used by `cmake --install`.
if (NOT OCTARINE_PACKAGE_PROJECT AND NOT ANDROID)
    install(TARGETS ${PROJECT_NAME}
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
    )
endif ()

# Packaging: wire the deliverable layout when a project dir is supplied. Android packages via Gradle
# (see android/) so the helper is skipped there entirely.
if (OCTARINE_PACKAGE_PROJECT AND NOT ANDROID)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/OctarinePackage.cmake)
    octarine_package(${PROJECT_NAME} PROJECT "${OCTARINE_PACKAGE_PROJECT}" NAME "${PROJECT_NAME}")
endif ()

# -----------------------------------------------------------------------------
# Linting & Formatting Integration
# -----------------------------------------------------------------------------

option(OCTARINE_ENABLE_LINTING "Run clang-tidy during compilation" OFF)

if (OCTARINE_ENABLE_LINTING)
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
    if (CLANG_TIDY_EXE)
        # Apply to every internal lib + the exe so the whole engine TU set gets linted.
        foreach (_tgt octarine_core octarine_assets octarine_renderer octarine_lua octarine_systems
                octarine_editor octarine_engine ${PROJECT_NAME})
            if (TARGET ${_tgt})
                set_target_properties(${_tgt} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
            endif ()
        endforeach ()
        message(STATUS "Octarine: clang-tidy ENABLED (reading from .clang-tidy)")
    else ()
        message(WARNING "Octarine: Linting enabled but clang-tidy executable not found!")
    endif ()
endif ()

find_program(CLANG_FORMAT_EXE NAMES "clang-format")
if (CLANG_FORMAT_EXE)
    # Gather all source files in the src/ directory
    file(GLOB_RECURSE FORMAT_SOURCES
            "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
    )

    # Create the custom format target
    add_custom_target(format
            COMMAND ${CLANG_FORMAT_EXE} -i -style=file ${FORMAT_SOURCES}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Formatting source code with clang-format"
    )
    message(STATUS "Octarine: clang-format target ADDED")
endif ()

# -----------------------------------------------------------------------------
# Benchmarks
# -----------------------------------------------------------------------------

option(OCTARINE_ENABLE_BENCHMARKS "Build benchmarks" OFF)

if (OCTARINE_ENABLE_BENCHMARKS)
    find_package(benchmark CONFIG REQUIRED)

    # octarine_core pulls the ECS + Logger TUs (and SDL3, glm, spdlog transitively) plus the
    # standard compile flags. The spatial-audio systems benchmarked here are header-only, so no
    # other engine lib is needed — bench runs are microbenchmarks, not full-engine timings.
    add_executable(OctarineBenchmarks
            tests/benchmarks/EntityPoolBenchmark.cpp
            tests/benchmarks/SpatialAudioBenchmark.cpp
            tests/benchmarks/CollisionSystemBenchmark.cpp
            tests/benchmarks/TextCacheBenchmark.cpp
    )
    set_target_properties(OctarineBenchmarks PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
    )

    # SDL3_mixer is required by SpatialAudioBenchmark: the fixture initializes a real MIX_Mixer
    # (under the "dummy" audio driver) and allocates real MIX_Tracks so the systems' MIX_Set* call
    # path is exercised end-to-end rather than mocked. SDL3 itself comes in via octarine_core.
    target_link_libraries(OctarineBenchmarks PRIVATE
            octarine_core
            benchmark::benchmark
            benchmark::benchmark_main
            $<IF:$<TARGET_EXISTS:SDL3_mixer::SDL3_mixer>,SDL3_mixer::SDL3_mixer,SDL3_mixer::SDL3_mixer-static>
    )

    if (MSVC)
        target_compile_options(OctarineBenchmarks PRIVATE /W4 /permissive- /MP /bigobj)
        target_compile_options(OctarineBenchmarks PRIVATE
                $<IF:$<CONFIG:Debug>,,-WX /arch:AVX2>
        )
    else ()
        target_compile_options(OctarineBenchmarks PRIVATE -Wall -Wextra -Wpedantic)
        target_compile_options(OctarineBenchmarks PRIVATE
                $<IF:$<CONFIG:Debug>,,-Werror ${OCTARINE_ARCH_FLAG} -ffast-math>
        )
    endif ()

    message(STATUS "Octarine: Benchmarks ENABLED")
endif ()

# -----------------------------------------------------------------------------
# Tests
# -----------------------------------------------------------------------------

option(OCTARINE_ENABLE_TESTS "Build engine tests" OFF)

if (OCTARINE_ENABLE_TESTS)
    enable_testing()

    # Both engine-spanning tests link octarine_engine — same surface the real exe sees, minus
    # Main.cpp. The test .cpp constructs a Game directly and replays its Setup sequence headless.
    add_executable(OctarineLuaApiTest tests/LuaApiSmokeTest.cpp)
    set_target_properties(OctarineLuaApiTest PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
    )
    # Smoke test writes its EmmyLua stub + JSON indexes here so repo-root copies stay in sync with
    # the live bindings; CI diffs all three against tracked files to fail on drift.
    target_compile_definitions(OctarineLuaApiTest PRIVATE
            LUA_API_SMOKE_OUTPUT="${CMAKE_CURRENT_SOURCE_DIR}/lua_api.smoke.lua"
            LUA_API_COMPONENTS_OUTPUT="${CMAKE_CURRENT_SOURCE_DIR}/components.json"
            LUA_API_MODULES_OUTPUT="${CMAKE_CURRENT_SOURCE_DIR}/modules.json")
    target_link_libraries(OctarineLuaApiTest PRIVATE octarine_engine)
    if (MSVC)
        target_compile_options(OctarineLuaApiTest PRIVATE /MP /bigobj /permissive- /wd4201 /wd5321)
    endif ()
    add_test(NAME LuaApiSmokeTest COMMAND OctarineLuaApiTest)

    # Asset pipeline checks (metadata defaults + catalog build). Same target shape — links the
    # full engine via octarine_engine so AssetCatalog::Build (which needs GameConfig / sol2) is
    # reachable without re-plumbing sources.
    add_executable(OctarineAssetPipelineTest tests/AssetPipelineTest.cpp)
    set_target_properties(OctarineAssetPipelineTest PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
    )
    target_compile_definitions(OctarineAssetPipelineTest PRIVATE
            ASSET_TEST_FIXTURE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/assets")
    target_link_libraries(OctarineAssetPipelineTest PRIVATE octarine_engine)
    if (MSVC)
        target_compile_options(OctarineAssetPipelineTest PRIVATE /MP /bigobj /permissive- /wd4201 /wd5321)
    endif ()
    add_test(NAME AssetPipelineTest COMMAND OctarineAssetPipelineTest)

    # Process-spawn smoke test: standalone — no engine sources required, just the wrapper.
    # Validates argv quoting + spawn/wait/output capture against `cmake --version` on PATH.
    add_executable(OctarineProcessTest
            src/Process/Process.cpp
            tests/ProcessTest.cpp
    )
    set_target_properties(OctarineProcessTest PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
    )
    target_include_directories(OctarineProcessTest PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    target_link_libraries(OctarineProcessTest PRIVATE Threads::Threads)
    if (MSVC)
        target_compile_options(OctarineProcessTest PRIVATE /W4 /permissive- /MP /EHsc)
    endif ()
    add_test(NAME ProcessTest COMMAND OctarineProcessTest)

    # project.ini reader smoke test: standalone — no engine sources required. Validates parse +
    # shipping-identity rules (mirrors cmake/OctarinePackage.cmake + android/app/build.gradle).
    add_executable(OctarineProjectIniTest
            src/Project/ProjectIni.cpp
            tests/ProjectIniTest.cpp
    )
    set_target_properties(OctarineProjectIniTest PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
    )
    target_include_directories(OctarineProjectIniTest PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    if (MSVC)
        target_compile_options(OctarineProjectIniTest PRIVATE /W4 /permissive- /MP /EHsc)
    endif ()
    add_test(NAME ProjectIniTest COMMAND OctarineProjectIniTest)

    # adb wrapper parser test: standalone — Adb + its Process/Logger deps. Exercises the pure
    # `adb devices -l` parser; no device or adb binary required.
    add_executable(OctarineAdbTest
            src/Deploy/Adb.cpp
            src/Process/Process.cpp
            src/General/Logger.cpp
            tests/AdbTest.cpp
    )
    set_target_properties(OctarineAdbTest PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
    )
    target_include_directories(OctarineAdbTest PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    target_link_libraries(OctarineAdbTest PRIVATE spdlog::spdlog Threads::Threads)
    if (MSVC)
        target_compile_options(OctarineAdbTest PRIVATE /W4 /permissive- /MP /EHsc)
    endif ()
    add_test(NAME AdbTest COMMAND OctarineAdbTest)

    # Headless ECS / system tests. These link only the ECS core (Registry + Logger) — no SDL,
    # window, or Lua — exactly like the benchmark target, so they build and run fast. One helper
    # per target keeps the registrations DRY.
    function(octarine_add_core_test exe_name ctest_name source)
        add_executable(${exe_name} ${source}
                src/ECS/Registry.cpp
                src/General/Logger.cpp)
        set_target_properties(${exe_name} PROPERTIES
                CXX_STANDARD 20
                CXX_STANDARD_REQUIRED ON
                CXX_EXTENSIONS OFF)
        target_compile_definitions(${exe_name} PRIVATE GLM_FORCE_INTRINSICS)
        target_include_directories(${exe_name} PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/src
                ${CMAKE_CURRENT_SOURCE_DIR}/libs)
        target_link_libraries(${exe_name} PRIVATE glm::glm spdlog::spdlog Threads::Threads)
        if (MSVC)
            target_compile_options(${exe_name} PRIVATE /W4 /permissive- /MP /EHsc /bigobj)
        endif ()
        add_test(NAME ${ctest_name} COMMAND ${exe_name})
    endfunction()

    octarine_add_core_test(OctarineEcsTest EcsRegistryTest tests/EcsRegistryTest.cpp)
    octarine_add_core_test(OctarineEcsHierarchyTest EcsHierarchyTest tests/EcsHierarchyTest.cpp)
    octarine_add_core_test(OctarineSystemLogicTest SystemLogicTest tests/SystemLogicTest.cpp)
    octarine_add_core_test(OctarineCollisionResponseTest CollisionResponseTest tests/CollisionResponseTest.cpp)
    octarine_add_core_test(OctarineCollisionSystemTest CollisionSystemTest tests/CollisionSystemTest.cpp)
    octarine_add_core_test(OctarineProjectileEmitSystemTest ProjectileEmitSystemTest tests/ProjectileEmitSystemTest.cpp)

    # EventBus is header-only; it needs Logger for its construction log lines, nothing else.
    add_executable(OctarineEventBusTest
            src/General/Logger.cpp
            tests/EventBusTest.cpp)
    set_target_properties(OctarineEventBusTest PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF)
    target_include_directories(OctarineEventBusTest PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(OctarineEventBusTest PRIVATE spdlog::spdlog)
    if (MSVC)
        target_compile_options(OctarineEventBusTest PRIVATE /W4 /permissive- /MP /EHsc)
    endif ()
    add_test(NAME EventBusTest COMMAND OctarineEventBusTest)

    # AssetRefcounter unit test (Stage 10): the acquire-count policy split out of AssetManager is
    # pure bookkeeping — no SDL, no renderer, no Logger — so the test links nothing but the standard
    # library. This is the "catalog/asset logic testable without SDL_CreateRenderer" outcome.
    add_executable(OctarineAssetRefcounterTest tests/AssetRefcounterTest.cpp)
    set_target_properties(OctarineAssetRefcounterTest PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF)
    target_include_directories(OctarineAssetRefcounterTest PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src)
    if (MSVC)
        target_compile_options(OctarineAssetRefcounterTest PRIVATE /W4 /permissive- /MP /EHsc)
    endif ()
    add_test(NAME AssetRefcounterTest COMMAND OctarineAssetRefcounterTest)

    # FileWatcher unit test: the generic mtime watcher shared by the script + asset hot-reload
    # paths. Pure filesystem — links only the watcher source + Logger (for its stat-warning line).
    add_executable(OctarineFileWatcherTest
            src/General/FileWatcher.cpp
            src/General/Logger.cpp
            tests/FileWatcherTest.cpp)
    set_target_properties(OctarineFileWatcherTest PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF)
    target_include_directories(OctarineFileWatcherTest PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    target_link_libraries(OctarineFileWatcherTest PRIVATE spdlog::spdlog)
    if (MSVC)
        target_compile_options(OctarineFileWatcherTest PRIVATE /W4 /permissive- /MP /EHsc)
    endif ()
    add_test(NAME FileWatcherTest COMMAND OctarineFileWatcherTest)

    # DevListenServer smoke test (Stage 6 PR-A): standalone — only the server source + Logger.
    # Bound to the same OCTARINE_SHIPPED gate as the engine-side server.
    if (NOT OCTARINE_SHIPPED)
        add_executable(OctarineDevListenServerTest
                src/Dev/DevListenServer.cpp
                src/General/Logger.cpp
                tests/DevListenServerTest.cpp)
        set_target_properties(OctarineDevListenServerTest PROPERTIES
                CXX_STANDARD 20
                CXX_STANDARD_REQUIRED ON
                CXX_EXTENSIONS OFF)
        target_include_directories(OctarineDevListenServerTest PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/src)
        target_link_libraries(OctarineDevListenServerTest PRIVATE spdlog::spdlog Threads::Threads)
        if (WIN32)
            target_link_libraries(OctarineDevListenServerTest PRIVATE ws2_32)
        endif ()
        if (MSVC)
            target_compile_options(OctarineDevListenServerTest PRIVATE /W4 /permissive- /MP /EHsc)
        endif ()
        add_test(NAME DevListenServerTest COMMAND OctarineDevListenServerTest)
    endif ()

    # Lua behavior test: calls bound component methods from Lua and verifies the C++ side mutated
    # (damage/heal round-trip, clamping setters, fromLua defaults). Links the full engine via
    # octarine_engine — same surface as the smoke test, minus Main.cpp.
    add_executable(OctarineLuaBehaviorTest tests/LuaBehaviorTest.cpp)
    set_target_properties(OctarineLuaBehaviorTest PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF)
    target_link_libraries(OctarineLuaBehaviorTest PRIVATE octarine_engine)
    if (MSVC)
        target_compile_options(OctarineLuaBehaviorTest PRIVATE /MP /bigobj /permissive- /wd4201 /wd5321)
    endif ()
    add_test(NAME LuaBehaviorTest COMMAND OctarineLuaBehaviorTest)

    # Game bake test: headless Game construction + Game::Bake against a minimal fixture project +
    # SceneAssetScanner::CollectRefs provenance / cross-scene delta. BAKE_FIXTURE_DIR points the
    # end-to-end bake check at tests/fixtures/bake-project.
    add_executable(OctarineGameBakeTest tests/GameBakeTest.cpp)
    set_target_properties(OctarineGameBakeTest PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF)
    target_compile_definitions(OctarineGameBakeTest PRIVATE
            BAKE_FIXTURE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/bake-project")
    target_link_libraries(OctarineGameBakeTest PRIVATE octarine_engine)
    if (MSVC)
        target_compile_options(OctarineGameBakeTest PRIVATE /MP /bigobj /permissive- /wd4201 /wd5321)
    endif ()
    add_test(NAME GameBakeTest COMMAND OctarineGameBakeTest)

    message(STATUS "Octarine: Tests ENABLED")
endif ()
