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
find_package(Sol2 CONFIG REQUIRED)
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
        src/General/SDL3_Linux_Stubs.cpp
        src/Renderer/Renderer.cpp
        src/Systems/AudioSystem.cpp
)

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
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/sol2_ImGui_Bindings
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
    target_compile_options(${PROJECT_NAME} PRIVATE
            /W3
            /permissive-
            /MP
            /bigobj
    )
    target_compile_options(${PROJECT_NAME} PRIVATE
            $<IF:$<CONFIG:Debug>,,-WX>
    )
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wfatal-errors
    )
    target_compile_options(${PROJECT_NAME} PRIVATE
            $<IF:$<CONFIG:Debug>,,-Werror>
    )
endif ()