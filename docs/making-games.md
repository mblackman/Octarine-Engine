# Making Games with Octarine Engine

A practical guide for game developers. It covers how to set up a project, the
core concepts you'll use every day, and where to go for deeper reference.

The companion example project,
[Octarine-Engine-Example](https://github.com/mblackman/Octarine-Engine-Example),
is a small but complete game that exercises every feature described here — run
it alongside this guide whenever an example would help.

---

## Table of Contents

1. [Prerequisites](#1-prerequisites)
2. [Project Structure](#2-project-structure)
3. [Configuration](#3-configuration)
4. [Core Concepts](#4-core-concepts)
5. [Your First Entity](#5-your-first-entity)
6. [Collision and Damage](#6-collision-and-damage)
7. [Scripting Entities](#7-scripting-entities)
8. [Scenes](#8-scenes)
9. [Assets](#9-assets)
10. [Audio](#10-audio)
11. [Camera](#11-camera)
12. [Using the Editor](#12-using-the-editor)
13. [Game CI](#13-game-ci)
14. [Further Reading](#14-further-reading)

---

## 1. Prerequisites

Before writing game code you need a built copy of the engine.

### Option A — Download a versioned release (recommended)

Pre-built engine binaries are published as assets on every
[GitHub Release](https://github.com/mblackman/Octarine-Engine/releases).

1. Open the release matching your target engine version (e.g. `v0.1.0`).
2. Download the SDK binary for your platform:

| Asset | Platform |
|---|---|
| `OctarineEngine-{version}-linux.tar.gz` | Linux x64 |
| `OctarineEngine-{version}-windows.zip` | Windows x64 |
| `OctarineEngine-{version}-macos.tar.gz` | macOS (arm64) |

3. Extract the archive and run the binary directly — no install step required:

```bash
# Linux / macOS
tar -xzf OctarineEngine-0.1.0-linux.tar.gz
./OctarineEngine path/to/MyGame

# Windows (PowerShell)
Expand-Archive OctarineEngine-0.1.0-windows.zip
.\OctarineEngine.exe path/to/MyGame
```

### Option B — Build from source

Requires CMake 3.15+, a C++20-capable compiler (MSVC 2022, GCC 12+, or Clang 14+),
and vcpkg (the engine uses manifest mode and manages its own dependencies).

```bash
git clone https://github.com/mblackman/Octarine-Engine
cd Octarine-Engine
cmake --preset editor-debug
cmake --build build/editor-debug
```

The resulting binary is `build/editor-debug/bin/debug/OctarineEngine` (Linux/macOS)
or `build\editor-debug\bin\debug\OctarineEngine.exe` (Windows).

**Run against your project**

```bash
./build/editor-debug/bin/debug/OctarineEngine path/to/MyGame
```

The engine takes your game's root directory as its only positional argument. The
`config.ini` there tells it what startup script to run.

---

## 2. Project Structure

A minimal project needs only `config.ini` and `game.lua`. A more typical layout:

```
MyGame/
├── config.ini              # window settings, startup script
├── game.lua                # entry point — runs first
├── scenes/
│   ├── title.lua
│   ├── gameplay.lua
│   └── game_over.lua
├── scripts/
│   └── lib/                # reusable Lua helpers
│       ├── input_map.lua
│       └── player_controller.lua
├── images/                 # .png / .jpg textures
├── fonts/                  # .ttf fonts
├── sounds/                 # .wav / .ogg audio
└── tilemaps/               # .map files + tileset PNGs
```

There are no enforced path conventions beyond `config.ini` and the startup
script — organise the rest however suits your project.

---

## 3. Configuration

### `config.ini` — runtime window settings

`config.ini` lives at the project root and controls window setup:

```ini
Title=My Awesome Game
Version=1.0.0
Author=Your Name
StartupScript=game.lua
DefaultWindowWidth=1280
DefaultWindowHeight=720
DefaultScalingMode=nearest      # 'nearest' for pixel art, 'linear' for smooth
FpsTarget=60                    # frame-rate cap; 0 = uncapped (default 60)
```

`StartupScript` is the first Lua file that runs. Everything else — scenes,
assets, entities — is loaded from there.

### `project.ini` — packaging and release identity

`project.ini` is the single source of truth for your game's release identity.
It is read by the engine's CMake packaging helper, Android Gradle build, and
the engine version check. The format is flat `key = value` (no sections).

```ini
name          = My Awesome Game
package_id    = com.studio.myawesomegame    # reverse-DNS, required for shipping
version_name  = 1.0.0                       # semver displayed in stores / About
version_code  = 1                           # monotonic integer for store updates
vendor        = My Studio
description   = A short description for store listings.
icon          = images/icon.png             # source PNG for icon generation
engine_version = 0.1.0                     # engine version this project targets
```

| Key | Required for shipping | Notes |
|---|---|---|
| `name` | Yes | Display name |
| `package_id` | Yes | Reverse-DNS (e.g. `com.studio.mygame`) |
| `version_name` | Yes | SemVer string |
| `version_code` | No | Monotonic int; defaults to 1 |
| `vendor` | No | Studio / publisher name |
| `description` | No | One-line store summary |
| `icon` | No | Source image for icon generation |
| `engine_version` | No | Declared engine version; checked at package time |

The `engine_version` field is used by the `setup-engine` action in game CI (see
[Game CI](#game-ci)) and checked at CMake configure time — a mismatch emits a
warning so you know when a game project is being built against a newer or older
engine than intended.

---

## 4. Core Concepts

### Entities and Components

Octarine uses an **Entity-Component System (ECS)**. The key ideas:

- An **entity** is just an ID — a number with no data of its own.
- **Components** are plain data structs attached to entities (`position`,
  `sprite`, `rigidbody`, etc.).
- **Systems** are the engine's update loops — they query for entities with a
  given set of components and act on them each frame.

You never subclass anything or register callbacks on a base class. Instead you
compose entities from components and attach a `script` component when an entity
needs custom per-frame behaviour.

### Lua is the game layer

All game logic lives in Lua. The engine exposes its entire surface through:

- **Free globals** — `load_entity`, `load_scene`, `play_sound`, `get_position`,
  `set_position`, `fire_projectile`, `log`, etc.
- **Namespace tables** — `input.*`, `ImGui.*`, `registry.*`
- **The `script` component** — attach `on_update` and `on_debug_gui` callbacks
  to any entity.

You do not need to touch C++ to make a game.

### Scenes

A scene is a Lua file. `load_scene("scenes/gameplay.lua")` replaces the current
scene with a new one. Scenes can be declarative (return a table the engine
processes) or procedural (build the scene themselves). See [Scenes](#7-scenes).

---

## 5. Your First Entity

Pass a table to `load_entity` from anywhere in your Lua code:

```lua
load_entity({
    tag = "player",         -- optional group label
    name = "Player",        -- optional display name
    components = {
        transform = {
            position = { x = 100, y = 100 },
            scale    = { x = 1.0, y = 1.0 },
            rotation = 0.0,
        },
        sprite = {
            texture_asset_id = "player-texture",
            width  = 32,
            height = 32,
            layer  = 2,
        },
        rigidbody = {
            velocity = { x = 0, y = 0 },
        },
        health = {
            max_health     = 100,
            current_health = 100,
        },
    }
})
```

For the full list of available components and their fields see
[`docs/ecs-components.md`](ecs-components.md).

---

## 6. Collision and Damage

Add `box_collider` to any entity that should participate in collision detection.
The engine resolves overlaps every frame and dispatches damage automatically —
no script code required for the common case.

### How damage works

When a projectile hits an entity, the engine compares collision masks to decide
whether the hit registers, then subtracts the projectile's `hit_damage` from the
target's `health.current_health`. All three components must be present for the
full chain:

```lua
-- Shooter — emits projectiles
load_entity({
    components = {
        transform          = { position = { x = 50, y = 300 } },
        sprite             = { texture_asset_id = "tank", width = 32, height = 32, layer = 2 },
        box_collider       = { width = 32, height = 32 },
        projectile_emitter = {
            projectile_velocity = { x = 200, y = 0 },
            projectile_duration = 4.0,
            repeat_frequency    = 0,       -- 0 = manual; call fire_projectile() from a script
            hit_damage          = 25,
            friendly            = false,   -- can hurt the player
        },
    }
})

-- Target — takes damage automatically when hit
load_entity({
    components = {
        transform    = { position = { x = 500, y = 300 } },
        sprite       = { texture_asset_id = "enemy", width = 32, height = 32, layer = 2 },
        box_collider = { width = 32, height = 32 },
        health       = { max_health = 100, current_health = 100 },
    }
})
```

### Collision masks

Masks let you control which entity groups can collide with each other. The engine
performs a bitwise AND between the projectile's `collision_mask` and the target
entity's `entity_mask`; a non-zero result means the hit registers.

```lua
-- entity_mask groups the entity into one or more collision layers
entity_mask = { value = 2 }     -- layer 2 = "enemies"

-- box_collider and projectile_emitter filter which layers they react to
box_collider       = { width = 32, height = 32, collision_mask = 1 }  -- reacts to layer 1
projectile_emitter = { ..., collision_mask = 2 }                       -- hits layer 2 only
```

### Checking health in a script

If you need to react to damage (play a sound, spawn particles, change behaviour
when low on health), poll the health component in `on_update`:

```lua
on_update = function(self, entity, dt)
    if registry.has_health(entity) then
        local hp = registry.get_health(entity)
        if hp.current_health <= 0 then
            blam(entity)    -- destroy when dead
        elseif hp.current_health < hp.max_health * 0.25 then
            play_sound("warning-beep")
        end
    end
end
```

---

## 7. Scripting Entities

Attach a `script` component to give an entity per-frame Lua behaviour. The
engine calls `on_update(self, entity, dt)` every frame and `on_debug_gui(self,
entity)` when editor debug UI is visible. `self` is the script table — store
per-entity state there.

See [`docs/lua-scripting.md`](lua-scripting.md) for the full scripting guide:
update loop, `registry.get_*` / `registry.has_*`, all global helpers, input
binding, and ImGui debug UI.

---

## 8. Scenes

`load_scene(path)` replaces the current scene. Scene transitions are
synchronous — `load_scene` does not return to the caller.

```lua
load_scene("scenes/gameplay.lua")
reload_scene()      -- hot-reload the current scene (useful during development)
```

Switching scenes from inside a script:

```lua
script = {
    on_update = function(self, entity, dt)
        if input.is_key_pressed("r")      then load_scene("scenes/gameplay.lua") end
        if input.is_key_pressed("escape") then load_scene("scenes/title.lua")    end
    end,
}
```

Scene files can be declarative (return a table), procedural (return a function),
or side-effect style (build the scene at top level as the file executes). See
[`docs/scenes.md`](scenes.md) for the full lifecycle, file shape, and asset
preload flow.

---

## 9. Assets

Assets must be loaded before any entity references them. The recommended
approach is `acquire_scene_assets` — it accepts `preload` lists and scans
entity definitions for asset references automatically. Explicit `load_asset`
calls are also supported.

```lua
load_asset({ type = "texture", id = "player",    file = "images/player.png" })
load_asset({ type = "font",    id = "main-font", file = "fonts/arial.ttf", font_size = 16 })
load_asset({ type = "sound",   id = "jump-sfx",  file = "sounds/jump.wav" })
```

All asset paths are relative to the project root. Use `get_asset_path` when
building paths dynamically:

```lua
local path = get_asset_path("images/player.png")
```

For shipping builds, bake assets into a manifest with:

```bash
./OctarineEngine path/to/MyGame -m bake
```

See [`docs/asset-pipeline.md`](asset-pipeline.md) for `.meta` sidecars, atlas
packing, audio normalisation, and the full bake/manifest workflow.

---

## 10. Audio

```lua
play_sound("jump-sfx")          -- one-shot
play_sound("music-gameplay")    -- looped music
```

For spatial audio, add `audio_listener` to the player/camera entity and
`audio_source` to emitting entities. The engine handles attenuation
automatically.

```lua
audio_listener = {}

audio_source = {
    asset_id = "enemy-growl",
    loop     = true,
    volume   = 0.8,
}
```

---

## 11. Camera

The camera follows whichever entity carries a `camera_follow` component. Only
one should be active at a time.

```lua
camera_follow = {}
```

```lua
local cam = get_camera_position()            -- returns {x, y}
set_game_map_dimensions(width_px, height_px) -- clamp scrolling to world bounds
```

Entities with `fixed = true` on their `sprite`, `square`, or `text_label`
render in screen space, unaffected by camera movement — use this for HUD
elements.

---

## 12. Using the Editor

Launch an editor build against your project:

```bash
./OctarineEngine path/to/MyGame
```

The editor adds a toolbar, entity inspector, scene hierarchy, and your
`on_debug_gui` windows. See [`docs/editor.md`](editor.md) for the full panel
reference and keyboard shortcuts.

---

## 13. Game CI

The `setup-engine` composite action downloads the right SDK binary for the
current runner platform and adds it to `PATH`. Use it in game repo workflows to
bake assets or run the game headlessly without building the engine from source.

### Basic usage

```yaml
# .github/workflows/ci.yml (in your game repo)
jobs:
  ci:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4

      - uses: mblackman/Octarine-Engine/.github/actions/setup-engine@v0.1.0
        with:
          engine-version: '0.1.0'   # pin to the version in project.ini
          token: ${{ secrets.GITHUB_TOKEN }}

      # Bake the asset manifest (validates all asset references)
      - run: OctarineEngine . -m bake
```

### Pinning the engine version

Declare the engine version in your `project.ini` so it is visible alongside
your other identity metadata:

```ini
engine_version = 0.1.0
```

When you package the game (`cmake --preset ship-release ...`) the engine
compares this value against its own version at configure time and warns if
they differ, making version drift visible before it reaches CI.

### Available outputs

| Output | Description |
|---|---|
| `engine-path` | Directory containing the engine binary |
| `engine-bin` | Full path to the `OctarineEngine` binary |

### Multi-platform matrix

```yaml
strategy:
  matrix:
    os: [ubuntu-latest, windows-latest, macos-latest]
runs-on: ${{ matrix.os }}
steps:
  - uses: mblackman/Octarine-Engine/.github/actions/setup-engine@v0.1.0
    with:
      engine-version: '0.1.0'
  - run: OctarineEngine . -m bake
```

---

## 14. Further Reading

### Engine documentation

| Doc | What's in it |
|---|---|
| [`docs/ecs-components.md`](ecs-components.md) | Every component — all fields, types, and defaults |
| [`docs/lua-scripting.md`](lua-scripting.md) | Full Lua scripting guide |
| [`docs/scenes.md`](scenes.md) | Scene lifecycle, declarative vs. procedural forms |
| [`docs/asset-pipeline.md`](asset-pipeline.md) | `.meta` sidecars, baking, atlases, audio normalisation |
| [`docs/systems.md`](systems.md) | Built-in systems and the order they run |
| [`docs/events.md`](events.md) | The event bus — every event type and how to use them |
| [`docs/editor.md`](editor.md) | Editor panels, hotkeys, export workflow |
| [`docs/tilemaps.md`](tilemaps.md) | Tilemap scene field |
| [`docs/device-builds.md`](device-builds.md) | Shipping for desktop and Android |

### Lua API reference

[`lua_api.smoke.lua`](https://github.com/mblackman/Octarine-Engine/blob/main/lua_api.smoke.lua)
is the authoritative, auto-generated EmmyLua stub of the entire Lua surface.
Load it into your editor (VS Code + the Lua Language Server extension is
recommended) for autocomplete and inline docs.

### Example project

[Octarine-Engine-Example](https://github.com/mblackman/Octarine-Engine-Example)
is a complete, runnable game showing every engine feature in practice.

### Engine repository

Source, issue tracker, and releases:
[github.com/mblackman/Octarine-Engine](https://github.com/mblackman/Octarine-Engine)

### Lua 5.4

The engine embeds Lua 5.4. Reference manual: [lua.org/manual/5.4](https://www.lua.org/manual/5.4/)

### Dear ImGui

The UI binding closely mirrors the C++ API. The
[ImGui wiki](https://github.com/ocornut/imgui/wiki) and header comments in
[`imgui.h`](https://github.com/ocornut/imgui/blob/master/imgui.h) are the best
references for functions not yet in the engine's Lua stub.
