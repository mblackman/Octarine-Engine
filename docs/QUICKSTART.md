# Quickstart

The shortest path from a fresh clone to a code change you can verify. For deeper material, jump to one of the topic docs listed at the bottom.

## 1. Build

```bash
# Editor build (ImGui + yellowish-purple editor tools, debug).
cmake --preset editor-debug
cmake --build build/editor-debug
```

CMake presets live in `CMakePresets.json`. The common ones:

| Preset            | When to use                                       |
|-------------------|---------------------------------------------------|
| `editor-debug`    | Day-to-day dev with editor + ImGui debug overlays |
| `editor-release`  | Optimized editor build                            |
| `player-release`  | Optimized runtime, no editor (live-scan catalog)  |
| `player-profile`  | RelWithDebInfo + `OCTARINE_PROFILING` for timing  |
| `ship-release`    | Canonical shipping config (manifest-load catalog) |

## 2. Run

The engine takes the game project directory as a positional argument:

```bash
./build/editor-debug/bin/debug/OctarineEngine ../Octarine-Engine-Example
```

[`Octarine-Engine-Example`](https://github.com/mblackman/Octarine-Engine-Example) is the reference project — a small but complete Lua-driven game that exercises every binding the engine ships.

Bake-only (headless, no window): runs the asset pipeline and writes `asset_manifest.lua` next to the project. Nonzero exit on any unresolved asset reference.

```bash
./build/editor-debug/bin/debug/OctarineEngine ../Octarine-Engine-Example -m bake
```

## 3. Verify

Two engine tests are gated behind `OCTARINE_ENABLE_TESTS=ON`:

```bash
cmake --preset editor-debug -DOCTARINE_ENABLE_TESTS=ON
cmake --build build/editor-debug
ctest --test-dir build/editor-debug --output-on-failure
```

- **`LuaApiSmokeTest`** — constructs `Game` headlessly, replays `Game::Setup`'s Lua-binding sequence, and asserts every registered component/module/system surface is reachable from Lua. Also re-emits the EmmyLua stub at `lua_api.smoke.lua` (CI fails on drift; commit the regenerated file).
- **`AssetPipelineTest`** — exercises `.meta` sidecar parsing, catalog build, manifest round-trip, and validation against a tiny fixture asset tree.

CI (`.github/workflows/build.yml`) runs both on the Linux editor-release leg on every push to `main` and PR.

## 4. Where things live

| Path | What's there |
|------|--------------|
| `src/ECS/` | Archetype registry, chunk SoA storage, queries |
| `src/Game/Game.cpp` | Main loop and `Setup` (binding/registration ordering matters) |
| `src/Components/` | Plain component structs (data only) |
| `src/Systems/` | Engine systems (input, collision, rendering, audio, ...) |
| `src/Lua/Bindings/` | `LuaBinding<T>` per component, `LuaSystemBinding<S>` per system surface |
| `src/Lua/Modules/` | `LuaModuleBinding<M>` — Lua free-function globals (one file per domain) |
| `src/Renderer/` | SDL3 render path + render queue |
| `src/EventBus/`, `src/Events/` | Type-erased pub/sub |
| `src/AssetManager/` | Catalog, `.meta` sidecar parsing, bake/manifest |
| `tests/` | `LuaApiSmokeTest.cpp`, `AssetPipelineTest.cpp` (off by default) |
| `cmake/`, `scripts/` | Packaging, license aggregation, icon generation |
| `android/` | Gradle host app (multi-ABI, vcpkg android triplets) |
| `lua_api.smoke.lua` | Auto-generated EmmyLua stub of the whole Lua surface |

## 5. Adding new code

| To add… | Touch points |
|---------|--------------|
| A **component** | `src/Components/X.h` + `src/Lua/Bindings/XLuaBinding.h` + a `registerComponent<X>()` line in `src/Lua/Bindings/RegisterAllBindings.cpp`. Header-only — no CMake edit. |
| A **Lua module** (free-function globals) | `src/Lua/Modules/XModuleLuaBinding.{h,cpp}` + an `install` line in `src/Lua/Modules/RegisterAllModules.cpp` + append the `.cpp` to `SRC_FILES` in `CMakeLists.txt`. |
| A **system** | `src/Systems/X.h` + a registration call in `Game::Setup` (`RegisterSystem` / `RegisterParallelSystem` / `RegisterBulkSystem`). Optional `LuaSystemBinding<X>` if it exposes a Lua surface. Mind `Game::Setup` ordering — see [`docs/lua-scripting.md`](lua-scripting.md). |
| An **event** | `src/Events/X.h` + `EmitEvent<X>(...)` in producers + `SubscribeEvent<X>(...)` in each consumer's `Init`. |
| A **new `.cpp`** under `src/` | Append to `SRC_FILES` in `CMakeLists.txt`. |

## Deeper reading

- [`docs/ecs-architecture.md`](ecs-architecture.md) — archetype graph, chunk storage, query model
- [`docs/ecs-components.md`](ecs-components.md) — every component's Lua shape
- [`docs/lua-scripting.md`](lua-scripting.md) — how Lua scripts wire into the engine
- [`docs/device-builds.md`](device-builds.md) — shipping artifacts for desktop and Android
- `lua_api.smoke.lua` — generated, exhaustive reference for the live Lua surface
