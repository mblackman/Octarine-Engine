# Host-only icon resize + .ico bundle helper. Replaces the ImageMagick PATH dep that
# octarine-icons.cmake used to rely on. Built from vendored stb single-headers — no
# external dependencies, no install, no vcpkg.
#
# octarine-icons.cmake configures and builds this tree on demand via cmake -P; the
# exe lives in the build dir and is invoked directly. Nothing here ever ships in the
# packaged engine artifacts.

cmake_minimum_required(VERSION 3.20)
project(octarine_icon_tool CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default to Release when no config was passed (the single-config generator path —
# Ninja/Make under cmake -P inheritance). Multi-config generators (MSVC) ignore this
# and pick up --config Release from the caller.
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif ()

add_executable(octarine-icon-tool main.cpp)

target_include_directories(octarine-icon-tool PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/../../libs/stb")

# Centralize output regardless of generator so octarine-icons.cmake can find the exe
# at a stable path. MSVC's per-config subdir is overridden by setting the *_RELEASE
# variant explicitly.
set_target_properties(octarine-icon-tool PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}/bin")

if (MSVC)
    target_compile_options(octarine-icon-tool PRIVATE /W3 /utf-8)
    target_compile_definitions(octarine-icon-tool PRIVATE _CRT_SECURE_NO_WARNINGS)
else ()
    target_compile_options(octarine-icon-tool PRIVATE -Wall -Wextra -Wno-unused-function)
endif ()
