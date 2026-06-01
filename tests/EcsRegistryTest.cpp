// Correctness checks for the ECS Registry — the spine every system rides on. Archetype-migration
// bugs corrupt component data silently, so these assert the round-trips and transitions directly.
// gtest-free; exit code = failed-check count. Registered with ctest as EcsRegistryTest.
//
// Links the ECS core only (Registry.cpp + Logger.cpp) — no SDL/window — exactly like the
// benchmark target. Uses local POD component structs (same approach as EntityPoolBenchmark.cpp)
// so the test is independent of the real Components/ headers.

#include "ECS/Query.h"  // full ComponentQuery definition for CreateQuery / ForEach
#include "ECS/Registry.h"
#include "TestHarness.h"

using octarine::test::Check;

namespace {
struct Position {
  float x = 0.0f;
  float y = 0.0f;
};
struct Velocity {
  float dx = 0.0f;
  float dy = 0.0f;
};
struct Health {
  int hp = 0;
};
}  // namespace

int main() {
  // Add / Has / Get round-trip.
  {
    Registry registry;
    const Entity e = registry.CreateEntity();
    Check(registry.IsAlive(e), "freshly created entity is alive");
    Check(!registry.HasComponent<Position>(e), "no component before AddComponent");

    registry.AddComponent(e, Position{1.0f, 2.0f});
    Check(registry.HasComponent<Position>(e), "HasComponent true after AddComponent");
    Check(registry.GetComponent<Position>(e).x == 1.0f && registry.GetComponent<Position>(e).y == 2.0f,
          "GetComponent round-trips the stored value");
  }

  // Archetype migration: adding a second component preserves the first; removing one keeps the other.
  {
    Registry registry;
    const Entity e = registry.CreateEntity();
    registry.AddComponent(e, Position{3.0f, 4.0f});
    registry.AddComponent(e, Velocity{5.0f, 6.0f});  // archetype {Position} -> {Position, Velocity}
    Check(registry.HasComponent<Position>(e) && registry.HasComponent<Velocity>(e),
          "entity holds both components after second add");
    Check(registry.GetComponent<Position>(e).x == 3.0f, "first component value survives archetype migration");
    Check(registry.GetComponent<Velocity>(e).dx == 5.0f, "second component value placed correctly");

    registry.RemoveComponent<Position>(e);  // {Position, Velocity} -> {Velocity}
    Check(!registry.HasComponent<Position>(e), "removed component is gone");
    Check(registry.HasComponent<Velocity>(e) && registry.GetComponent<Velocity>(e).dy == 6.0f,
          "untouched component intact after a removal migration");
  }

  // Re-adding an existing component assigns over the live slot (no duplicate, value replaced).
  {
    Registry registry;
    const Entity e = registry.CreateEntity();
    registry.AddComponent(e, Position{1.0f, 1.0f});
    const ArchetypeID before = registry.GetArchetypeID(e);
    registry.AddComponent(e, Position{9.0f, 9.0f});  // re-add path
    Check(registry.GetComponent<Position>(e).x == 9.0f, "re-add assigns over the existing slot");
    Check(registry.GetArchetypeID(e) == before, "re-add does not change the archetype");
  }

  // CreateEntityWithBundle lands in the same end-state as incremental adds.
  {
    Registry registry;
    const Entity bundled = registry.CreateEntityWithBundle(Position{7.0f, 8.0f}, Velocity{1.0f, 2.0f});
    Check(registry.HasComponent<Position>(bundled) && registry.HasComponent<Velocity>(bundled),
          "bundle create lands all components");
    Check(registry.GetComponent<Position>(bundled).y == 8.0f, "bundle component value correct");

    const Entity incremental = registry.CreateEntity();
    registry.AddComponent(incremental, Position{0.0f, 0.0f});
    registry.AddComponent(incremental, Velocity{0.0f, 0.0f});
    Check(registry.GetArchetypeID(bundled) == registry.GetArchetypeID(incremental),
          "bundle create reaches the same archetype as incremental adds");
  }

  // Blam dedup + deferred destruction via Update.
  {
    Registry registry;
    const Entity e = registry.CreateEntity();
    registry.AddComponent(e, Position{0.0f, 0.0f});
    registry.QueueBlamEntity(e);
    registry.QueueBlamEntity(e);  // duplicate within the frame — must collapse to one blam
    Check(registry.IsAlive(e), "queued blam is deferred until Update");
    registry.Update(1.0f / 60.0f);
    Check(!registry.IsAlive(e), "entity is destroyed after Update processes the blam queue");
    Check(!registry.HasComponent<Position>(e), "destroyed entity reports no components");
    registry.Update(1.0f / 60.0f);  // second Update must not double-process the (now empty) queue
    Check(!registry.IsAlive(e), "stale handle stays invalid across further updates");
  }

  // Queries visit exactly the matching set.
  {
    Registry registry;
    for (int i = 0; i < 3; ++i) {
      const Entity e = registry.CreateEntity();
      registry.AddComponent(e, Position{static_cast<float>(i), 0.0f});
    }
    for (int i = 0; i < 2; ++i) {
      const Entity e = registry.CreateEntity();
      registry.AddComponent(e, Velocity{0.0f, 0.0f});  // no Position — must be skipped
    }
    int seen = 0;
    const auto query = registry.CreateQuery<Position>();
    query->ForEach([&](Position&) { ++seen; });
    Check(seen == 3, "query<Position> visits exactly the entities that have Position");
  }

  // Activate / Deactivate move across the active partition without crossing archetypes.
  {
    Registry registry;
    const Entity e = registry.CreateEntity();
    registry.AddComponent(e, Position{0.0f, 0.0f});
    const ArchetypeID archetype = registry.GetArchetypeID(e);
    Check(registry.IsActive(e), "new entity starts active");

    registry.Deactivate(e);
    Check(!registry.IsActive(e) && registry.IsAlive(e), "deactivated entity is inactive but still alive");
    Check(registry.GetArchetypeID(e) == archetype, "deactivate does not change the archetype");
    int seenAfterDeactivate = 0;
    const auto query = registry.CreateQuery<Position>();
    query->ForEach([&](Position&) { ++seenAfterDeactivate; });
    Check(seenAfterDeactivate == 0, "default query skips deactivated entities");

    registry.Activate(e);
    Check(registry.IsActive(e), "reactivated entity is active again");
  }

  // Tags: string and typed, add / has / remove.
  {
    Registry registry;
    const Entity e = registry.CreateEntity();
    registry.AddTag(e, "enemy");
    Check(registry.HasTag(e, "enemy"), "string tag present after AddTag");
    Check(!registry.HasTag(e, "boss"), "unrelated string tag absent");
    registry.RemoveTag(e, "enemy");
    Check(!registry.HasTag(e, "enemy"), "string tag gone after RemoveTag");

    struct Frozen {};
    registry.AddTag<Frozen>(e);
    Check(registry.HasTag<Frozen>(e), "typed tag present after AddTag<T>");
    registry.RemoveTag<Frozen>(e);
    Check(!registry.HasTag<Frozen>(e), "typed tag gone after RemoveTag<T>");
  }

  // Singletons: Set / Get / TryGet.
  {
    Registry registry;
    Check(registry.TryGet<Health>() == nullptr, "TryGet returns nullptr before Set");
    registry.Set<Health>(Health{42});
    Check(registry.Get<Health>().hp == 42, "Get returns the Set value");
    Check(registry.TryGet<Health>() != nullptr && registry.TryGet<Health>()->hp == 42,
          "TryGet returns the live singleton after Set");
  }

  // Archetype generation bumps on a new shape, stays stable for a repeated shape.
  {
    Registry registry;
    const Entity a = registry.CreateEntity();
    const uint64_t gen0 = registry.ArchetypeGeneration();
    registry.AddComponent(a, Position{0.0f, 0.0f});  // creates archetype {Position}
    const uint64_t gen1 = registry.ArchetypeGeneration();
    Check(gen1 > gen0, "ArchetypeGeneration bumps when a new archetype is created");

    const Entity b = registry.CreateEntity();
    registry.AddComponent(b, Position{0.0f, 0.0f});  // same shape — no new archetype
    Check(registry.ArchetypeGeneration() == gen1, "ArchetypeGeneration stable for a repeated shape");
  }

  return octarine::test::Result();
}
