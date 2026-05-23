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
        src/Renderer/Renderer.cpp
        src/Systems/AudioSystem.cpp
)

if (OCTARINE_WITH_IMGUI)
    list(APPEND SRC_FILES src/Systems/RenderDebugGUISystem.cpp)
endif ()

if (OCTARINE_WITH_EDITOR)
    list(APPEND SRC_FILES src/Editor/EditorPersistence.cpp)
    list(APPEND SRC_FILES src/Editor/EditorLayoutPresets.cpp)
endif ()

if (UNIX AND NOT APPLE)
    list(APPEND SRC_FILES src/General/SDL3_Linux_Stubs.cpp)
endif ()

# Add packaged libs
if (OCTARINE_WITH_IMGUI)
    add_subdirectory(libs/sol2_ImGui_Bindings)
endif ()

# Create the executable target
add_executable(${PROJECT_NAME} ${SRC_FILES})

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

# Include directories
target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/libs
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

# Install rules (optional)
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)

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
    target_compile_options(${PROJECT_NAME} PRIVATE
            $<IF:$<CONFIG:Debug>,,-Werror -march=native -ffast-math>
    )
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

    message(STATUS "Octarine: Tests ENABLED")
endif ()
