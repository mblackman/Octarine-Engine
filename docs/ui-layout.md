# UI Layout

How Octarine positions UI: a canvas root, anchored children, and the
`UILayoutSystem` that resolves them into pixel rectangles every frame.

Audience: project authors building menus, HUDs, and overlays in Lua. The
underlying scene/entity model is in [`docs/scenes.md`](scenes.md); the Lua
surface as a whole is in [`docs/lua-scripting.md`](lua-scripting.md).

---

## The model

UI layout is driven entirely by the **entity hierarchy** (`registry.set_parent`),
not by a separate widget tree:

1. One entity is the **canvas** — the layout root. It defines the coordinate
   space everything else is measured against.
2. Every other UI entity is a **descendant** of that canvas, parented with
   `registry.set_parent(child, parent)`.
3. Each child carries an **anchor** that says where, inside its parent's
   rectangle, its own rectangle should sit.

Each frame `UILayoutSystem` finds every canvas, computes the canvas rectangle,
then walks the hierarchy depth-first. For each child it reads the anchor and
writes a resolved `ui_rect` (`left/top/right/bottom/layer`) in pixels. The
renderer draws from those rects.

Only **descendants of a canvas** are laid out. An anchored entity with no
canvas ancestor is never resolved and never appears.

---

## Quick start

```lua
-- A canvas filling the window, plus one centered label.
local screen = load_entity({ components = {} })
ui.canvas(screen, { fixed = true })

local title = load_entity({
    components = {
        text_label = { text = "GRAVEWARDEN", font_id = "ui-font" },
    },
})
registry.set_parent(title, screen)        -- child first, parent second
ui.anchor(title, "center", { width = 400, height = 40 })
```

`set_parent(child, parent)` — the **child** is the first argument. Reversing it
parents the canvas under the label, which both breaks layout and can trip
hierarchy-cycle errors.

---

## The API

All four live on the global `ui` table, installed by the UI Lua module.

### `ui.canvas(entity, opts?)`

Marks `entity` as a layout root. `opts` (all optional):

| Key          | Default | Meaning                                                                   |
|--------------|---------|---------------------------------------------------------------------------|
| `fixed`      | `true`  | Canvas is screen-fixed (does not scroll with the camera).                 |
| `width`      | `0`     | Canvas width in pixels. `0` → use the live viewport width at layout time. |
| `height`     | `0`     | Canvas height in pixels. `0` → use the live viewport height.              |
| `base_layer` | `0`     | Render layer all entities in this canvas inherit as their baseline.       |

The canvas itself needs no transform components — `{ components = {} }` is a
fine canvas entity.

### `ui.anchor(entity, preset_or_table, opts?)`

Attaches (or replaces) the anchor on `entity`. The second argument is either a
**preset name** (string) or a **raw anchor table**.

**Preset form** — `ui.anchor(e, "center", { width = 400, height = 40 })`. The
optional third argument reads only three keys:

| Opt key  | Used by       | Meaning                      |
|----------|---------------|------------------------------|
| `width`  | sized presets | Element width in pixels.     |
| `height` | sized presets | Element height in pixels.    |
| `margin` | `fill`        | Inset from the parent edges. |

Presets:

| Preset                                                 | Result                                           |
|--------------------------------------------------------|--------------------------------------------------|
| `fill`                                                 | Stretch to the parent rect, inset by `margin`.   |
| `center`                                               | `width`×`height` box centered in the parent.     |
| `top_left`, `top_right`, `bottom_left`, `bottom_right` | `width`×`height` box pinned to that corner.      |
| `top_center`, `bottom_center`                          | `width`×`height` box centered on that edge.      |
| `left`, `right`                                        | Full-height bar of `width`, pinned to that side. |

> Presets ignore `offset_*` keys. Passing `offset_top` to `ui.anchor(e, "center", {...})`
> does nothing — use the raw table form when you need explicit offsets.

**Raw table form** — full control:

```lua
ui.anchor(entity, {
    left = 0.5, top = 0.5, right = 0.5, bottom = 0.5,  -- anchors: fraction of parent (0..1)
    offset_left  = -200, offset_top    = 30,           -- offsets: pixels added after
    offset_right =  200, offset_bottom = 50,
})
```

Each edge resolves to:

```
edge_px = parent_edge + parent_size * anchor + offset
```

So `left = 0.5, offset_left = -200` means "parent's horizontal midpoint, minus
200px". The four anchors default to `0,0,1,1` (fill); the four offsets default
to `0`. Width is `offset_right - offset_left`, height is
`offset_bottom - offset_top` — the box above is 400 wide, 20 tall, sitting 40px
below the parent center.

### `ui.z_index(entity, z)`

Adds `z` to the entity's render layer. Layers **accumulate down the hierarchy**:
a child's final layer is `canvas.base_layer` + the sum of every `z_index` on the
path from the canvas to that child. Use it to lift a panel and its contents
above the background in one place.

---

## Worked example: a title screen

```lua
-- menu.lua — side-effect scene: builds its entities at file scope.

local menu = load_entity({ components = {} })
ui.canvas(menu, { fixed = true })

-- Full-bleed background.
local bg = load_entity({
    components = { square = { color = { r = 16, g = 10, b = 24, a = 255 }, is_fixed = true } },
})
registry.set_parent(bg, menu)
ui.anchor(bg, "fill")

-- Centered title.
local title = load_entity({
    components = {
        text_label = { text = "GRAVEWARDEN", font_id = "ui-font",
                       color = { r = 200, g = 160, b = 230, a = 255 }, is_fixed = true },
    },
})
registry.set_parent(title, menu)
ui.anchor(title, "center", { width = 400, height = 40 })

-- Subtitle, 40px below center (raw table so the offset applies).
local sub = load_entity({
    components = {
        text_label = { text = "Dash is a weapon. Survive.", font_id = "ui-font",
                       color = { r = 150, g = 140, b = 165, a = 255 }, is_fixed = true },
    },
})
registry.set_parent(sub, menu)
ui.anchor(sub, {
    left = 0.5, top = 0.5, right = 0.5, bottom = 0.5,
    offset_left = -200, offset_top = 30, offset_right = 200, offset_bottom = 50,
})
```

---

## Buttons

A clickable button is a layout entity carrying a `ui_button` component. Add an
anchor and it becomes both visible and clickable — `UIButtonSystem` hit-tests
the mouse against the entity's resolved `ui_rect`, so the click region always
matches what layout drew. No collider is involved.

```lua
local btn = load_entity({
    components = {
        square    = { color = { r = 60, g = 40, b = 90, a = 230 } },  -- panel body
        ui_button = {
            is_active = true,
            is_fixed  = true,
            on_click  = function(self, entity)
                self.is_active = false        -- `self` is the ui_button table; guard re-fire
                load_scene("scenes/arena.lua")
            end,
        },
    },
})
registry.set_parent(btn, menu)
ui.anchor(btn, "center", { width = 240, height = 64 })   -- sizes both the panel and the hit region

-- Caption: a label that covers the button rect, with the text centered inside it.
local label = load_entity({
    components = {
        text_label = { text = "Start Run", font_id = "ui-font", is_fixed = true,
                       align = "center", valign = "center" },
    },
})
registry.set_parent(label, btn)
ui.anchor(label, "center", { width = 240, height = 64 })  -- label rect covers the button
ui.z_index(label, 1)                                      -- draw the text above the panel
```

- **No `box_collider` needed.** Hit-testing uses `ui_rect`. A collider on a UI
  button is ignored (and, being world-space, would not follow the anchor
  anyway). Colliders remain the hit path only for world-space buttons that have
  a `transform` instead of a `ui_rect`.
- **`on_click(self, entity)`** — `self` is the `ui_button` table (set extra
  fields on it to read them back here); `entity` is the button entity. Fetch the
  live component elsewhere with `registry.get_ui_button(entity)`.
- **`is_active = false`** disables hit-testing without destroying the entity —
  use it to debounce a click that triggers a scene load mid-frame.
- **Center the caption with `align` / `valign`.** A `text_label` on the UI path
  draws its texture at the rect's top-left by default. Set `align = "center"`
  (also `"right"`) and `valign = "center"` (also `"bottom"`) to position the
  text within its rect — so a label whose rect covers the button appears
  centered. Give the caption a higher `ui.z_index` than the panel, or it sorts
  behind the square at the same layer.

---

## Gotchas

- **No canvas ancestor → no layout.** An anchored entity that is not a
  descendant of a canvas is never resolved.
- **Presets ignore `offset_*`.** Preset opts read only `width` / `height` /
  `margin`; for explicit pixel offsets, use the raw anchor table.
- **Layers accumulate.** A child's layer is the canvas base plus every
  `z_index` above it — set the lift once on the container, not on each leaf.
- **UI entities need no transform.** Canvas and anchored nodes carry no
  position/scale/rotation; the layout system resolves them purely from the
  hierarchy and anchors.

## Related components

| Lua key      | Set by                      | Holds                                             |
|--------------|-----------------------------|---------------------------------------------------|
| `ui_canvas`  | `ui.canvas`                 | Canvas flags + size + base layer.                 |
| `ui_anchor`  | `ui.anchor`                 | The four anchors and four offsets.                |
| `ui_rect`    | `UILayoutSystem` (computed) | Resolved `left/top/right/bottom/layer` in pixels. |
| `ui_z_index` | `ui.z_index`                | Per-entity layer delta.                           |
| `ui_button`  | project scripts             | Interactive state (`registry.get_ui_button`).     |

`ui_rect` is output, not input — read it if you need an element's resolved
screen position, but do not author it by hand.
