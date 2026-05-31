# ECS Architectural Overview

Octarine Engine features a custom, archetype-based Entity-Component System (ECS) designed for high performance, cache
efficiency, and developer flexibility. This document outlines the internal architecture, data layout, and capabilities
of the system.

## Core Concepts

The ECS is built around four primary pillars:

- **Entities:** Unique identifiers that represent individual "things" in the game world.
- **Components:** Plain data-only structures (PODs) that define the properties of an entity.
- **Archetypes:** Groupings of entities that share the exact same set of components.
- **Systems:** Logic that operates on sets of entities by querying for specific component combinations.

---

## Data Layout: Archetype-Based SoA

The defining characteristic of Octarine's ECS is its **archetype-based** storage. Instead of storing components as
objects in a list, they are organized by their composition.

### Archetypes

When an entity is created with a specific set of components (e.g., `Transform`, `Sprite`, `RigidBody`), it is assigned
to an `Archetype`. All entities with this exact signature are stored together.

Adding or removing a component at runtime triggers an **archetype transition**, moving the entity's data to a new
archetype.

### Chunks and SoA

Within an Archetype, data is stored in fixed-size **Chunks** (default 16KB). Each chunk uses a
**Structure of Arrays (SoA)** layout:

```text
Chunk (16KB)
[ EntityID Array ] [ ComponentA Array ] [ ComponentB Array ] ... [ Padding ]
```

**Why this design?**

1. **Cache Locality:** Systems usually only need a subset of an entity's components. By storing each component type
   contiguously, the CPU cache is utilized much more efficiently during iteration.
2. **Predictable Memory:** Chunks provide stable memory regions, making parallel processing safe and reducing allocation
   fragmentation.
3. **Efficient Querying:** Finding entities is a matter of identifying archetypes that contain the required components,
   then iterating over their chunks.

---

## Entities and Registry

### Entity Representation

An `Entity` is a 64-bit handle composed of:

- **ID (32-bit):** The base index in the entity pool.
- **Generation (32-bit):** Incremented whenever an ID is recycled, preventing stale handles from accessing new entities.

### The Registry

The `Registry` is the central manager for the entire ECS. It handles:

- Entity creation and destruction (`CreateEntity`, `BlamEntity`).
- Component registration and assignment.
- Archetype graph management.
- System orchestration.
- Singleton components (global state).

---

## Querying and Systems

### Component Queries

Systems interact with data via `ComponentQuery<TComponents...>`. A query identifies all archetypes
that are a **superset** of the requested components.

Queries are cached; they only re-evaluate the archetype graph when the registry's `archetype_generation` increases.

### Iteration Models

Octarine supports three primary ways to process entities:

1. **Serial Systems (`RegisterSystem`):**
   Standard iteration on the main thread.
   ```cpp
   registry.RegisterSystem<Transform, RigidBody>([](Entity e, Transform& t, RigidBody& rb) {
       t.position += rb.velocity;
   });
   ```

2. **Parallel Systems (`RegisterParallelSystem`):**
   Distributes chunks across a `ThreadPool`. Since chunks are independent memory regions, this is safe for per-entity
   independent writes.
   *Note: Use a `CommandBuffer` to queue registry modifications during parallel updates.*

3. **Bulk Systems (`RegisterBulkSystem`):**
   Provides access to raw `Iterable` blocks. This is used for advanced optimizations where you might want to process
   entire chunks at once or handle complex logic across multiple entities.

---

## Advanced Capabilities

### Relationships and Hierarchy

The ECS supports **Pairs** to model relationships between entities:

- **Pairs:** Represented as `(Relationship, Target)`.
- **Hierarchy:** Built using the `ChildOf` relationship. The `TransformSystem` handles nested
  transforms efficiently via this mechanism.

### Command Buffers

To prevent iterator invalidation and race conditions, use `CommandBuffer` to defer structural changes (like destroying
an entity) until the end of a system's update.

### Singleton Components

For global data like `AssetManager` or `GameConfig`, the Registry allows setting "Singletons":

```cpp
registry.Set<AssetManager>(std::move(assets));
AssetManager& assets = registry.Get<AssetManager>();
```

### Tags

Tags are zero-size components used as flags. They are memory-efficient because they only affect which archetype an
entity belongs to, without occupying space in the chunk's component arrays.

---

## Building on the ECS

### Adding a New Component

Define a simple struct in `src/Components/`. It should be move-constructible.

```cpp
struct Health {
    int current;
    int max;
};
```

If the component is exposed to Lua via a `LuaBinding<T>` specialization, any
methods bound through `bindUsertype` may read or write the component's own
fields only — no `Registry&`, no `EventBus*`, no cross-entity touches, no
allocation. Anything beyond that belongs in a system. See [Lua scripting:
adding component methods](lua-scripting.md#8-adding-component-methods).

### Creating a System

Implement your logic in `src/Systems/` and register it in `Game::Setup()`.

- Use `RegisterSystem` for most logic.
- Use `RegisterParallelSystem` for heavy computations (physics, animations).
- Use `RegisterBulkSystem` if you need to optimize for chunk-level processing.

### Performance Tips

- **Bundle Creation:** Use `CreateEntityWithBundle(T... components)` to avoid multiple archetype transitions during
  entity initialization.
- **Minimize Transitions:** Adding/removing components in a tight loop is expensive. Prefer using tags or updating
  existing component data.
- **Prefer Parallelism:** For systems operating on many entities with no cross-entity dependencies, use parallel systems
  to utilize all CPU cores.
