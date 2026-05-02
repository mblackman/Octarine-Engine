# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name
project(OctarineEngine LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set export compile commands for clang-tidy
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Gather source files
file(GLOB_RECURSE SRC_FILES
        "src/Main.cpp"
        "src/Game/*.cpp"
        "src/General/*.cpp"
        "src/ECS/*.cpp"
        "src/AssetManager/*.cpp"
        "src/Renderer/*.cpp"
)

# Add executable target
add_executable(OctarineEngine ${SRC_FILES})

# Include directories
target_include_directories(OctarineEngine PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/libs"
)

# Compiler flags
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(OctarineEngine PRIVATE -Wall -Wfatal-errors)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(OctarineEngine PRIVATE /W4 /WX)
endif()

# Using find_package
find_package(glm CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
find_package(Lua REQUIRED)
find_package(Sol2 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

target_link_libraries(OctarineEngine PRIVATE
        glm::glm
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
        $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,SDL3_image::SDL3_image-shared,SDL3_image::SDL3_image-static>
        ${LUA_LIBRARIES}
        imgui::imgui
        spdlog::spdlog
)

target_include_directories(OctarineEngine PRIVATE ${LUA_INCLUDE_DIR})