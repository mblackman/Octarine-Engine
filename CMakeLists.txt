cmake_minimum_required(VERSION 3.15...4.4)

# =============================================================================
# Project Definition
# =============================================================================

project(OctarineEngine
        VERSION 0.2.0
        DESCRIPTION "A lightweight 2D game engine of a yellowish-purple color"
        LANGUAGES CXX
)

# Global language standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# =============================================================================
# Build Options & Feature Toggles
# =============================================================================

# Engine features
option(OCTARINE_WITH_EDITOR "Build with the editor environment" ON)
option(OCTARINE_WITH_IMGUI "Build with Dear ImGui debug tools" ON)
option(OCTARINE_ENABLE_PROFILING "Enable per-system performance instrumentation" OFF)
option(OCTARINE_SHIPPED "Build a shipping artifact (loads baked asset manifest, skips filesystem scan)" OFF)

# Packaging & distribution
set(OCTARINE_PACKAGE_PROJECT "" CACHE PATH "Game project directory to package into a standalone release artifact (empty = no packaging)")
set(OCTARINE_LOG_LEVEL "" CACHE STRING "Compile-time default log level (trace|debug|info|warn|error|critical|off; empty = auto-detect)")

# Compiler & performance tuning
option(OCTARINE_USE_CCACHE "Use ccache or sccache as compiler launcher when available" ON)
option(OCTARINE_NATIVE_ARCH "Tune optimized builds for host CPU (-march=native; not portable, keep off for distribution)" OFF)
set(OCTARINE_SANITIZE "OFF" CACHE STRING "Sanitizer instrumentation: OFF, asan (ASan+UBSan), or tsan")
set_property(CACHE OCTARINE_SANITIZE PROPERTY STRINGS OFF asan tsan)

# Tooling, benchmarks, and testing
option(OCTARINE_ENABLE_LINTING "Run clang-tidy during compilation" OFF)
option(OCTARINE_ENABLE_BENCHMARKS "Build benchmarks" OFF)
option(OCTARINE_ENABLE_TESTS "Build engine tests" OFF)

# =============================================================================
# Platform Overrides & Constraints
# =============================================================================

# Android is always a shipping shared-library build (no editor or ImGui)
if (ANDROID)
    set(OCTARINE_WITH_EDITOR OFF CACHE BOOL "Disabled on Android" FORCE)
    set(OCTARINE_WITH_IMGUI OFF CACHE BOOL "Disabled on Android" FORCE)
    set(OCTARINE_SHIPPED ON CACHE BOOL "Forced ON on Android (manifest loading, no filesystem scan)" FORCE)
endif ()

# Editor requires ImGui
if (OCTARINE_WITH_EDITOR)
    set(OCTARINE_WITH_IMGUI ON CACHE BOOL "Forced ON by Editor" FORCE)
endif ()

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/release")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/bin/relwithdebinfo")

# Default compile-time log level resolution
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

# =============================================================================
# Compiler & Toolchain Configuration
# =============================================================================

# Compiler launcher (ccache / sccache)
if (OCTARINE_USE_CCACHE AND NOT CMAKE_CXX_COMPILER_LAUNCHER)
    find_program(OCTARINE_COMPILER_CACHE NAMES ccache sccache)
    if (OCTARINE_COMPILER_CACHE)
        set(CMAKE_CXX_COMPILER_LAUNCHER "${OCTARINE_COMPILER_CACHE}")
        set(CMAKE_C_COMPILER_LAUNCHER "${OCTARINE_COMPILER_CACHE}")
        message(STATUS "Octarine: compiler cache ENABLED (${OCTARINE_COMPILER_CACHE})")
    endif ()
endif ()

# Architecture flags: default to portable baseline (x86-64-v3 / AVX2 floor) on x86_64
if (OCTARINE_NATIVE_ARCH)
    set(OCTARINE_ARCH_FLAG "-march=native")
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64|amd64)$")
    set(OCTARINE_ARCH_FLAG "-march=x86-64-v3")
else ()
    set(OCTARINE_ARCH_FLAG "")
endif ()

# Link-Time Optimization (LTO / IPO) for non-Debug configurations
include(CheckIPOSupported)
check_ipo_supported(RESULT OCTARINE_IPO_SUPPORTED OUTPUT OCTARINE_IPO_ERROR)
if (OCTARINE_IPO_SUPPORTED)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO ON)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_MINSIZEREL ON)
endif ()

# Sanitizers (GCC/Clang only)
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

# Status summary
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

# =============================================================================
# Dependencies
# =============================================================================

find_package(glm CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
find_package(SDL3_mixer CONFIG REQUIRED)
find_package(Lua REQUIRED)
find_package(sol2 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Threads REQUIRED)

if (OCTARINE_WITH_IMGUI)
    find_package(imgui CONFIG REQUIRED)
    find_package(sol2_ImGui_Bindings CONFIG REQUIRED)
endif ()

# Standard compiler flags and warning configurations
include(cmake/octarine_library.cmake)

# =============================================================================
# Engine Libraries
# =============================================================================

# Builds layered static libraries: octarine_{core,assets,renderer,lua,systems,editor,engine}
add_subdirectory(src)

# =============================================================================
# Primary Artifact: OctarineEngine
# =============================================================================

# On Android, SDLActivity dlopens libmain.so and calls SDL_main. Everywhere else it is an executable.
# Dev player builds rename the executable to OctarineEngine-player to coexist with the editor binary.
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

if (ANDROID)
    target_link_libraries(${PROJECT_NAME} PRIVATE android log)
endif ()

# Target compiler and linker flags
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

# =============================================================================
# Installation & Packaging
# =============================================================================

# Standard installation rules (skipped when packaging a project)
if (NOT OCTARINE_PACKAGE_PROJECT AND NOT ANDROID)
    install(TARGETS ${PROJECT_NAME}
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
    )
endif ()

# Standalone game packaging via CPack
if (OCTARINE_PACKAGE_PROJECT AND NOT ANDROID)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/OctarinePackage.cmake)
    octarine_package(${PROJECT_NAME} PROJECT "${OCTARINE_PACKAGE_PROJECT}" NAME "${PROJECT_NAME}")
endif ()

# =============================================================================
# Code Quality & Formatting
# =============================================================================

if (OCTARINE_ENABLE_LINTING)
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
    if (CLANG_TIDY_EXE)
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
    file(GLOB_RECURSE FORMAT_SOURCES
            "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
    )
    add_custom_target(format
            COMMAND ${CLANG_FORMAT_EXE} -i -style=file ${FORMAT_SOURCES}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Formatting source code with clang-format"
    )
    message(STATUS "Octarine: clang-format target ADDED")
endif ()

# =============================================================================
# Benchmarks
# =============================================================================

if (OCTARINE_ENABLE_BENCHMARKS)
    find_package(benchmark CONFIG REQUIRED)

    add_executable(OctarineBenchmarks
            tests/benchmarks/EcsCoreBenchmark.cpp
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

# =============================================================================
# Tests
# =============================================================================

if (OCTARINE_ENABLE_TESTS)
    enable_testing()

    # Helper for lightweight ECS & system unit tests
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

    # --- Core ECS & Systems Tests ---
    octarine_add_core_test(OctarineEcsTest EcsRegistryTest tests/EcsRegistryTest.cpp)
    octarine_add_core_test(OctarineEcsHierarchyTest EcsHierarchyTest tests/EcsHierarchyTest.cpp)
    octarine_add_core_test(OctarineSystemLogicTest SystemLogicTest tests/SystemLogicTest.cpp)
    octarine_add_core_test(OctarineCollisionResponseTest CollisionResponseTest tests/CollisionResponseTest.cpp)
    octarine_add_core_test(OctarineCollisionSystemTest CollisionSystemTest tests/CollisionSystemTest.cpp)
    octarine_add_core_test(OctarineProjectileEmitSystemTest ProjectileEmitSystemTest tests/ProjectileEmitSystemTest.cpp)

    # EventBus unit test
    add_executable(OctarineEventBusTest
            src/General/Logger.cpp
            tests/EventBusTest.cpp)
    set_target_properties(OctarineEventBusTest PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF)
    target_include_directories(OctarineEventBusTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(OctarineEventBusTest PRIVATE spdlog::spdlog)
    if (MSVC)
        target_compile_options(OctarineEventBusTest PRIVATE /W4 /permissive- /MP /EHsc)
    endif ()
    add_test(NAME EventBusTest COMMAND OctarineEventBusTest)

    # --- Asset Pipeline & Storage Tests ---
    # Asset catalog build and metadata checks
    add_executable(OctarineAssetPipelineTest tests/AssetPipelineTest.cpp)
    set_target_properties(OctarineAssetPipelineTest PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF)
    target_compile_definitions(OctarineAssetPipelineTest PRIVATE
            ASSET_TEST_FIXTURE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/assets")
    target_link_libraries(OctarineAssetPipelineTest PRIVATE octarine_engine)
    if (MSVC)
        target_compile_options(OctarineAssetPipelineTest PRIVATE /MP /bigobj /permissive- /wd4201 /wd5321)
    endif ()
    add_test(NAME AssetPipelineTest COMMAND OctarineAssetPipelineTest)

    # AssetRefcounter unit test
    add_executable(OctarineAssetRefcounterTest tests/AssetRefcounterTest.cpp)
    set_target_properties(OctarineAssetRefcounterTest PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF)
    target_include_directories(OctarineAssetRefcounterTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    if (MSVC)
        target_compile_options(OctarineAssetRefcounterTest PRIVATE /W4 /permissive- /MP /EHsc)
    endif ()
    add_test(NAME AssetRefcounterTest COMMAND OctarineAssetRefcounterTest)

    # Asset store and texture lifecycle test
    add_executable(OctarineAssetStoreTest tests/AssetStoreTest.cpp)
    set_target_properties(OctarineAssetStoreTest PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF)
    target_link_libraries(OctarineAssetStoreTest PRIVATE octarine_engine)
    if (MSVC)
        target_compile_options(OctarineAssetStoreTest PRIVATE /MP /bigobj /permissive- /wd4201 /wd5321)
    endif ()
    add_test(NAME AssetStoreTest COMMAND OctarineAssetStoreTest)

    # --- Lua & Scripting Tests ---
    # API smoke test (validates EmmyLua stubs and JSON indexes against tracked copies)
    add_executable(OctarineLuaApiTest tests/LuaApiSmokeTest.cpp)
    set_target_properties(OctarineLuaApiTest PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF)
    target_compile_definitions(OctarineLuaApiTest PRIVATE
            LUA_API_SMOKE_OUTPUT="${CMAKE_CURRENT_SOURCE_DIR}/lua_api.smoke.lua"
            LUA_API_COMPONENTS_OUTPUT="${CMAKE_CURRENT_SOURCE_DIR}/components.json"
            LUA_API_MODULES_OUTPUT="${CMAKE_CURRENT_SOURCE_DIR}/modules.json")
    target_link_libraries(OctarineLuaApiTest PRIVATE octarine_engine)
    if (MSVC)
        target_compile_options(OctarineLuaApiTest PRIVATE /MP /bigobj /permissive- /wd4201 /wd5321)
    endif ()
    add_test(NAME LuaApiSmokeTest COMMAND OctarineLuaApiTest)

    # Lua component mutation and behavioral test
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

    # Sandboxed Lua storage and file write test
    add_executable(OctarineFileSandboxTest tests/FileSandboxTest.cpp)
    set_target_properties(OctarineFileSandboxTest PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF)
    target_link_libraries(OctarineFileSandboxTest PRIVATE octarine_lua)
    if (MSVC)
        target_compile_options(OctarineFileSandboxTest PRIVATE /MP /bigobj /permissive- /wd4201 /wd5321)
    endif ()
    add_test(NAME FileSandboxTest COMMAND OctarineFileSandboxTest)

    # --- Subsystems & Tooling Tests ---
    # Process execution wrapper test
    add_executable(OctarineProcessTest
            src/Process/Process.cpp
            tests/ProcessTest.cpp)
    set_target_properties(OctarineProcessTest PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF)
    target_include_directories(OctarineProcessTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(OctarineProcessTest PRIVATE Threads::Threads)
    if (MSVC)
        target_compile_options(OctarineProcessTest PRIVATE /W4 /permissive- /MP /EHsc)
    endif ()
    add_test(NAME ProcessTest COMMAND OctarineProcessTest)

    # project.ini parser and identity validation test
    add_executable(OctarineProjectIniTest
            src/Project/ProjectIni.cpp
            tests/ProjectIniTest.cpp)
    set_target_properties(OctarineProjectIniTest PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF)
    target_include_directories(OctarineProjectIniTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    if (MSVC)
        target_compile_options(OctarineProjectIniTest PRIVATE /W4 /permissive- /MP /EHsc)
    endif ()
    add_test(NAME ProjectIniTest COMMAND OctarineProjectIniTest)

    # File watcher test
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

    # ADB parser test
    add_executable(OctarineAdbTest
            src/Deploy/Adb.cpp
            src/Process/Process.cpp
            src/General/Logger.cpp
            tests/AdbTest.cpp)
    set_target_properties(OctarineAdbTest PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF)
    target_include_directories(OctarineAdbTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(OctarineAdbTest PRIVATE spdlog::spdlog Threads::Threads)
    if (MSVC)
        target_compile_options(OctarineAdbTest PRIVATE /W4 /permissive- /MP /EHsc)
    endif ()
    add_test(NAME AdbTest COMMAND OctarineAdbTest)

    # Headless Game::Bake validation test
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

    # RenderQueue sorting and batching invariants test
    add_executable(OctarineRenderQueueTest tests/RenderQueueTest.cpp)
    set_target_properties(OctarineRenderQueueTest PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF)
    target_link_libraries(OctarineRenderQueueTest PRIVATE octarine_engine)
    if (MSVC)
        target_compile_options(OctarineRenderQueueTest PRIVATE /MP /bigobj /permissive- /wd4201 /wd5321)
    endif ()
    add_test(NAME RenderQueueTest COMMAND OctarineRenderQueueTest)

    # Dev listen server test (non-shipping builds only)
    if (NOT OCTARINE_SHIPPED)
        add_executable(OctarineDevListenServerTest
                src/Dev/DevListenServer.cpp
                src/General/Logger.cpp
                tests/DevListenServerTest.cpp)
        set_target_properties(OctarineDevListenServerTest PROPERTIES
                CXX_STANDARD 20
                CXX_STANDARD_REQUIRED ON
                CXX_EXTENSIONS OFF)
        target_include_directories(OctarineDevListenServerTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
        target_link_libraries(OctarineDevListenServerTest PRIVATE spdlog::spdlog Threads::Threads)
        if (WIN32)
            target_link_libraries(OctarineDevListenServerTest PRIVATE ws2_32)
        endif ()
        if (MSVC)
            target_compile_options(OctarineDevListenServerTest PRIVATE /W4 /permissive- /MP /EHsc)
        endif ()
        add_test(NAME DevListenServerTest COMMAND OctarineDevListenServerTest)
    endif ()

    message(STATUS "Octarine: Tests ENABLED")
endif ()
