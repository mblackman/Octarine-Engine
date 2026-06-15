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
| `player-profile`  | RelWithDebInfo + `OCTARINE_ENABLE_PROFILING` for timing |
| `ship-release`    | Canonical shipping config (manifest-load catalog) |

### Linux build prerequisites

vcpkg builds SDL3 (and its `alsa` dependency) **from source** on first configure, so the host needs the windowing/audio dev headers and autotools **before** that build runs. If they're absent, the SDL3 build doesn't fail loudly — it silently disables the missing backends, and you only find out at runtime when the engine exits with `SDL_Init Error: No available video device` (or `No available audio device`). The vcpkg binary cache then keeps that broken build around, so installing the packages later isn't enough on its own — see the note below.

On Debian/Ubuntu/Pop!\_OS:

```bash
sudo apt install cmake ninja-build \
  autoconf autoconf-archive automake libtool \
  wayland-protocols libwayland-bin libxkbcommon-dev \
  libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxi-dev \
  libxfixes-dev libxtst-dev libxss-dev libxrender-dev \
  libegl1-mesa-dev libgles2-mesa-dev \
  libasound2-dev libpulse-dev libdbus-1-dev libdrm-dev libgbm-dev
```

If you already configured once with packages missing, the broken SDL3 lives in vcpkg's binary cache, and `apt install` alone won't trigger a rebuild (the cache key ignores system libraries). Force a clean from-source rebuild:

```bash
rm -rf build/<preset>/vcpkg_installed
VCPKG_BINARY_SOURCES=clear cmake --preset <preset>
```

To confirm the backends actually compiled in, the built binary should contain `wayland`/`x11` (video) and `alsa` (audio):

```bash
for s in wayland x11 alsa; do strings build/<preset>/bin/debug/OctarineEngine* | grep -qx "$s" && echo "present: $s"; done
```

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
| A **Lua module** (free-function globals) | `src/Lua/Modules/XModuleLuaBinding.{h,cpp}` + an `install` line in `src/Lua/Modules/RegisterAllModules.cpp` + add the `.cpp` to the `octarine_lua` list in `src/CMakeLists.txt`. |
| A **system** | `src/Systems/X.h` + a registration call in `Game::Setup` (`RegisterSystem` / `RegisterParallelSystem` / `RegisterBulkSystem`). Optional `LuaSystemBinding<X>` if it exposes a Lua surface. Mind registration order — see [`docs/systems.md`](systems.md). |
| An **event** | `src/Events/X.h` + `EmitEvent<X>(...)` in producers + `SubscribeEvent<X>(...)` in each consumer's `Init` — see [`docs/events.md`](events.md). |
| A **new `.cpp`** under `src/` | Add it to the matching per-layer source list in `src/CMakeLists.txt` (`octarine_core`/`_assets`/`_renderer`/`_lua`/`_systems`/`_editor`/`_engine`). |

## Deeper reading

- [`docs/ecs-architecture.md`](ecs-architecture.md) — archetype graph, chunk storage, query model
- [`docs/ecs-components.md`](ecs-components.md) — every component's Lua shape
- [`docs/lua-scripting.md`](lua-scripting.md) — how Lua scripts wire into the engine
- [`docs/systems.md`](systems.md) — built-in systems and the order they run in
- [`docs/events.md`](events.md) — the event bus and every event type
- [`docs/editor.md`](editor.md) — editor windows, hotkeys, Run Player, Export Build
- [`docs/scenes.md`](scenes.md) — scene file shape, lifecycle, `load_scene` / `reload_scene`
- [`docs/tilemaps.md`](tilemaps.md) — what the scene `tilemap` field does (and doesn't) today
- [`docs/asset-pipeline.md`](asset-pipeline.md) — `.meta` sidecars, bake step, atlases, audio normalize
- [`docs/profiling.md`](profiling.md) — profiling build, PerfUtils, benchmarks, the perf dashboard
- [`docs/device-builds.md`](device-builds.md) — shipping artifacts for desktop and Android
- `lua_api.smoke.lua` — generated, exhaustive reference for the live Lua surface
