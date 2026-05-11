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

## Benchmarking

### Continuous Benchmarking

The engine runs automated performance benchmarks via GitHub Actions on every push to `main` and Pull Request.

You can view the interactive historical performance dashboard here:
👉 **[Octarine Engine Performance Dashboard](https://mblackman.github.io/Octarine-Engine/dev/bench/)**

### Local Profiling

The engine ships with two CMake presets that enable `OCTARINE_PROFILING` and emit `TIMER:` lines via `spdlog`:

- `debug-profile` — debug build with profiling
- `release-profile` — optimized build with profiling (use this for perf numbers)

`scripts/bench.sh` runs the engine headlessly against a game directory for a fixed duration and streams the timer output:

```bash
# By default, runs for 8s using release-profile preset.
# Automatically clones Octarine-Engine-Example to .benchmark_game/ if no game is found!
scripts/bench.sh

# Override preset and duration
scripts/bench.sh release-profile 15

# Point at a different game
OCT_BENCH_GAME=/path/to/game scripts/bench.sh

# Aggregate and format stats into JSON (requires Python 3)
scripts/bench.sh | python3 scripts/parse_bench_output.py
```

**Developer Requirements for Benchmarking:**
- **Python 3:** Required if you want to use `scripts/parse_bench_output.py` to aggregate raw timers into P95/P99/Max JSON metrics.
- **Windows Users:** `scripts/bench.sh` is a Bash script utilizing the Unix `timeout` command. Run it within **WSL (Windows Subsystem for Linux)** or **Git Bash**.

The script handles the headless SDL env (`SDL_VIDEODRIVER=dummy`, `SDL_AUDIODRIVER=dummy`), passes `--startup-mode` to suppress the debug UI overlay, and builds the preset on first run.

### Available timers

Frame-level (one sample per frame):

- `Game::Update (total)` — full update phase
- `Game::ProcessInput` — SDL event poll loop
- `Game::WaitTime` — frame-cap idle (high = headroom, ~0 = behind budget)
- `Game::Render (total)` — full render phase
- `Render: Sort` — render queue radix sort
- `Render: Draw` — `Renderer::Render` SDL draw calls
- `Render: Present` — `SDL_RenderPresent` GPU sync
- `Registry::Update (total)` — system loop
- `Collision System Update`, `Gather Boxes`, `Emit Events` — collision phase breakdown

Sub-frame (multiple samples per frame; aggregated as a distribution):

- `CommandBuffer::Playback` — once per parallel system per frame

Accumulated within the async collision job (reported as `ACCUM:` lines):

- `Brute Force Intersection`, `Partition Boxes`, `Brute Force Bipartite`, `Sweep Bipartite`

### Adding new timers

Wrap the scope you want to measure with one of the macros from `src/General/PerfUtils.h`:

```cpp
#include "General/PerfUtils.h"

void MySystem::Update() {
  PROFILE_NAMED_SCOPE("MySystem::Update");
  // ...
}
```

All macros compile to `((void)0)` when `OCTARINE_PROFILING` is off, so non-profile builds pay zero cost.

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
