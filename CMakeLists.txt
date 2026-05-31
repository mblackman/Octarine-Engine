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

# LTO for non-Debug builds. Combines with --gc-sections to drop unreferenced
# code/data from statically linked third-party libs (SDL3, Lua, etc).
include(CheckIPOSupported)
check_ipo_supported(RESULT OCTARINE_IPO_SUPPORTED OUTPUT OCTARINE_IPO_ERROR)
if (OCTARINE_IPO_SUPPORTED)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO ON)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_MINSIZEREL ON)
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

# iOS build target is parked on the defer/ios branch pending an Apple Developer account; see
# ai/iOSDeferralPlan.md.

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

# Source files
set(SRC_FILES
        src/Main.cpp
        src/AssetManager/AssetManager.cpp
        src/AssetManager/AssetCatalog.cpp
        src/AssetManager/AssetPak.cpp
        src/AssetManager/AtlasBaker.cpp
        src/AssetManager/AudioNormalizer.cpp
        src/AssetManager/GlyphAtlas.cpp
        src/AssetManager/TextureAtlasBaker.cpp
        src/AssetManager/SceneAssetScanner.cpp
        src/ECS/Registry.cpp
        src/Game/Game.cpp
        src/Game/GameConfig.cpp
        src/General/Logger.cpp
        src/Lua/Bindings/RegisterAllBindings.cpp
        src/Lua/LuaApiManifest.cpp
        src/Lua/Modules/RegisterAllModules.cpp
        src/Lua/Modules/AssetsModuleLuaBinding.cpp
        src/Lua/Modules/AudioModuleLuaBinding.cpp
        src/Lua/Modules/EntityModuleLuaBinding.cpp
        src/Lua/Modules/GameModuleLuaBinding.cpp
        src/Lua/Modules/IoModuleLuaBinding.cpp
        src/Lua/Modules/LogModuleLuaBinding.cpp
        src/Lua/Modules/SceneModuleLuaBinding.cpp
        src/Process/Process.cpp
        src/Project/ProjectIni.cpp
        src/Renderer/Renderer.cpp
        src/Systems/AudioSystem.cpp
)

if (OCTARINE_WITH_IMGUI)
    list(APPEND SRC_FILES src/Systems/RenderDebugGUISystem.cpp)
endif ()

if (OCTARINE_WITH_EDITOR)
    list(APPEND SRC_FILES src/Editor/EditorPersistence.cpp)
    list(APPEND SRC_FILES src/Editor/EditorLayoutPresets.cpp)
    list(APPEND SRC_FILES src/Editor/Inspectors/RegisterAllInspectors.cpp)
endif ()

if (UNIX AND NOT APPLE AND NOT ANDROID)
    list(APPEND SRC_FILES src/General/SDL3_Linux_Stubs.cpp)
endif ()

# TextureAtlasBaker.cpp is the single TU that emits the stb_image / stb_image_write / stb_rect_pack
# implementations. The vendored headers don't satisfy the engine's `-Wconversion -Werror`
# baseline (int -> short_int, int -> size_t, etc.), so disable warnings just for this file. The
# user-authored code at the bottom of the .cpp is small enough to eyeball without -Wall help.
set_source_files_properties(src/AssetManager/TextureAtlasBaker.cpp PROPERTIES
        COMPILE_OPTIONS "$<IF:$<CXX_COMPILER_ID:MSVC>,/w,-w>")

# Add packaged libs
if (OCTARINE_WITH_IMGUI)
    add_subdirectory(libs/sol2_ImGui_Bindings)
endif ()

# Create the engine target. On Android, SDLActivity dlopens libmain.so and jumps to SDL_main, so the
# engine must be that shared lib (OUTPUT_NAME main → libmain.so). Everywhere else it is a plain
# executable.
if (ANDROID)
    add_library(${PROJECT_NAME} SHARED ${SRC_FILES})
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME main)
else ()
    add_executable(${PROJECT_NAME} ${SRC_FILES})
endif ()

# Set target properties
set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)

if (OCTARINE_ENABLE_PROFILING)
    target_compile_definitions(${PROJECT_NAME} PRIVATE OCTARINE_PROFILING)
    message(STATUS "Octarine: Profiling ENABLED")
endif ()

if (OCTARINE_WITH_IMGUI)
    target_compile_definitions(${PROJECT_NAME} PRIVATE OCTARINE_WITH_IMGUI)
    message(STATUS "Octarine: ImGui Debug UI ENABLED")
endif ()

if (OCTARINE_WITH_EDITOR)
    target_compile_definitions(${PROJECT_NAME} PRIVATE OCTARINE_WITH_EDITOR)
    message(STATUS "Octarine: Editor ENABLED")
endif ()

if (OCTARINE_SHIPPED)
    target_compile_definitions(${PROJECT_NAME} PRIVATE OCTARINE_SHIPPED)
    message(STATUS "Octarine: SHIPPED build — asset catalog loads baked manifest (no scan)")
endif ()

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
target_compile_definitions(${PROJECT_NAME} PRIVATE OCTARINE_LOG_LEVEL_NAME="${_octarine_default_log_level}")
message(STATUS "Octarine: default log level = ${_octarine_default_log_level}")

# Include directories
target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/libs
        # libs/stb is its own include root so src/AssetManager/TextureAtlasBaker.cpp can write
        # `#include "stb_image.h"` — matches the pattern in scripts/octarine-icon-tool/.
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/stb
        ${LUA_INCLUDE_DIR}
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        glm::glm
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
        $<IF:$<TARGET_EXISTS:SDL3_mixer::SDL3_mixer>,SDL3_mixer::SDL3_mixer,SDL3_mixer::SDL3_mixer-static>
        $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,SDL3_image::SDL3_image-shared,SDL3_image::SDL3_image-static>
        ${LUA_LIBRARIES}
        spdlog::spdlog
        sol2::sol2
        Threads::Threads
)

if (OCTARINE_WITH_IMGUI)
    target_link_libraries(${PROJECT_NAME}
            PRIVATE
            imgui::imgui
            sol2_ImGui_Bindings
    )
endif ()

# Android NDK system libs. SDL3 (statically linked from the arm64-android vcpkg triplet) pulls in the
# native logging and platform APIs; the linker needs them named explicitly for the shared lib.
if (ANDROID)
    target_link_libraries(${PROJECT_NAME} PRIVATE android log)
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

# Compiler-specific flags - Treat warnings as errors in prod builds
if (MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE GLM_FORCE_INTRINSICS)
    target_compile_options(${PROJECT_NAME} PRIVATE
            /W4
            /permissive-
            /MP
            /bigobj
            /w14242 # Conversion: possible loss of data
            /w14244 # Conversion: possible loss of data (float/double/int)
            /w14254 # Conversion: possible loss of data (larger to smaller type)
            /w44456 # Shadowing: declaration hides previous local declaration
            /w44457 # Shadowing: declaration hides function parameter
            /w44458 # Shadowing: declaration hides class member
            /w44459 # Shadowing: declaration hides global declaration
            /wd4201 # nonstandard extension used: nameless struct/union (common in GLM)
            /wd5321 # nonstandard extension used: encoding UTF-8 character (common in sol2 headers)
    )
    target_compile_options(${PROJECT_NAME} PRIVATE
            $<IF:$<CONFIG:Debug>,,-WX /arch:AVX2>
    )
else ()
    target_compile_definitions(${PROJECT_NAME} PRIVATE GLM_FORCE_INTRINSICS)
    target_compile_options(${PROJECT_NAME} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wshadow
            -Wconversion
            -Wfatal-errors
    )
    if (ANDROID)
        # Cross-compile: -march=native would target the build host, not the device. -Werror is dropped
        # because device-arch-specific warnings can't be observed/cleared from a desktop checkout.
        target_compile_options(${PROJECT_NAME} PRIVATE
                $<IF:$<CONFIG:Debug>,,-ffast-math>
        )
    else ()
        target_compile_options(${PROJECT_NAME} PRIVATE
                $<IF:$<CONFIG:Debug>,,-Werror -march=native -ffast-math>
        )
    endif ()
    # Size reduction for non-Debug builds:
    # - function/data sections + --gc-sections drops unreferenced symbols from static libs
    # - -s strips the linked binary
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

# -----------------------------------------------------------------------------
# Linting & Formatting Integration
# -----------------------------------------------------------------------------

option(OCTARINE_ENABLE_LINTING "Run clang-tidy during compilation" OFF)

if (OCTARINE_ENABLE_LINTING)
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
    if (CLANG_TIDY_EXE)
        set_target_properties(${PROJECT_NAME} PROPERTIES
                CXX_CLANG_TIDY "${CLANG_TIDY_EXE}"
        )
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

    set(BENCHMARK_SRC_FILES
            tests/benchmarks/EntityPoolBenchmark.cpp
            src/ECS/Registry.cpp
            src/General/Logger.cpp
    )

    add_executable(OctarineBenchmarks ${BENCHMARK_SRC_FILES})

    # Always compile in Release mode flags for the benchmark where possible,
    # though it will inherit CMake build type
    target_include_directories(OctarineBenchmarks PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${CMAKE_CURRENT_SOURCE_DIR}/libs
    )

    target_link_libraries(OctarineBenchmarks PRIVATE
            benchmark::benchmark
            benchmark::benchmark_main
            glm::glm
            spdlog::spdlog
            Threads::Threads
    )

    if (MSVC)
        target_compile_definitions(OctarineBenchmarks PRIVATE GLM_FORCE_INTRINSICS)
        target_compile_options(OctarineBenchmarks PRIVATE /W4 /permissive- /MP /bigobj)
        target_compile_options(OctarineBenchmarks PRIVATE
                $<IF:$<CONFIG:Debug>,,-WX /arch:AVX2>
        )
    else ()
        target_compile_definitions(OctarineBenchmarks PRIVATE GLM_FORCE_INTRINSICS)
        target_compile_options(OctarineBenchmarks PRIVATE -Wall -Wextra -Wpedantic)
        target_compile_options(OctarineBenchmarks PRIVATE
                $<IF:$<CONFIG:Debug>,,-Werror -march=native -ffast-math>
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

    # The Lua API smoke test links the whole engine (it constructs a Game) minus the real
    # entry point, then replays Game::Setup's binding sequence headless.
    set(TEST_SRC_FILES ${SRC_FILES})
    list(REMOVE_ITEM TEST_SRC_FILES src/Main.cpp)
    list(APPEND TEST_SRC_FILES tests/LuaApiSmokeTest.cpp)

    add_executable(OctarineLuaApiTest ${TEST_SRC_FILES})

    set_target_properties(OctarineLuaApiTest PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
    )

    # Mirror the main target's feature defines so the shared sources compile identically.
    target_compile_definitions(OctarineLuaApiTest PRIVATE GLM_FORCE_INTRINSICS)
    # Smoke test writes its EmmyLua stub + JSON indexes here so repo-root copies stay in sync with
    # the live bindings; CI diffs all three against tracked files to fail on drift.
    target_compile_definitions(OctarineLuaApiTest PRIVATE
            LUA_API_SMOKE_OUTPUT="${CMAKE_CURRENT_SOURCE_DIR}/lua_api.smoke.lua"
            LUA_API_COMPONENTS_OUTPUT="${CMAKE_CURRENT_SOURCE_DIR}/components.json"
            LUA_API_MODULES_OUTPUT="${CMAKE_CURRENT_SOURCE_DIR}/modules.json")
    if (OCTARINE_WITH_IMGUI)
        target_compile_definitions(OctarineLuaApiTest PRIVATE OCTARINE_WITH_IMGUI)
    endif ()
    if (OCTARINE_WITH_EDITOR)
        target_compile_definitions(OctarineLuaApiTest PRIVATE OCTARINE_WITH_EDITOR)
    endif ()
    if (OCTARINE_ENABLE_PROFILING)
        target_compile_definitions(OctarineLuaApiTest PRIVATE OCTARINE_PROFILING)
    endif ()

    target_include_directories(OctarineLuaApiTest PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${CMAKE_CURRENT_SOURCE_DIR}/libs
            ${CMAKE_CURRENT_SOURCE_DIR}/libs/stb
            ${LUA_INCLUDE_DIR}
    )

    target_link_libraries(OctarineLuaApiTest PRIVATE
            glm::glm
            SDL3::SDL3
            SDL3_ttf::SDL3_ttf
            $<IF:$<TARGET_EXISTS:SDL3_mixer::SDL3_mixer>,SDL3_mixer::SDL3_mixer,SDL3_mixer::SDL3_mixer-static>
            $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,SDL3_image::SDL3_image-shared,SDL3_image::SDL3_image-static>
            ${LUA_LIBRARIES}
            spdlog::spdlog
            sol2::sol2
            Threads::Threads
    )
    if (OCTARINE_WITH_IMGUI)
        target_link_libraries(OctarineLuaApiTest PRIVATE imgui::imgui sol2_ImGui_Bindings)
    endif ()

    # /bigobj: sol2 + the aggregated engine TU exceed MSVC's default section limit.
    if (MSVC)
        target_compile_options(OctarineLuaApiTest PRIVATE /MP /bigobj /permissive- /wd4201 /wd5321)
    endif ()

    add_test(NAME LuaApiSmokeTest COMMAND OctarineLuaApiTest)

    # Asset pipeline checks (metadata defaults + catalog build). Links the full engine minus the
    # real entry point, matching the smoke test, so the tests can exercise AssetCatalog::Build
    # (which needs GameConfig / sol2) without re-plumbing the target.
    set(ASSET_TEST_SRC_FILES ${SRC_FILES})
    list(REMOVE_ITEM ASSET_TEST_SRC_FILES src/Main.cpp)
    list(APPEND ASSET_TEST_SRC_FILES tests/AssetPipelineTest.cpp)

    add_executable(OctarineAssetPipelineTest ${ASSET_TEST_SRC_FILES})

    set_target_properties(OctarineAssetPipelineTest PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
    )

    target_compile_definitions(OctarineAssetPipelineTest PRIVATE GLM_FORCE_INTRINSICS)
    target_compile_definitions(OctarineAssetPipelineTest PRIVATE
            ASSET_TEST_FIXTURE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/assets")
    if (OCTARINE_WITH_IMGUI)
        target_compile_definitions(OctarineAssetPipelineTest PRIVATE OCTARINE_WITH_IMGUI)
    endif ()
    if (OCTARINE_WITH_EDITOR)
        target_compile_definitions(OctarineAssetPipelineTest PRIVATE OCTARINE_WITH_EDITOR)
    endif ()
    if (OCTARINE_ENABLE_PROFILING)
        target_compile_definitions(OctarineAssetPipelineTest PRIVATE OCTARINE_PROFILING)
    endif ()

    target_include_directories(OctarineAssetPipelineTest PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${CMAKE_CURRENT_SOURCE_DIR}/libs
            ${CMAKE_CURRENT_SOURCE_DIR}/libs/stb
            ${LUA_INCLUDE_DIR}
    )

    target_link_libraries(OctarineAssetPipelineTest PRIVATE
            glm::glm
            SDL3::SDL3
            SDL3_ttf::SDL3_ttf
            $<IF:$<TARGET_EXISTS:SDL3_mixer::SDL3_mixer>,SDL3_mixer::SDL3_mixer,SDL3_mixer::SDL3_mixer-static>
            $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,SDL3_image::SDL3_image-shared,SDL3_image::SDL3_image-static>
            ${LUA_LIBRARIES}
            spdlog::spdlog
            sol2::sol2
            Threads::Threads
    )
    if (OCTARINE_WITH_IMGUI)
        target_link_libraries(OctarineAssetPipelineTest PRIVATE imgui::imgui sol2_ImGui_Bindings)
    endif ()

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

    message(STATUS "Octarine: Tests ENABLED")
endif ()
