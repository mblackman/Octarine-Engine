# ECS Component Reference

This document lists all components available to Lua in the Octarine Engine, including their fields, types, and
functionality.

Components are added to entities within the `components` table of an entity definition.

---

## Core Components

### `transform`

Defines the position, scale, and rotation of an entity in the world.

| Field      | Type           | Default          | Description                                              |
|------------|----------------|------------------|----------------------------------------------------------|
| `position` | `table {x, y}` | `{x=0, y=0}`     | The world position of the entity's top-left corner.      |
| `scale`    | `table {x, y}` or `number` | `{x=1, y=1}` | The scale of the entity, applied about its anchor. A bare number scales both axes. |
| `rotation` | `number`       | `0.0`            | Rotation, in the project's `AngleUnit` (degrees by default). |
| `pivot`    | `table {x, y}` | *(centre)*       | Anchor for rotation *and* scale. Builds a `pivot` component. |

```lua
components = { transform = { scale = 2 } }                  -- uniform: both axes at once
components = { transform = { scale = { x = 2, y = 5 } } }   -- per-axis
```

### `pivot`

The anchor that rotation and scale both act about. It is its own component, independent of
`rotation` in both directions: an entity can rotate with no pivot, and can carry a pivot with no
rotation purely to anchor its scaling.

| Field  | Type           | Default          | Description                                        |
|--------|----------------|------------------|----------------------------------------------------|
| `x`    | `number`       | `0.5`            | Horizontal anchor, normalized to the entity's bounds. |
| `y`    | `number`       | `0.5`            | Vertical anchor, normalized to the entity's bounds.   |

Unlike `position` and `scale`, the standalone component is authored **flat** — `x` and `y` at the
top level, the same shape it has inside the transform table. The nested `{ value = { x, y } }`
spelling those two components use is accepted as well, so either reads correctly:

```lua
components = { transform = { pivot = { x = 0, y = 0 } } }   -- inline in the transform
components = { pivot = { x = 0, y = 0 } }                   -- as its own component
components = { pivot = 0 }                                  -- shorthand: both axes at once
components = { pivot = { value = { x = 0, y = 0 } } }       -- nested, matching scale/position
```

At runtime the component exposes `value` (a `vec2`) plus `x` and `y` accessors:

```lua
local p = registry.get_pivot(entity)   -- registry.has_pivot(entity) to test first
p.x = 0                                -- writes through to the component
```

`pivot` is normalized against whatever the entity draws at: `{x=0, y=0}` is the top-left corner,
`{x=0.5, y=0.5}` the centre, `{x=1, y=1}` the bottom-right. Values outside `0..1` are allowed and
place the anchor outside the bounds, which is a convenient way to make something orbit a point.

The size the anchor resolves against comes from the entity's `sprite`, else its `square`, else its
`boxcollider`. An entity with none of those has nothing to anchor to and falls back to `position`.

**Omitting the component anchors to the centre of the entity's geometry** — the same point
`{x=0.5, y=0.5}` resolves to. So you only need a `pivot` to move the anchor somewhere other than
the middle, and rotation behaves identically with or without one.

The anchor is the single point everything agrees on: the sprite and primitive renderers, the
collision broadphase, the collider debug draw, and — importantly — child entities, which orbit
their parent's anchor rather than its top-left corner.

`scale` acts about it too, so an entity grows in place rather than sprawling down and to the right
from its corner. Anchor at `{x=0, y=0}` to get corner-anchored growth instead. Scale `1` is always
a no-op: an unscaled entity never moves because of this.

> **Migrating an existing project.** Scaling about the anchor is a change in where scaled entities
> land. Anything at scale `1` is untouched, and rotation-only behaviour is unchanged, but a sprite
> at scale `2` now sits half its growth up and to the left of where it used to, holding its centre
> instead of its corner. Add `pivot = { x = 0, y = 0 }` to restore the old placement per entity.
>
> Separately, a child that carries its own geometry under a **rotated** parent moves even at scale
> `1`. That is a bug fix: the child's anchor previously did not travel through the parent's
> rotation, so it span about the wrong point. Nothing needs to change in your project, but
> rotated hierarchies will look different — and correct.

### Reading transforms from Lua

`position`, `scale`, and `pivot` all expose their `vec2` as `value` plus `x` and `y` accessors that
write through to the component:

```lua
local pos = registry.get_position(entity)
pos.x = 100          -- equivalent to pos.value.x = 100
local s = registry.get_scale(entity)
s.scale = 2          -- uniform view: writes both axes
if not s:is_uniform() then print(s.x, s.y) end
```

`scale` (the singular accessor) reports the **x** axis, so it only summarises the pair when
`is_uniform()` is true. Reading it on a per-axis scale is safe; it just tells you half the story.

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
