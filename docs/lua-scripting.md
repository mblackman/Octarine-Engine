# Lua Scripting Guide

Octarine Engine is designed to be driven by Lua scripts. This guide covers the
`script` component, the update loop, reading and writing components at runtime,
the full input API, and the ImGui debug UI surface.

For a complete working example, refer to the sibling `Octarine-Engine-Example/`
directory.

---

## 1. Project Structure

A typical Lua project for Octarine Engine should have the following structure:

```text
MyGame/
├── config.ini          # Main configuration file
├── game.lua            # Entry point script
├── scripts/            # Additional Lua scripts
├── images/             # Sprite textures
├── fonts/              # TTF fonts
├── sounds/             # Audio files
└── tilemaps/           # Map data (.map and .png files)
```

---

## 2. Configuration (`config.ini`)

The `config.ini` file defines the game's metadata and initial window settings.

```ini
Title=My Awesome Game
Version=1.0.0
Author=Developer Name
StartupScript=game.lua
DefaultWindowWidth=1280
DefaultWindowHeight=720
DefaultScalingMode=nearest # 'nearest' or 'linear'
FpsTarget=60               # frame-rate cap; 0 = uncapped (default 60)
```

---

## 3. The Script Component

Attach a `script` table to any entity to give it per-frame Lua behaviour. The
engine calls two callbacks on it each frame:

| Callback | When called |
|---|---|
| `on_update(self, entity, dt)` | Every frame, in system order |
| `on_debug_gui(self, entity)` | Every frame, but only when editor debug UI is visible |
| `on_click(self, entity)` | When the entity has a `ui_button` component and is clicked |

- **`self`** is the script table itself — store per-entity state here.
- **`entity`** is the numeric entity ID — pass it to `registry.*` and helper globals.
- **`dt`** is delta time in seconds since the last frame.

```lua
load_entity({
    components = {
        transform = { position = { x = 100, y = 100 } },
        sprite    = { texture_asset_id = "player", width = 32, height = 32, layer = 2 },
        rigidbody = { velocity = { x = 0, y = 0 } },
        script    = {
            speed    = 80,
            cooldown = 0,
            on_update = function(self, entity, dt)
                self.cooldown = self.cooldown - dt
                -- movement and actions handled here each frame
            end,
        },
    }
})
```

Scripts are plain Lua tables — any key you add to the table persists across
frames and is accessible through `self`.

---

## 4. Reading and Writing Components

### `registry.get_*` / `registry.has_*`

The `registry` table exposes a getter and a presence check for every component.
The pattern is `registry.get_<key>` / `registry.has_<key>`, where the key for
each component is listed in [`docs/ecs-components.md`](ecs-components.md).

```lua
-- Read and mutate a component (get returns a mutable proxy)
local pos = registry.get_position(entity)
pos.x = pos.x + 10

-- Guard before accessing an optional component
if registry.has_health(entity) then
    local hp = registry.get_health(entity)
    hp.current_health = hp.current_health - 1
end

-- Rigidbody velocity
local rb = registry.get_rigidbody(entity)
rb.velocity.x = 150
rb.velocity.y = 0
```

### Entity hierarchy

| Function | Description |
|---|---|
| `registry.get_parent(entity)` | Returns the parent entity, or `nil` if the entity has no parent |
| `registry.set_parent(child, parent)` | Parents `child` under `parent`, detaching it from any previous parent. Self-parenting and cycles are rejected with an error log. |

Parented entities inherit their parent's transform — see
[`docs/ecs-architecture.md`](ecs-architecture.md) for how the hierarchy is resolved.

### Convenience helpers

These globals are shorthand for the most common reads and writes:

| Function | Description |
|---|---|
| `get_position(entity)` | Returns `{x, y}` |
| `set_position(entity, x, y)` | Teleports the entity |
| `get_name(entity)` | Returns the entity's name string |
| `set_name(entity, name)` | Sets the name |
| `set_sprite_src_rect(entity, x, y)` | Sets the sprite source-rect origin |
| `find_entity_by_name(name)` | Returns the entity ID, or `nil` |
| `blam(entity_or_table)` | Destroys an entity, or every entity in a table |

### Other Lua globals

| Function | Description |
|---|---|
| `log(message)` | Print to the engine console |
| `load_asset(table)` | Load a texture, font, or sound |
| `load_entity(table)` | Spawn a new entity; returns the root entity handle (or `nil` on an invalid table) |
| `get_asset_path(path)` | Resolve a project-relative path to an absolute path |
| `fire_projectile(entity, dx, dy)` | Spawn a projectile from the entity's emitter |
| `play_sound(asset_id)` | Play a sound or music track |
| `get_camera_position()` | Returns `{x, y}` of the camera viewport origin |
| `get_game_map_dimensions()` | Returns `{w, h}` of the playable area |
| `set_game_map_dimensions(w, h)` | Set the world bounds for camera clamping |
| `read_file_lines(path)` | Read a file and return its lines as a table |
| `quit_game()` | Gracefully exit the engine |

---

## 5. Input

The `input` table handles action binding, polling, and event callbacks.

### Binding actions

Bind named logical actions to physical keys once at startup (typically in
`game.lua`). Bindings persist across the session unless explicitly removed.

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

input.unbind("fire", "space")       -- remove one key
input.unbind("fire")                -- remove all keys for the action
```

> **Note:** action bindings and callbacks are cleared on `load_scene` /
> `reload_scene`. Re-install them in the new scene's startup or use a guard for
> idempotency. See [`docs/scenes.md`](scenes.md) for the full scene lifecycle.

### Polling

```lua
-- Actions (fires for any key bound to the action)
input.is_action_down("fire")        -- held this frame
input.is_action_pressed("fire")     -- just went down this frame
input.is_action_released("fire")    -- just came up this frame

-- Raw keys
input.is_key_down("escape")
input.is_key_pressed("f5")
input.is_key_released("space")

-- Mouse
local pos   = input.mouse_position()   -- returns {x, y}, viewport-aware
local wheel = input.mouse_wheel()      -- returns {x, y} per-frame delta
input.is_mouse_down(1)                 -- 1 = left, 2 = right, 3 = middle
input.is_mouse_pressed(1)
input.is_mouse_released(1)
```

### Callbacks

```lua
input.on_key_down(function(key_name) ... end)
input.on_key_up(function(key_name) ... end)
input.on_mouse_down(function(btn, x, y) ... end)
input.on_mouse_up(function(btn, x, y) ... end)
input.on_mouse_wheel(function(dx, dy) ... end)
```

### Example: 8-way movement

```lua
local function movement_update(self, entity, dt)
    local dx, dy = 0, 0
    if input.is_action_down("move_up")    then dy = dy - 1 end
    if input.is_action_down("move_down")  then dy = dy + 1 end
    if input.is_action_down("move_left")  then dx = dx - 1 end
    if input.is_action_down("move_right") then dx = dx + 1 end

    local rb = registry.get_rigidbody(entity)
    if dx ~= 0 or dy ~= 0 then
        local len = math.sqrt(dx * dx + dy * dy)
        rb.velocity.x = (dx / len) * self.speed
        rb.velocity.y = (dy / len) * self.speed
    else
        rb.velocity.x = 0
        rb.velocity.y = 0
    end
end

load_entity({
    components = {
        transform = { position = { x = 200, y = 200 } },
        sprite    = { texture_asset_id = "player", width = 32, height = 32, layer = 2 },
        rigidbody = { velocity = { x = 0, y = 0 } },
        script    = { speed = 120, on_update = movement_update },
    }
})
```

---

## 6. ImGui Debug UI

The engine exposes full [Dear ImGui](https://github.com/ocornut/imgui) bindings
to Lua. Wire debug windows through the `on_debug_gui` callback — it only fires
when the editor's debug UI is visible, so release builds pay zero cost.

```lua
load_entity({
    components = {
        script = {
            spawn_x = 100,
            spawn_y = 100,
            on_debug_gui = function(self, entity)
                if ImGui.Begin("Spawn Tool") then
                    self.spawn_x = ImGui.InputInt("X", self.spawn_x)
                    self.spawn_y = ImGui.InputInt("Y", self.spawn_y)

                    if ImGui.Button("Spawn Enemy") then
                        load_entity({
                            components = {
                                transform = { position = { x = self.spawn_x, y = self.spawn_y } },
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

## 7. Adding Component Methods (engine contributors)

When you add a Lua binding for a component (`src/Lua/Bindings/<X>ComponentLuaBinding.h`),
the `bindUsertype` call exposes both fields and **member functions** to scripts:

```cpp
lua.new_usertype<HealthComponent>(kUsertypeName,
    "max_health", &HealthComponent::maxHealth,
    "damage",     &HealthComponent::Damage,
    "heal",       &HealthComponent::Heal,
    "is_dead",    sol::property(&HealthComponent::IsDead),
    "fraction",   sol::property(&HealthComponent::Fraction));
```

### Hard rule

A component method may **read and write the component's own fields only**.

No:

- `Registry&` access (no looking up sibling components, no other entities).
- `EventBus*` access (no `EmitEvent`).
- Cross-entity reads or writes.
- Archetype mutation (`AddComponent` / `RemoveComponent` / `DestroyEntity`).
- Allocation, file I/O, or any other side effect.

Anything that needs to cross those lines belongs in a **system**, not a method.
The moment a method wants to touch a sibling entity or fire an event, move the
logic out and call it from the relevant system's update.

### Why this matters

Component methods run from Lua at unpredictable points in the frame — inside an
`on_update`, inside an `on_debug_gui`, inside a callback chain. Systems run in
known order with known invariants (input → simulation → render). Touching the
registry from a method during, say, a render-pass debug-gui call can mutate
state mid-frame and cause iterator invalidation, stale archetype caches, or
visible single-frame glitches. Keeping methods data-only means you can call
them anywhere safely.

### Patterns

- **Mutators with invariants** — bind with `sol::property` and a clamping
  setter so Lua can't bypass the invariant by direct field assignment.
  `HealthComponent::currentHealth` is the canonical example.
- **Derived read-only state** — bind with `sol::property(&T::accessor)` so
  scripts get field-style access (`health.is_dead`) without exposing a setter.
- **Plain getters/setters** — bind the member function directly.

For the canonical reference, read the comment block in
[`src/Lua/Bindings/LuaBinding.h`](../src/Lua/Bindings/LuaBinding.h) and
[`HealthComponentLuaBinding.h`](../src/Lua/Bindings/HealthComponentLuaBinding.h).
