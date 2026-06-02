# Built-in systems

Systems are the logic half of the ECS. Each one runs a query over entities with a given set of
components and does work every frame. This page catalogs the engine's built-in systems and — just as
importantly — the **order they run in**, which is load-bearing for correct behavior.

For how systems are written and the three execution models (serial / parallel / bulk), see
[`ecs-architecture.md`](ecs-architecture.md). For the Lua side, see
[`lua-scripting.md`](lua-scripting.md).

## How systems run

Systems are registered on the `Registry` during engine startup and driven once per frame by
`Registry::Update(dt)`:

- `RegisterSystem<Components...>(fn)` — serial, one call per matching entity.
- `RegisterParallelSystem<Components...>(fn)` — same, but chunks are spread across a thread pool.
  The callback must be data-parallel (no shared mutable state); registry mutations go through an
  `EntityCommandBuffer` played back after the pass.
- `RegisterBulkSystem<Components...>(fn)` — called once per frame with the whole matching set, for
  full-pass work (transforms, collision broadphase).

**Registration order is execution order.** There is no separate scheduling layer — the sequence below
is exactly the order written in `Game::Setup` (which composes the shared boot phases in
`src/Engine/EngineBootstrap.{h,cpp}`). That file is the source of truth; this table mirrors it. A
handful of systems are *not* per-frame: they are event-driven or invoked on demand (see the later
sections).

## Per-frame update order

| # | System | Model · query | What it does |
|--:|--------|---------------|--------------|
| 1 | `ScriptSystem` | serial · `ScriptComponent` | Runs each entity's Lua `on_update` (and `on_debug_gui`) callback. |
| 2 | `AudioSystem` | serial · `AudioSourceComponent` | Drives the mixer / audio-track pool; also handles `AudioPlayEvent` (see below). |
| 3 | `AnimationSystem` | parallel · `SpriteComponent, AnimationComponent` | Advances frame timers and updates the sprite source rect. |
| 4 | `ProjectileLifecycleSystem` | parallel · `ProjectileComponent` | Counts down projectile lifetime and despawns on expiry. |
| 5 | `VelocityIntegrationSystem` | parallel · `PositionComponent, RigidBodyComponent` | Integrates velocity into local position. Runs **before** transform resolution. |
| 6 | `OffScreenDespawnSystem` | parallel · `PositionComponent, SpriteComponent` | Despawns non-player entities that leave the playable bounds. |
| 7 | `TransformSystem` | bulk · `GlobalTransformComponent` (+ optional position/scale/rotation) | Resolves the entity hierarchy into world-space `GlobalTransformComponent`. Fast path when no `ChildOf` relationships exist. |
| 8 | `CollisionSystem` | bulk · `GlobalTransformComponent, BoxColliderComponent, EntityMaskComponent` | Broadphase + OBB narrowphase; **emits `CollisionEvent`** for each overlapping pair. |
| 9 | `UpdateListenerTransformSystem` | bulk · `GlobalTransformComponent, AudioListenerComponent` | Snapshots the active listener's position/velocity for the spatial-audio chain. |
| 10 | `AudioCullingSystem` | serial · `GlobalTransformComponent, AudioSourceComponent` | Gates spatial sources by listener radius (adds/removes the active tag + sink). |
| 11 | `SpatialAudioSystem` | serial · `GlobalTransformComponent, AudioSourceComponent, AudioSinkComponent` | Distance attenuation + stereo pan for active spatial sources. |
| 12 | `DopplerSystem` | serial · `GlobalTransformComponent, RigidBodyComponent, AudioSourceComponent, AudioSinkComponent` | Doppler pitch shift from relative emitter/listener velocity. |
| 13 | `CameraFollowSystem` | serial · `PositionComponent, CameraFollowComponent` | Moves the camera viewport to follow its target within bounds. |
| 14 | `RenderSpriteSystem` | parallel · `GlobalTransformComponent, SpriteComponent` | Resolves textures and enqueues visible sprites into the render queue (viewport-culled). |
| 15 | `RenderTextSystem` | serial · `TextLabelComponent` | Rasterizes/caches glyphs and enqueues visible text (viewport-culled). |
| 16 | `RenderPrimitiveSystem` | parallel · `SquarePrimitiveComponent, GlobalTransformComponent` | Enqueues square primitives (viewport-culled). |

The render systems (14–16) only *produce* render-queue entries; `Game::Render` sorts the queue and
draws it after `Update` (see [`ecs-architecture.md`](ecs-architecture.md) § Rendering).

### Why the order matters

- **Velocity (5) → Transform (7) → Collision (8) / Render (14–16):** local position must be integrated
  before transforms resolve, and transforms must be world-space before collision and rendering read
  them.
- **Listener (9) → cull (10) → spatial (11) → doppler (12):** the spatial-audio chain depends on the
  current listener snapshot, then progressively narrows to the sources that need full processing.

## Event-driven systems

These are created and subscribed during setup but do **not** run on a per-frame query — they react to
events on the [event bus](events.md):

| System | Reacts to | What it does |
|--------|-----------|--------------|
| `DamageSystem` | `CollisionEvent` | Applies projectile damage / health deduction and despawns. |
| `ObstacleBounceSystem` | `CollisionEvent` | Bounces an entity off an obstacle and flips its sprite. |
| `UIButtonSystem` | `MouseInputEvent` | Hit-tests clicks against button colliders and invokes callbacks. |

## Services and on-demand systems

Not per-frame query systems, but part of the engine:

| System | How it's used | Lua surface |
|--------|---------------|-------------|
| `InputSystem` | Installed at boot; subscribes to the input events and tracks per-frame key/mouse/wheel state and action bindings. | `input.*` (held/pressed/released, actions) |
| `EntityPoolManager` (`EntityPoolSystem.h`) | Object pool for recycling entities (e.g. projectiles); used by `ProjectileEmitSystem`. | — |
| `ProjectileEmitSystem` | Installed at boot; spawns projectiles on demand, driven by the Lua `fire_projectile` global. | via `fire_projectile` |
| `DrawColliderSystem` | Instantiated per frame in `Game::Render` only when the `drawColliders` option is on. Draws collider wireframes. | debug (ImGui builds) |
| `RenderDebugGUISystem` | Called from `Game::Render`; renders the ImGui editor/profiler/hierarchy. Compiled out unless `OCTARINE_WITH_IMGUI`. | editor UI |

## Adding a system

`src/Systems/<Name>System.{h,cpp}` (a `.cpp` also goes in the `octarine_systems` list in
`src/CMakeLists.txt`), then register it in `Game::Setup` at the right point in the order above. If it
needs a Lua surface, add a `LuaSystemBinding` (see [`lua-scripting.md`](lua-scripting.md)); if it
reacts to events, subscribe in its `Init` / `SubscribeToEvents` hook (see [`events.md`](events.md)).
