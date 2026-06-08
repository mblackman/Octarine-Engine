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
6. [Scripting Entities](#6-scripting-entities)
7. [Input](#7-input)
8. [Scenes](#8-scenes)
9. [Assets](#9-assets)
10. [Audio](#10-audio)
11. [Camera](#11-camera)
12. [Debug UI with ImGui](#12-debug-ui-with-imgui)
13. [Using the Editor](#13-using-the-editor)
14. [Further Reading](#14-further-reading)

---

## 1. Prerequisites

Before writing game code you need a built copy of the engine.

### Option A — Download a pre-built binary (recommended)

CI builds the engine for Windows, macOS, and Linux on every push to `main`.
Download the artifact for your platform from the
[Actions tab](https://github.com/mblackman/Octarine-Engine/actions) on GitHub:

1. Open the latest successful **Build** workflow run.
2. Scroll to **Artifacts** at the bottom.
3. Download `OctarineEngine-<platform>-editor-release` for day-to-day development
   or `OctarineEngine-<platform>-player-release` for a runtime-only build with no
   editor tools.
4. Unzip and run the binary directly — no install step required.

Available artifact names:

| Artifact | Contents |
|---|---|
| `OctarineEngine-windows-editor-release` | Windows editor build |
| `OctarineEngine-windows-player-release` | Windows player-only build |
| `OctarineEngine-macos-editor-release` | macOS editor build |
| `OctarineEngine-macos-player-release` | macOS player-only build |
| `OctarineEngine-linux-editor-release` | Linux editor build |
| `OctarineEngine-linux-player-release` | Linux player-only build |

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

`config.ini` lives at the project root and controls window setup:

```ini
Title=My Awesome Game
Version=1.0.0
Author=Your Name
StartupScript=game.lua
DefaultWindowWidth=1280
DefaultWindowHeight=720
DefaultScalingMode=nearest      # 'nearest' for pixel art, 'linear' for smooth
```

`StartupScript` is the first Lua file that runs. Everything else — scenes,
assets, entities — is loaded from there.

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
processes) or procedural (build the scene themselves). See [Scenes](#8-scenes).

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

## 6. Scripting Entities

The `script` component is how entities get custom behaviour. It supports two
callbacks:

| Callback | When called |
|---|---|
| `on_update(self, entity, dt)` | Every frame, in update order |
| `on_debug_gui(self, entity)` | Every frame, but only when editor debug UI is visible |

`self` is the script table itself — store per-entity state there. `entity` is
the entity ID. `dt` is delta time in seconds.

```lua
load_entity({
    components = {
        transform = { position = { x = 400, y = 300 } },
        sprite    = { texture_asset_id = "gem", width = 16, height = 16, layer = 3 },
        script    = {
            speed = 80,
            on_update = function(self, entity, dt)
                if input.is_action_down("move_right") then
                    local pos = registry.get_position(entity)
                    pos.x = pos.x + self.speed * dt
                end
            end,
        },
    }
})
```

**Reading and writing components from a script**

```lua
-- Get a mutable component reference
local pos = registry.get_position(entity)
pos.x = pos.x + 10

-- Check whether a component exists before touching it
if registry.has_health(entity) then
    local hp = registry.get_health(entity)
    hp.current_health = hp.current_health - 1
end
```

The pattern is `registry.get_<key>` / `registry.has_<key>`. Component keys are
listed in [`docs/ecs-components.md`](ecs-components.md).

**Convenience helpers**

```lua
local pos = get_position(entity)      -- returns {x, y}
set_position(entity, new_x, new_y)
local name = get_name(entity)
set_name(entity, "New Name")
blam(entity)                          -- destroy an entity
```

For the full Lua scripting guide see [`docs/lua-scripting.md`](lua-scripting.md).

---

## 7. Input

The `input` table is available globally. Bind named actions to keys once
(typically in `game.lua`), then query them anywhere.

### Binding actions

```lua
input.bind("move_up",    "up")
input.bind("move_up",    "w")       -- multiple keys per action
input.bind("move_down",  "down")
input.bind("move_down",  "s")
input.bind("move_left",  "left")
input.bind("move_left",  "a")
input.bind("move_right", "right")
input.bind("move_right", "d")
input.bind("fire",       "space")
```

### Querying actions and keys

```lua
-- Action queries (works with any key bound to the action)
input.is_action_down("fire")        -- held this frame
input.is_action_pressed("fire")     -- just went down this frame
input.is_action_released("fire")    -- just came up this frame

-- Raw key queries
input.is_key_down("escape")
input.is_key_pressed("f5")

-- Mouse
local pos = input.mouse_position()     -- returns {x, y}
input.is_mouse_down(1)                 -- 1 = left, 2 = right, 3 = middle
input.is_mouse_pressed(1)
```

### Example: 8-way movement

```lua
local function update(self, entity, dt)
    local dx, dy = 0, 0
    if input.is_action_down("move_up")    then dy = dy - 1 end
    if input.is_action_down("move_down")  then dy = dy + 1 end
    if input.is_action_down("move_left")  then dx = dx - 1 end
    if input.is_action_down("move_right") then dx = dx + 1 end

    if dx ~= 0 or dy ~= 0 then
        local len = math.sqrt(dx * dx + dy * dy)
        local rb  = registry.get_rigidbody(entity)
        rb.velocity.x = (dx / len) * self.speed
        rb.velocity.y = (dy / len) * self.speed
    else
        local rb = registry.get_rigidbody(entity)
        rb.velocity.x = 0
        rb.velocity.y = 0
    end
end
```

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

## 12. Debug UI with ImGui

The engine ships full [Dear ImGui](https://github.com/ocornut/imgui) bindings.
Use them inside `on_debug_gui` callbacks — they only fire when the editor's
debug UI is visible, so release builds pay nothing.

```lua
local spawn_x = 100
local spawn_y = 100

load_entity({
    components = {
        script = {
            on_debug_gui = function(self, entity)
                if ImGui.Begin("Spawn Tool") then
                    spawn_x = ImGui.InputInt("X", spawn_x)
                    spawn_y = ImGui.InputInt("Y", spawn_y)

                    if ImGui.Button("Spawn Enemy") then
                        load_entity({
                            components = {
                                transform = { position = { x = spawn_x, y = spawn_y } },
                                sprite    = { texture_asset_id = "enemy", width = 32, height = 32, layer = 2 },
                                health    = { max_health = 50 },
                            }
                        })
                    end
                end
                ImGui.End()
            end,
        }
    }
})
```

The full ImGui API is documented in the
[Dear ImGui wiki](https://github.com/ocornut/imgui/wiki) and the engine's
generated Lua stub at
[`lua_api.smoke.lua`](https://github.com/mblackman/Octarine-Engine/blob/main/lua_api.smoke.lua)
(search for `ImGui.`).

---

## 13. Using the Editor

Launch an editor build against your project:

```bash
./OctarineEngine path/to/MyGame
```

The editor adds a toolbar, entity inspector, scene hierarchy, and your
`on_debug_gui` windows. See [`docs/editor.md`](editor.md) for the full panel
reference and keyboard shortcuts.

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
