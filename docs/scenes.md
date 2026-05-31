# Scenes

How Octarine loads, swaps, and reloads scenes — the file shape `load_scene`
expects, the lifecycle the engine drives around it, and what survives a
transition.

Audience: project authors writing scene scripts and transitions. The asset
side (catalog, `.meta`, bake) lives in
[`docs/asset-pipeline.md`](asset-pipeline.md); the engine ECS is in
[`docs/ecs-architecture.md`](ecs-architecture.md).

---

## The model

A scene is a **Lua script**. `load_scene(path)` runs it; what the script
returns drives how the engine builds the scene:

| Script returns | Treated as |
|---------------|-----------|
| A **table** | Declarative scene — the loader scans `entities`, `preload`, `tilemap`, etc., acquires assets, loads entities, then calls the optional `setup` / `run` / `load` hook. |
| A **function** | Procedural scene — the loader runs the function once; the function builds the scene itself (typically via `acquire_scene_assets` + `load_entity`). |
| **Nothing** | Side-effect scene — the script built the scene at top level as it executed. |

All three are first-class. Pick whichever fits the scene best; most projects
mix them.

---

## File layout

Scenes are plain Lua files. The conventional location is `<project>/scenes/`,
but the path passed to `load_scene` is resolved via `AssetManager::GetFullPath`
against the project root, so they can live anywhere:

```
<project>/
├── config.ini
├── game.lua                       # startup script
├── scenes/
│   ├── title.lua
│   ├── gameplay.lua
│   └── game_over.lua
└── scripts/lib/
    └── …
```

`StartupScript=` in `config.ini` runs first; it typically calls
`load_scene("scenes/title.lua")` after setting up globals.

---

## Scene file: declarative form (recommended)

The clearest path. Return a table; the loader does the rest.

```lua
-- scenes/gameplay.lua
return {
    -- Optional: ids the static scan can't see (runtime spawns, load-time injection).
    preload = { "explosion", "music-gameplay" },

    -- Optional: tilemap drives a parallel asset reference and renders into the world.
    tilemap = {
        texture_asset_id = assets["tilemap-forest"],
        map = "tilemaps/forest.map",
    },

    -- Entities loaded after assets resolve.
    entities = {
        {
            tag = "player",
            components = {
                transform = { position = { x = 100, y = 100 } },
                sprite    = { texture_asset_id = assets.tank, width = 32, height = 32 },
                rigidbody = { velocity = { x = 0, y = 0 } },
                health    = { max_health = 100 },
                script    = {
                    on_update = function(self, entity, delta_time)
                        -- per-entity per-frame
                    end,
                },
            },
        },
        -- … more entities
    },

    -- Optional one-shot hook fired after assets + entities are in place.
    -- 'setup', 'load', and 'run' are all accepted as the function key.
    setup = function(scene)
        input.bind("jump", "space")
        input.on_key_down(function(key)
            if key == "escape" then load_scene("scenes/title.lua") end
        end)
    end,
}
```

The loader sequences this as:

1. **Scan the scene table** via `SceneAssetScanner` — collects every
   `texture_asset_id`, `font_id`, audio source id, the `tilemap.texture_asset_id`,
   and the explicit `preload` list.
2. **Validate** every reference against the catalog. Unresolved ids log an
   error. If `EngineOptions.assetValidationFatal` is set, the scene load
   aborts.
3. **Acquire** all references through `AssetManager::AcquireAll`. Refcounted
   — ids shared with the previous scene stay loaded across the swap.
4. **Load entities** via `LuaEntityLoader::LoadEntityFromLua` for every row
   in `entities`.
5. **Call `setup` / `load` / `run`** (whichever the table provides) with the
   scene table as the argument.

The legacy `assets = { ... }` block (explicit `{ type, id, file }` rows)
still works, but the catalog scan in step 1 covers the same ground without
the hand-maintained list. Prefer the scan; only use `assets` for projects
predating the catalog.

---

## Scene file: procedural form

Return a function. Useful when the scene's structure is computed at load
time (procedural generation, save-game replay):

```lua
-- scenes/dungeon.lua
return function()
    acquire_scene_assets({
        preload = { "wall", "floor", "torch" },
    })

    for i = 1, 50 do
        load_entity({
            components = {
                transform = { position = random_floor_tile() },
                sprite    = { texture_asset_id = assets.floor, width = 32, height = 32 },
            },
        })
    end

    input.bind("interact", "e")
end
```

You're responsible for calling `acquire_scene_assets` (or the legacy
`load_asset`) yourself — the loader can't scan a function for references
the way it scans a table.

---

## Scene file: side-effect form

Return nothing. The script does everything as it runs:

```lua
-- scenes/intro.lua
acquire_scene_assets({ preload = { "logo" } })
load_entity({
    components = {
        transform = { position = { x = 640, y = 360 } },
        sprite    = { texture_asset_id = assets.logo, width = 256, height = 64 },
    },
})
input.on_key_down(function() load_scene("scenes/title.lua") end)
```

Equivalent to the procedural form, just without the wrapping function.
Cheapest path for a small scene.

---

## Lifecycle

Across a scene swap, the engine runs **acquire-before-release** for assets
and **clear-before-script** for entities:

```
load_scene("B") while scene A is active
│
├── stash A's tracked asset ids (don't release yet)
├── clear A's entities + reset Lua input state
├── run scene B's script
│   ├── (declarative) scan → validate → acquire → load entities → setup hook
│   ├── (procedural)   run the returned function (script acquires itself)
│   └── (side-effect)  script already ran during the top-level execution
└── release A's stashed asset ids
    └── ids B re-acquired survive thanks to refcounting; the rest unload
```

The entity clear happens **before** the script runs, not after — a
side-effect script that spawns entities at the top level needs the old
entities gone first, otherwise the clear would wipe the new ones.

Asset release happens **after** the new scene's acquires, so a texture
shared by both scenes never unloads/reloads across the swap.

### `reload_scene()`

Re-runs the script at the currently loaded path. Editor-only
(`OCTARINE_WITH_EDITOR`-gated): logs a warning otherwise. Useful for
iteration — edit the file, press the reload hotkey, see the change.

### `stop_scene()` / `clear_scene()`

Clears entities, releases tracked assets, sets `scene_running_ = false`. No
new scene loads — the engine sits idle until the next `load_scene`. Both
names are bound to the same C++ entry point.

### `load_scene` from inside a scene

Calling `load_scene` from within an `on_update`, an `on_key_down`, or a
`setup` hook is fine. The swap runs synchronously the moment the call
returns to the loader, so any code after the `load_scene` call runs against
the OLD scene's state — keep the call near the end of the callback, or
return immediately after.

---

## What gets cleared

| State | Behavior |
|-------|----------|
| User entities | Cleared via `Registry::ClearUserEntities`. |
| Lua input action bindings | Cleared via `InputSystem::ResetLuaState`. |
| `input.on_key_*` / `input.on_mouse_*` callbacks | Cleared. |
| Per-frame held / pressed / released input state | Cleared (so a tap that started during the swap doesn't stick). |
| Tracked assets for the previous scene | Released after the new scene acquires. Refcounted. |

Inputs in particular: every `input.bind`, every `input.on_*`, and every
action map entry the previous scene installed is gone after the swap. The
new scene's `setup` (or top-level script) re-installs whatever it needs.
Use a guard if your `setup` might run twice:

```lua
setup = function(scene)
    if not scene._installed then
        input.bind("jump", "space")
        scene._installed = true
    end
end,
```

---

## What survives

| State | Behavior |
|-------|----------|
| Asset catalog | One catalog for the lifetime of the process. The scene swap touches refcounts, never the catalog itself. |
| Singletons on the `Registry` (`AssetManager`, `GameConfig`, renderer, mixer, …) | Persist across scenes. Set once in `Game::Setup`. |
| Lua globals (`assets`, modules, `require`'d libraries) | Persist. The Lua state is one per process. |
| Engine systems registered in `Game::Setup` | Persist. Systems iterate whatever entities exist after the swap. |

Anything you want to carry across scenes (player progress, score,
checkpoints) goes in a singleton or a Lua global, not in an entity.

---

## Patterns

### Per-scene `setup` + `teardown`

The loader calls `setup` automatically. There is no symmetric `teardown`
hook — the engine clears state mechanically (entities, input). For
manual cleanup (closing a socket, stopping a stream you opened by hand),
the cleanest pattern is to attach a `tear_down` field to the scene table
and call it before your next `load_scene`:

```lua
local scene = {
    setup = function(self)
        self.socket = open_my_socket()
    end,
    tear_down = function(self)
        if self.socket then self.socket:close() end
    end,
}

input.on_key_down(function(key)
    if key == "escape" then
        scene:tear_down()
        load_scene("scenes/title.lua")
    end
end)

return scene
```

Engine-side state (entities, input) is still cleared automatically — the
hook is for *your* extras.

### Persistent state across scenes

Singleton-style on the C++ side via `Registry::Set<T>` / `Get<T>`; Lua
globals on the script side. A simple Lua-side pattern:

```lua
-- game.lua
game_state = game_state or { score = 0, lives = 3 }

load_scene("scenes/title.lua")
```

```lua
-- scenes/game_over.lua
return {
    setup = function(self)
        log("Final score: " .. tostring(game_state.score))
    end,
}
```

### Hot-iterate during dev

Bind a key to `reload_scene` in your dev `setup`:

```lua
setup = function(scene)
    -- editor-only convenience
    input.on_key_down(function(key)
        if key == "f5" then reload_scene() end
    end)
end,
```

The editor toolbar also exposes a reload action; this is the equivalent
from within the game.

---

## Worked example: title → gameplay → game over

```lua
-- scenes/title.lua
return {
    entities = {
        {
            tag = "title_text",
            components = {
                transform  = { position = { x = 640, y = 360 } },
                text_label = { text = "Press SPACE", font_id = assets["ui-32"] },
            },
        },
    },
    setup = function()
        input.on_key_down(function(key)
            if key == "space" then load_scene("scenes/gameplay.lua") end
        end)
    end,
}
```

```lua
-- scenes/gameplay.lua
return {
    preload = { "explosion" }, -- spawned procedurally on death
    entities = { /* player, enemies, walls … */ },
    setup = function(scene)
        scene.score = 0
        input.bind("shoot", "space")
    end,
}
```

```lua
-- scenes/game_over.lua
return {
    entities = {
        {
            components = {
                transform  = { position = { x = 640, y = 360 } },
                text_label = { text = "Game Over", font_id = assets["ui-48"] },
            },
        },
    },
    setup = function()
        input.on_key_down(function(key)
            if key == "r" then load_scene("scenes/gameplay.lua") end
            if key == "escape" then load_scene("scenes/title.lua") end
        end)
    end,
}
```

---

## Troubleshooting

**Scene loads but inputs don't respond** — `input.bind` / `input.on_*`
calls have to happen *after* the swap, i.e. in the new scene's `setup` or
its top-level script. Bindings on the global Lua state from before the
swap were cleared.

**Asset error: "unresolved asset reference"** — the scene scan saw an id
the catalog doesn't have. Add the file under the project tree (it'll be
picked up by the scan), set `meta.id` if you want a non-default id, or
add the id to the scene's `preload = { ... }` if it's only ever
runtime-spawned. See [`docs/asset-pipeline.md`](asset-pipeline.md) §
Troubleshooting.

**Texture flickers on every scene reload** — refcounting isn't holding it
across the swap. Likely the asset id you're using in scene A differs from
scene B's (typo, or a `meta.id` override on one side). Acquire-before-release
only protects ids that match exactly.

**`reload_scene` warns "no scene currently loaded"** — the editor's
`EditorPersistence.currentScenePath` is empty because no `load_scene`
has run yet (or the build wasn't editor-gated). Call `load_scene` once
to seed it.

**`setup` runs twice** — likely a `load_scene` chain that re-enters the
same path. Guard `setup` against double-install:
`if not scene._installed then … scene._installed = true end`.

---

## Where to look in the source

| Concern | File |
|---------|------|
| `load_scene` / `reload_scene` / `stop_scene` / `clear_scene` Lua bindings | `src/Lua/Modules/SceneModuleLuaBinding.cpp` |
| Loader sequencing (acquire-before-release, entity clear, script run, hook call) | `src/Game/Game.cpp` (`Game::LoadScene`, `Game::ReloadScene`, `Game::StopScene`, `Game::clearSceneEntities`) |
| Scene asset scanning | `src/AssetManager/SceneAssetScanner.{h,cpp}` |
| Per-scene asset tracking + refcount | `Game::TrackSceneAssets`, `AssetManager::AcquireAll` / `ReleaseAll` |
| Lua input reset across swap | `src/Systems/InputSystem.h` (`InputSystem::ResetLuaState`) |
| Entity clear across swap | `src/ECS/Registry.h` (`Registry::ClearUserEntities`) |
| Entity-from-table loader | `src/Lua/LuaEntityLoader.{h,cpp}` |
