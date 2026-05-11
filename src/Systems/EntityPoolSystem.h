#pragma once

#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../ECS/ECS.h"
#include "../ECS/Registry.h"
#include "../General/PerfUtils.h"

// Pool manager that recycles entities of a registered archetype shape via the active/inactive
// chunk partition — entities never cross archetypes when parked or unparked. Storage stays in
// place; only the active_count cursor moves. RegisterPool eagerly materializes one sample so
// the pool's archetype is reachable from despawn-time routing without a string lookup.
//
// Factories MUST attach PoolableTag to entities they produce, otherwise the despawn dispatch in
// Registry::Update will Blam them outright instead of routing them back here.
class EntityPoolManager {
 public:
  using Resetter = std::function<void(Registry&, Entity)>;
  using Factory = std::function<Entity(Registry&)>;

  // Resetter type pack Ts... lists components that accumulate state across an entity's life
  // (timers, flags) and must be re-zeroed on Spawn. PoolableTag has no payload, so don't list it.
  template <typename... Ts>
  ArchetypeID RegisterPool(Registry& registry, Factory factory) {
    const Entity sample = factory(registry);
    const ArchetypeID arch_id = registry.GetArchetypeID(sample);
    registry.Deactivate(sample);

    pools_[arch_id] = PoolDef{
        std::move(factory),
        [](Registry& reg, const Entity e) { ((reg.GetComponent<Ts>(e) = Ts{}), ...); },
    };
    free_lists_[arch_id].push_back(sample);
    return arch_id;
  }

  // Pull an entity from the pool, calling the registered factory if no parked entity is
  // available. Activates the entity (moving it back into the chunk's active prefix) and runs
  // the resetter to clear accumulated state. Throws if pool_id is not a registered pool —
  // that's a programmer error, not a runtime condition.
  Entity Spawn(Registry& registry, const ArchetypeID pool_id) {
    const auto pool_it = pools_.find(pool_id);
    if (pool_it == pools_.end()) {
      throw std::runtime_error("EntityPoolManager::Spawn: pool archetype id " + std::to_string(pool_id) +
                               " is not registered. Call RegisterPool first.");
    }
    if (const auto bucket_it = free_lists_.find(pool_id); bucket_it != free_lists_.end()) {
      auto& list = bucket_it->second;
      while (!list.empty()) {
        const Entity entity = list.back();
        list.pop_back();
        if (!registry.IsAlive(entity)) continue;
        registry.Activate(entity);
        pool_it->second.resetter(registry, entity);
        PROFILE_COUNTER_ADD("Pool: Spawn (reused)", 1);
        return entity;
      }
    }
    PROFILE_COUNTER_ADD("Pool: Spawn (factory)", 1);
    return pool_it->second.factory(registry);
  }

  // Push a live entity into its pool. Called from the despawn-playback loop in Registry::Update,
  // never from gameplay code. The entity's natural archetype id (the active shape) is the pool
  // key — no string lookup, no payload read off the entity.
  void Park(Registry& registry, const Entity entity) {
    if (!registry.IsAlive(entity)) return;
    const ArchetypeID arch_id = registry.GetArchetypeID(entity);
    registry.Deactivate(entity);
    free_lists_[arch_id].push_back(entity);
    PROFILE_COUNTER_ADD("Pool: Park", 1);
  }

 private:
  struct PoolDef {
    Factory factory;
    Resetter resetter;
  };

  std::unordered_map<ArchetypeID, std::vector<Entity>> free_lists_;
  std::unordered_map<ArchetypeID, PoolDef> pools_;
};
