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
        keyboard_controller = {
            velocity = 100
        },
        script = {
            on_update = function(self, entity, delta_time)
                -- Custom logic here
                if is_key_pressed("space") then
                    log("Jump!")
                end
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
| `is_key_pressed(key)` | Returns true if a key was just pressed (e.g., "space"). |
| `is_key_held(key)` | Returns true if a key is currently being held. |
| `set_position(entity, x, y)` | Manually updates an entity's position. |
| `read_file_lines(path)` | Reads a file and returns its lines as a table. |
| `quit_game()` | Gracefully exits the engine. |

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
