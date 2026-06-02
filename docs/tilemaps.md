# Tilemaps

What Octarine does with a scene's `tilemap` today, and — just as important — what
it does **not** yet do. Read this before assuming the engine parses or renders a
tile grid: it does not.

Audience: project authors who see `tilemap` in scene files and want to know what
it actually buys them. Scene file shape and lifecycle live in
[`docs/scenes.md`](scenes.md); the asset/refcount side is in
[`docs/asset-pipeline.md`](asset-pipeline.md).

---

## The `tilemap` field in a scene

A scene table may carry an optional `tilemap` block (see
[`docs/scenes.md`](scenes.md) for the full scene shape):

```lua
return {
    tilemap = {
        texture_asset_id = assets["tilemap-forest"],
        map = "tilemaps/forest.map",
    },
    entities = { --[[ ... ]] },
}
```

## What the engine does with it today

Exactly one thing: **asset tracking for the texture.** When a scene loads,
`SceneAssetScanner` reads `tilemap.texture_asset_id` and adds it to the scene's
asset reference set so the texture is acquired (and refcount-released on swap)
alongside everything else — see `src/AssetManager/SceneAssetScanner.cpp` (the
`tilemap.` branch) and the per-scene tracking in `Game::TrackSceneAssets`.

That is the whole integration. In particular, the engine does **not**:

- **parse the `map` file.** The `map = "tilemaps/forest.map"` path is a plain
  string. No `.map` format is defined, and no loader reads it — the scanner only
  pulls `texture_asset_id` out of the `tilemap` table, never `map`.
- **build a tile grid** or expose any tile-query API (no "tile at (x, y)").
- **render tiles.** `RenderSpriteSystem` draws individual sprite entities; there
  is no tilemap layer/renderer. See [`docs/systems.md`](systems.md).
- **collide against tiles.** `CollisionSystem` operates on `BoxColliderComponent`
  entities, not tiles.

Treat `tilemap.map` as a forward-looking convention a project may adopt: today it
is the project's own responsibility to load and interpret that file (e.g. from a
scene or startup script). There is intentionally no engine `.map` schema to
conform to yet.

## Map dimensions ≠ a tilemap

The one map-shaped runtime surface that *is* wired up is the **playable-area
size**, exposed from Lua and unrelated to any tile grid:

```lua
set_game_map_dimensions(width, height)   -- sets the playable area
local size = get_game_map_dimensions()   -- -> vec2(width, height)
```

These read/write `GameConfig.playableAreaWidth/Height`, which bound
`OffScreenDespawnSystem` (entities outside the area despawn). They define
*world extents for despawn*, not a tile layout. See
`src/Lua/Modules/GameModuleLuaBinding.cpp`.

## Building tiled levels in the meantime

Until a `.map` loader exists, lay out tiled content as ordinary sprite entities
in the scene's `entities` list (one entity per tile, or larger sprites for
backgrounds), reusing the shared `tilemap.texture_asset_id` as each sprite's
texture. The entity-from-table shape is documented in
[`docs/scenes.md`](scenes.md) and [`docs/ecs-components.md`](ecs-components.md).

---

## Where to look in the source

| Concern | File |
|---------|------|
| Scene `tilemap` texture scanning (only field read) | `src/AssetManager/SceneAssetScanner.cpp` |
| Per-scene asset acquire/release | `Game::TrackSceneAssets`, `AssetManager::AcquireAll` / `ReleaseAll` |
| `get_game_map_dimensions` / `set_game_map_dimensions` | `src/Lua/Modules/GameModuleLuaBinding.cpp` |
| Playable-area despawn bounds | `src/Systems/OffScreenDespawnSystem.h`, `GameConfig` |
