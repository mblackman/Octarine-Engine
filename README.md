# Octarine Engine

![Octarine Engine](https://img.shields.io/badge/Aesthetic-Yellowish--Purple-9370DB?style=for-the-badge)
![C++20](https://img.shields.io/badge/C++-20-blue.svg?style=for-the-badge&logo=c%2B%2B)
![Lua](https://img.shields.io/badge/Lua-5.4-blue.svg?style=for-the-badge&logo=lua)

Octarine Engine is a lightweight, high-performance 2D game engine built with C++20. It features a custom archetype-based Entity-Component System (ECS), integrated Lua scripting, and a unique yellowish-purple aesthetic.

## Tech Stack

- **Graphics:** SDL3
- **Scripting:** Lua 5.4 with [sol2](https://github.com/ThePhD/sol2) bindings
- **ECS:** Custom Archetype-based implementation
- **Math:** [GLM](https://github.com/g-truc/glm)
- **UI:** [ImGui](https://github.com/ocornut/imgui) with custom sol2 bindings
- **Logging:** [spdlog](https://github.com/gabime/spdlog)
- **Dependency Management:** [vcpkg](https://github.com/microsoft/vcpkg)

---

## Prerequisites

- **CMake:** Version 3.15+
- **C++ Compiler:** C++20 compliant (GCC 11+, Clang 13+, MSVC 2022+)
- **vcpkg:** For managing dependencies.
- **Git:** To clone the repo and submodules.

---

## Building the Engine

```bash
# Clone the repository
git clone https://github.com/your-repo/Octarine-Engine.git
cd Octarine-Engine

# Configure with vcpkg (replace [vcpkg-path] with your actual vcpkg root)
cmake -B build -DCMAKE_TOOLCHAIN_FILE=[vcpkg-path]/scripts/buildsystems/vcpkg.cmake

# Build the project
cmake --build build
```

The compiled binaries will be located in `build/bin/debug/` or `build/bin/release/`.

---

## Scripting with Lua

Octarine Engine is built to be driven by Lua scripts, allowing you to iterate quickly on game logic without recompiling the core engine.

For detailed instructions on project structure and the complete Lua API, see:
👉 **[Lua Scripting Guide](docs/lua-scripting.md)**

---

## Architecture (ECS)

The engine uses an **Archetype-based ECS**. Entities with the same set of components are stored together in memory, maximizing CPU cache efficiency.

For a deep dive into the design and implementation, see:
👉 **[ECS Architectural Overview](docs/ecs-architecture.md)**

- **Registry:** Manages all entities and components.
- **Archetypes:** Groups of entities with identical component signatures.
- **Queries:** Used by systems to iterate over entities matching specific component requirements.
- **Systems:** Contain the logic that processes entities (e.g., `MovementSystem`, `CollisionSystem`).

---

## License

This project is licensed under the MIT License - see the `LICENSE` file for details.
