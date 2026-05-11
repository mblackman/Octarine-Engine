#include <benchmark/benchmark.h>

#include "ECS/Registry.h"
#include "Systems/EntityPoolSystem.h"

struct Position {
  float x = 0.0f;
  float y = 0.0f;
};

struct Velocity {
  float dx = 0.0f;
  float dy = 0.0f;
};

// Benchmark standard Create/Blam without pooling
static void BM_EntityCreateAndBlam(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    Registry registry;
    state.ResumeTiming();

    for (int i = 0; i < state.range(0); ++i) {
      Entity e = registry.CreateEntity();
      registry.AddComponent(e, Position{1.0f, 1.0f});
      registry.AddComponent(e, Velocity{1.0f, 1.0f});
      registry.QueueBlamEntity(e);
    }

    registry.Update(1.0f / 60.0f);  // Process blams
  }
}
BENCHMARK(BM_EntityCreateAndBlam)->Range(8, 8192);

// Benchmark pooling system
static void BM_EntityPoolSpawnAndPark(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    Registry registry;
    EntityPoolManager pool;

    // Register the pool and get its ID
    ArchetypeID pool_id = pool.RegisterPool<Position, Velocity>(registry, [](Registry& reg) {
      Entity e = reg.CreateEntity();
      reg.AddComponent(e, Position{0.0f, 0.0f});
      reg.AddComponent(e, Velocity{0.0f, 0.0f});
      reg.AddTag<PoolableTag>(e);
      return e;
    });

    // Pre-warm the pool so we are only benchmarking the activation/deactivation cost
    std::vector<Entity> entities;
    entities.reserve(state.range(0));
    for (int i = 0; i < state.range(0); ++i) {
      entities.push_back(pool.Spawn(registry, pool_id));
    }
    for (Entity e : entities) {
      pool.Park(registry, e);
    }
    entities.clear();
    state.ResumeTiming();

    // The actual benchmark: Spawn (recycle) and Park
    for (int i = 0; i < state.range(0); ++i) {
      entities.push_back(pool.Spawn(registry, pool_id));
    }
    for (Entity e : entities) {
      pool.Park(registry, e);
    }
  }
}
BENCHMARK(BM_EntityPoolSpawnAndPark)->Range(8, 8192);
