# ECS Component Reference

This document lists all components available to Lua in the Octarine Engine, including their fields, types, and
functionality.

Components are added to entities within the `components` table of an entity definition.

---

## Core Components

### `transform`

Defines the position, scale, and rotation of an entity in the world.

| Field      | Type           | Default      | Description              |
|------------|----------------|--------------|--------------------------|
| `position` | `table {x, y}` | `{x=0, y=0}` | The world position.      |
| `scale`    | `table {x, y}` | `{x=1, y=1}` | The scale of the entity. |
| `rotation` | `number`       | `0.0`        | Rotation in degrees.     |

### `rigidbody`

Gives an entity physical properties like velocity.

| Field      | Type           | Default      | Description                              |
|------------|----------------|--------------|------------------------------------------|
| `velocity` | `table {x, y}` | `{x=0, y=0}` | The movement speed in pixels per second. |

---

## Rendering Components

### `sprite`

Renders a 2D texture from the asset manager.

| Field              | Type      | Default      | Description                                          |
|--------------------|-----------|--------------|------------------------------------------------------|
| `texture_asset_id` | `string`  | **Required** | The ID of the loaded texture asset.                  |
| `width`            | `number`  | **Required** | The render width.                                    |
| `height`           | `number`  | **Required** | The render height.                                   |
| `layer`            | `number`  | `1`          | Z-index for rendering order (higher is on top).      |
| `fixed`            | `boolean` | `false`      | If true, the sprite ignores the camera (UI element). |
| `src_rect_x`       | `number`  | `0`          | X-offset in the source texture.                      |
| `src_rect_y`       | `number`  | `0`          | Y-offset in the source texture.                      |

### `animation`

Adds frame-based animation to a `sprite`.

| Field        | Type     | Default | Description                              |
|--------------|----------|---------|------------------------------------------|
| `num_frames` | `number` | `1`     | Total number of frames in the animation. |
| `speed_rate` | `number` | `1`     | Animation speed in frames per second.    |

### `square`

Renders a simple colored rectangle.

| Field      | Type                 | Default                | Description              |
|------------|----------------------|------------------------|--------------------------|
| `position` | `table {x, y}`       | `{x=0, y=0}`           | Local offset position.   |
| `width`    | `number`             | `0`                    | Width of the rectangle.  |
| `height`   | `number`             | `0`                    | Height of the rectangle. |
| `color`    | `table {r, g, b, a}` | `{r=0, g=0, b=0, a=0}` | The color (0-255).       |
| `layer`    | `number`             | `1`                    | Rendering layer.         |
| `fixed`    | `boolean`            | `false`                | If true, ignores camera. |

### `text_label`

Renders text using a loaded font.

| Field      | Type                 | Default                | Description                      |
|------------|----------------------|------------------------|----------------------------------|
| `text`     | `string`             | **Required**           | The text to display.             |
| `font_id`  | `string`             | **Required**           | The ID of the loaded font asset. |
| `position` | `table {x, y}`       | `{x=0, y=0}`           | Local offset position.           |
| `color`    | `table {r, g, b, a}` | `{r=0, g=0, b=0, a=0}` | Text color.                      |
| `layer`    | `number`             | `1`                    | Rendering layer.                 |
| `is_fixed` | `boolean`            | `true`                 | If true, ignores camera.         |

---

## Logic & Interaction

### `script`

Attaches custom Lua behavior to an entity.

| Field          | Type       | Description                                               |
|----------------|------------|-----------------------------------------------------------|
| `on_update`    | `function` | Called every frame: `function(self, entity, delta_time)`. |
| `on_debug_gui` | `function` | Called during ImGui pass: `function(self, entity)`.       |

### `box_collider`

Defines a rectangular area for collision detection.

| Field            | Type           | Default      | Description                                  |
|------------------|----------------|--------------|----------------------------------------------|
| `width`          | `number`       | `1`          | Width of the collider box.                   |
| `height`         | `number`       | `1`          | Height of the collider box.                  |
| `offset`         | `table {x, y}` | `{x=0, y=0}` | Offset from the entity's transform position. |
| `collision_mask` | `number`       | `1`          | Bitmask for filtering collisions.            |

### `projectile_emitter`

Automatically spawns projectile entities at a set interval.

| Field                 | Type           | Default      | Description                             |
|-----------------------|----------------|--------------|-----------------------------------------|
| `projectile_velocity` | `table {x, y}` | `{x=0, y=0}` | Velocity of spawned projectiles.        |
| `repeat_frequency`    | `number`       | `1.0`        | Time in seconds between spawns.         |
| `projectile_duration` | `number`       | `1.0`        | Lifetime of projectiles in seconds.     |
| `hit_damage`          | `number`       | `10`         | Damage value assigned to projectiles.   |
| `collision_mask`      | `number`       | `1`          | Collision mask for spawned projectiles. |

### `health`

Tracks an entity's health.

| Field        | Type     | Default | Description               |
|--------------|----------|---------|---------------------------|
| `max_health` | `number` | `100`   | The maximum health value. |

### `camera_follow`

Forces the main camera to follow this entity. Has no fields.
Example: `camera_follow = {}`

### `audio_listener`

Marks an entity as the spatial audio listener. Attenuation is computed relative
to this entity's position. Has no fields — only one listener should exist per scene.
Example: `audio_listener = {}`

### `audio_source`

Attaches a looping or triggered sound to an entity. The engine spatializes it
relative to the `audio_listener`.

| Field      | Type      | Default | Description                                        |
|------------|-----------|---------|----------------------------------------------------|
| `asset_id` | `string`  | `""`    | The ID of the loaded sound asset.                  |
| `loop`     | `boolean` | `false` | If true, the sound loops continuously.             |
| `volume`   | `number`  | `1.0`   | Playback volume (0.0–1.0).                         |

### `ui_button`

Makes an entity clickable.

| Field       | Type       | Default | Description                                         |
|-------------|------------|---------|-----------------------------------------------------|
| `is_active` | `boolean`  | `true`  | Whether the button is interactive.                  |
| `on_click`  | `function` | `nil`   | Function called on click: `function(self, entity)`. |

---

## Common Entity Patterns

Components compose naturally. These combinations cover most game object archetypes.

### Player

```lua
{
    tag  = "player",
    name = "Player",
    components = {
        transform      = { position = { x = 200, y = 300 } },
        sprite         = { texture_asset_id = "player", width = 32, height = 32, layer = 2 },
        rigidbody      = { velocity = { x = 0, y = 0 } },
        box_collider   = { width = 32, height = 32 },
        health         = { max_health = 100, current_health = 100 },
        camera_follow  = {},
        audio_listener = {},
        script         = { speed = 100, on_update = player_update },
    }
}
```

### Enemy

```lua
{
    tag  = "enemy",
    name = "Enemy",
    components = {
        transform          = { position = { x = 600, y = 300 } },
        sprite             = { texture_asset_id = "enemy", width = 32, height = 32, layer = 2 },
        rigidbody          = { velocity = { x = 0, y = 0 } },
        box_collider       = { width = 32, height = 32 },
        health             = { max_health = 50, current_health = 50 },
        projectile_emitter = {
            projectile_velocity = { x = -150, y = 0 },
            projectile_duration = 4.0,
            repeat_frequency    = 2.0,
            hit_damage          = 10,
            friendly            = false,
        },
    }
}
```

### Ambient sound source

```lua
{
    components = {
        transform    = { position = { x = 400, y = 400 } },
        audio_source = { asset_id = "ambient-wind", loop = true, volume = 0.6 },
    }
}
```

### HUD overlay

Text and shapes with `fixed = true` / `is_fixed = true` stay in screen space,
unaffected by the camera. Layer them high so they always draw on top.

```lua
-- Score label
{
    components = {
        transform  = { position = { x = 20, y = 20 } },
        text_label = {
            text     = "Score: 0",
            font_id  = "hud-font",
            color    = { r = 255, g = 255, b = 255, a = 255 },
            layer    = 10,
            is_fixed = true,
        },
        script = {
            on_update = function(self, entity, dt)
                -- update text via registry when score changes
            end,
        },
    }
}

-- Semi-transparent health bar background
{
    components = {
        transform = { position = { x = 20, y = 50 } },
        square    = {
            width  = 200,
            height = 16,
            color  = { r = 60, g = 0, b = 0, a = 180 },
            layer  = 9,
            fixed  = true,
        },
    }
}
```

---

## Internal / Advanced

### `entity_mask`

Explicitly sets the collision bitmask for an entity.

| Field   | Type     | Default | Description        |
|---------|----------|---------|--------------------|
| `value` | `number` | `1`     | The bitmask value. |
