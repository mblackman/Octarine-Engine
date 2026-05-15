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

The engine uses **CMake Presets** to manage different build configurations. There are three primary variants:

- **Editor:** Includes ImGui and the yellowish-purple editor tools. Best for development and level design.
- **Player:** A minimal, high-performance runtime without the editor.
- **Profile:** An optimized player with performance instrumentation (`spdlog` timers) enabled.

### Build Commands

```bash
# Clone the repository
git clone https://github.com/mblackman/Octarine-Engine.git
cd Octarine-Engine

# List all available presets
cmake --list-presets

# Build the Editor (Standard development)
cmake --preset editor-debug
cmake --build --preset editor-debug

# Build the Player (Shipping build)
cmake --preset player-release
cmake --build --preset player-release

# Build the Profiling Player (For performance analysis)
cmake --preset player-profile
cmake --build --preset player-profile
```

The compiled binaries will be located in `cmake-build-[preset-name]/bin/`.

### Running the Engine

Octarine Engine accepts the game directory as a positional argument:

```bash
# Run a game project
./OctarineEngine ./projects/my-game

# Run with a specific startup mode (e.g. for benchmarking)
./OctarineEngine ./projects/my-game --startup-mode stress
```

---

## Benchmarking

### Continuous Benchmarking

The engine runs automated performance benchmarks via GitHub Actions on every push to `main` and Pull Request.

You can view the interactive historical performance dashboard here:
👉 **[Octarine Engine Performance Dashboard](https://mblackman.github.io/Octarine-Engine/dev/bench/)**

### Local Profiling

The `player-profile` preset is optimized for performance analysis. `scripts/bench.sh` uses this build to stream timing data:

```bash
# Run for 8s using player-profile preset against Octarine-Engine-Example.
scripts/bench.sh

# Run against a specific game directory
OCT_BENCH_GAME=/path/to/game scripts/bench.sh
```

For ad-hoc profiling runs:

```bash
SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy \
  ./cmake-build-player-profile/bin/relwithdebinfo/OctarineEngine /path/to/game --startup-mode bench
```
