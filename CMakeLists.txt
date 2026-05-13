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

# Profiling option — when ON, enables TIMER/ACCUM logging in PerfUtils.h.
# OFF by default so Debug and Release builds have zero profiling overhead.
option(OCTARINE_ENABLE_PROFILING "Enable per-system performance instrumentation" OFF)

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
find_package(imgui CONFIG REQUIRED)
find_package(Threads REQUIRED)

# Source files
set(SRC_FILES
        src/Main.cpp
        src/AssetManager/AssetManager.cpp
        src/ECS/Registry.cpp
        src/Game/Game.cpp
        src/Game/GameConfig.cpp
        src/General/Logger.cpp
        src/Renderer/Renderer.cpp
        src/Systems/AudioSystem.cpp
)

if (UNIX AND NOT APPLE)
    list(APPEND SRC_FILES src/General/SDL3_Linux_Stubs.cpp)
endif ()

# Add packaged libs
add_subdirectory(libs/sol2_ImGui_Bindings)

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
        imgui::imgui
        spdlog::spdlog
        sol2::sol2
        sol2_ImGui_Bindings
        Threads::Threads
)

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
            spdlog::spdlog
            Threads::Threads
    )

    if (MSVC)
        target_compile_options(OctarineBenchmarks PRIVATE /W4 /permissive- /MP /bigobj)
    else ()
        target_compile_options(OctarineBenchmarks PRIVATE -Wall -Wextra -Wpedantic)
    endif ()

    message(STATUS "Octarine: Benchmarks ENABLED")
endif ()