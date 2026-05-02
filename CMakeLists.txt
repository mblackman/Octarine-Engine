# Minimum CMake version required
cmake_minimum_required(VERSION 3.15)

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

# Set output directories for all build types
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/release")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/bin/relwithdebinfo")

# Source files
set(SRC_FILES
        src/Main.cpp
        src/AssetManager/AssetManager.cpp
        src/ECS/Registry.cpp
        src/Game/Game.cpp
        src/Game/GameConfig.cpp
        src/General/Logger.cpp
        src/Renderer/Renderer.cpp
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

# Dependencies
find_package(glm CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
find_package(Lua REQUIRED)
find_package(Sol2 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

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
        $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,SDL3_image::SDL3_image-shared,SDL3_image::SDL3_image-static>
        ${LUA_LIBRARIES}
        imgui::imgui
        spdlog::spdlog
        sol2_ImGui_Bindings
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
    )
    target_compile_options(${PROJECT_NAME} PUBLIC
            $<IF:$<CONFIG:Debug>,,-WX>
    )
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wfatal-errors
    )
    target_compile_options(${PROJECT_NAME} PUBLIC
            $<IF:$<CONFIG:Debug>,,-Werror>
    )
endif ()