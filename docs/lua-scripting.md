# Lua Scripting Guide

Octarine Engine is designed to be driven by Lua scripts. This guide explains how to structure your game project and use the engine's Lua API.

For a complete working example, refer to the `Octarine-Engine-Example-main/` directory in the repository.

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
```

---

## 3. Loading Assets

Assets must be loaded before they can be used by entities.

```lua
-- Load a texture
load_asset({ 
    type = "texture", 
    id = "player-texture", 
    file = "images/player.png" 
})

-- Load a font
load_asset({ 
    type = "font", 
    id = "main-font", 
    file = "fonts/arial.ttf", 
    font_size = 12 
})
```

---

### 4. Creating Entities

Entities are created by passing a table to `load_entity`. They are composed of a `tag` (optional) and several `components`.

For a full list of available components and their fields, see:
👉 **[ECS Component Reference](ecs-components.md)**

```lua
local player = {

    tag = "player",
    components = {
        transform = {
            position = { x = 100, y = 100 },
            scale = { x = 1.0, y = 1.0 },
            rotation = 0.0,
        },
        sprite = {
            texture_asset_id = "player-texture",
            width = 32,
            height = 32,
            layer = 1
        },
        rigidbody = {
            velocity = { x = 0, y = 0 }
        },
        script = {
            speed = 100,
            on_update = function(self, entity, delta_time)
                -- Movement via the global `input` table. See lib/player_controller.lua
                -- in the example project for a clamped + normalized version.
                local pos = get_position(entity)
                local dx, dy = 0, 0
                if input.is_key_down("left")  then dx = dx - 1 end
                if input.is_key_down("right") then dx = dx + 1 end
                if input.is_key_down("up")    then dy = dy - 1 end
                if input.is_key_down("down")  then dy = dy + 1 end
                set_position(entity, pos.x + dx * self.speed * delta_time,
                                     pos.y + dy * self.speed * delta_time)
                if input.is_key_pressed("space") then log("Jump!") end
            end
        }
    }
}

load_entity(player)
```

---

## 5. Lua API Reference

| Function | Description |
| --- | --- |
| `log(message)` | Logs a message to the console. |
| `load_asset(table)` | Loads a texture or font. |
| `load_entity(table)` | Spawns a new entity with components. |
| `blam(entity)` | Destroys the specified entity. |
| `get_asset_path(path)` | Returns the absolute path for a relative asset path. |
| `set_position(entity, x, y)` | Manually updates an entity's position. |
| `get_position(entity)` | Returns the entity's position as a `vec2`. |
| `get_game_map_dimensions()` | Returns playable area as `vec2(width, height)`. |
| `set_game_map_dimensions(w, h)` | Sets the playable area used by off-screen despawn. |
| `set_sprite_src_rect(entity, x, y)` | Sets the sprite's source-rect origin (animation row/col). |
| `fire_projectile(entity, dx, dy)` | Spawns a projectile from `entity`'s `projectile_emitter`, aimed at `(dx, dy)` (zero = emitter default). |
| `read_file_lines(path)` | Reads a file and returns its lines as a table. |
| `quit_game()` | Gracefully exits the engine. |

### `input` table

Polling, action map, and callbacks. Action bindings + callbacks are cleared on `load_scene`/`reload_scene` — re-install in the new scene's `setup` (use a guard for idempotency).

| Function | Description |
| --- | --- |
| `input.is_key_down(name)` / `is_key_pressed(name)` / `is_key_released(name)` | Held / went-down-this-frame / went-up-this-frame. |
| `input.is_mouse_down(btn)` / `is_mouse_pressed(btn)` / `is_mouse_released(btn)` | `btn` is `"left"`, `"middle"`, `"right"`, `"x1"`, `"x2"`, or an SDL button id. |
| `input.mouse_position()` | Cursor position as `vec2`. Mapped to game resolution, viewport-aware (works in Editor). |
| `input.mouse_wheel()` | Per-frame wheel delta as `vec2`. |
| `input.bind(action, key)` / `unbind(action, key?)` | Register / remove key for action; nil key removes all. |
| `input.is_action_down(name)` / `is_action_pressed(name)` / `is_action_released(name)` | As above, OR over all keys bound to the action. |
| `input.on_key_down(fn)` / `on_key_up(fn)` | `fn(key_name)`. |
| `input.on_mouse_down(fn)` / `on_mouse_up(fn)` | `fn(btn, x, y)`. |
| `input.on_mouse_wheel(fn)` | `fn(dx, dy)`. |

---

## 6. ImGui Integration

The engine exposes ImGui to Lua, allowing you to create debug tools and UI directly in script:

```lua
function on_debug_gui(self, entity)
    if ImGui.Begin("Tools") then
        if ImGui.Button("Spawn Enemy") then
            -- Logic to spawn an enemy
        end
    end
    ImGui.End()
end
```

To use this, add the `on_debug_gui` function to a `script` component.

---

## 7. Global Callbacks

The engine looks for specific functions in your scripts:

- `on_update(self, entity, delta_time)`: Called every frame.
- `on_debug_gui(self, entity)`: Called during the ImGui rendering pass.
- `on_click(self, entity)`: Called if the entity has a `button` component and is clicked.
