# Octarine-Engine #

Octarine Engine colors imagination as a lightweight 2D game engine that is present where magic happens. It has its own ECS, supports Lua scripting, and is a yellowish-purple color.

This engine uses:

-   GLM: <https://github.com/g-truc/glm>
-   ImGui: <https://github.com/ocornut/imgui>
-   Lua: <https://www.lua.org/>
-   SDL3: <https://www.libsdl.org/>
-   spdlog: <https://github.com/gabime/spdlog>
-   sol2: <https://github.com/ThePhD/sol2>

All dependencies are managed via [vcpkg](https://github.com/microsoft/vcpkg).

## Prerequisites

Before you begin, ensure you have the following installed:

1.  **Git:** For cloning the project and vcpkg.
2.  **CMake:** Version 3.15 or higher. ([Download CMake](https://cmake.org/download/))
3.  **C++ Compiler:** A C++17 compliant compiler (e.g., GCC, Clang, MSVC).
    * On Ubuntu, you can install `build-essential` which includes GCC: `sudo apt update && sudo apt install build-essential g++`
4.  **CLion IDE:** (Recommended for development) [JetBrains CLion](https://www.jetbrains.com/clion/).
