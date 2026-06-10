#include <benchmark/benchmark.h>

#include <algorithm>
#include <random>
#include <string>
#include <vector>

#include "ECS/Query.h"
#include "ECS/Registry.h"
#include "Systems/TransformSystem.h"

// ECS-core microbenchmarks: query iteration (serial / facade / parallel), query re-matching
// under archetype churn, archetype transitions, random component access, transform hierarchy.
// These pin down the per-frame costs of the ECS itself, independent of any gameplay system.

namespace {

struct CorePos {
  float x = 0.0f;
  float y = 0.0f;
};

struct CoreVel {
  float dx = 0.0f;
  float dy = 0.0f;
};

// Monotonic counter so every churn benchmark iteration mints a brand-new tag → a brand-new
// archetype, which is what bumps Registry::ArchetypeGeneration and forces query re-matching.
int g_unique_tag_counter = 0;

// Spread N entities across `archetypes` distinct shapes (same components, different tags) so
// iteration benchmarks exercise the multi-archetype path rather than one giant archetype.
std::vector<Entity> PopulateSpread(Registry& registry, int count, int archetypes) {
  std::vector<Entity> entities;
  entities.reserve(count);
  for (int i = 0; i < count; ++i) {
    Entity e = registry.CreateEntityWithBundle(CorePos{1.0f, 2.0f}, CoreVel{0.5f, 0.25f});
    registry.AddTag(e, "spread_" + std::to_string(i % archetypes));
    entities.push_back(e);
  }
  return entities;
}

}  // namespace

// --- Query iteration -------------------------------------------------------------------------

static void BM_QueryForEachSerial(benchmark::State& state) {
  Registry registry;
  PopulateSpread(registry, static_cast<int>(state.range(0)), 8);
  auto query = registry.CreateQuery<CorePos, CoreVel>();
  query->Update();

  for (auto _ : state) {
    query->ForEach([](const Entity /*e*/, CorePos& p, const CoreVel& v) {
      p.x += v.dx;
      p.y += v.dy;
    });
  }
  state.SetItemsProcessed(state.iterations() * state.range(0));
}
BENCHMARK(BM_QueryForEachSerial)->Range(64, 65536);

static void BM_QueryForEachFacade(benchmark::State& state) {
  Registry registry;
  PopulateSpread(registry, static_cast<int>(state.range(0)), 8);
  auto query = registry.CreateQuery<CorePos, CoreVel>();
  query->Update();

  for (auto _ : state) {
    query->ForEach([](const ContextFacade& /*ctx*/, const Entity /*e*/, CorePos& p, const CoreVel& v) {
      p.x += v.dx;
      p.y += v.dy;
    });
  }
  state.SetItemsProcessed(state.iterations() * state.range(0));
}
BENCHMARK(BM_QueryForEachFacade)->Range(64, 65536);

static void BM_QueryParallelForEach(benchmark::State& state) {
  Registry registry;
  PopulateSpread(registry, static_cast<int>(state.range(0)), 8);
  auto query = registry.CreateQuery<CorePos, CoreVel>();
  query->Update();

  for (auto _ : state) {
    query->ParallelForEach([](const Entity /*e*/, CorePos& p, const CoreVel& v) {
      p.x += v.dx;
      p.y += v.dy;
    });
  }
  state.SetItemsProcessed(state.iterations() * state.range(0));
}
BENCHMARK(BM_QueryParallelForEach)->Range(64, 65536);

// --- Query re-matching under archetype churn -------------------------------------------------

// Control: cost of minting one new archetype (entity + unique tag) with zero live queries.
// Subtract this from BM_QueryRematchChurn to isolate the query re-match cascade.
static void BM_ArchetypeChurnNoQueries(benchmark::State& state) {
  Registry registry;
  PopulateSpread(registry, static_cast<int>(state.range(0)), static_cast<int>(state.range(0)));

  for (auto _ : state) {
    Entity e = registry.CreateEntityWithBundle(CorePos{}, CoreVel{});
    registry.AddTag(e, "churn_" + std::to_string(g_unique_tag_counter++));
  }
}
BENCHMARK(BM_ArchetypeChurnNoQueries)->Range(64, 1024)->Iterations(512);

// The pathology: 16 live queries (≈ registered systems) each re-match against the full
// archetype list every time one new archetype appears anywhere.
static void BM_QueryRematchChurn(benchmark::State& state) {
  Registry registry;
  PopulateSpread(registry, static_cast<int>(state.range(0)), static_cast<int>(state.range(0)));

  constexpr int kQueryCount = 16;
  std::vector<std::unique_ptr<ComponentQuery<CorePos, CoreVel>>> queries;
  queries.reserve(kQueryCount);
  for (int q = 0; q < kQueryCount; ++q) {
    queries.push_back(registry.CreateQuery<CorePos, CoreVel>());
    queries.back()->Update();
  }

  for (auto _ : state) {
    Entity e = registry.CreateEntityWithBundle(CorePos{}, CoreVel{});
    registry.AddTag(e, "churn_" + std::to_string(g_unique_tag_counter++));
    for (auto& query : queries) {
      query->Update();
    }
  }
}
BENCHMARK(BM_QueryRematchChurn)->Range(64, 1024)->Iterations(512);

// --- Archetype transitions --------------------------------------------------------------------

// Hot-edge cycling: after the first lap both archetypes and their add/remove edges exist, so
// this measures the steady-state move cost (CopyComponents + swap-with-last + location patch).
static void BM_ArchetypeTransitionAddRemove(benchmark::State& state) {
  Registry registry;
  std::vector<Entity> entities;
  entities.reserve(state.range(0));
  for (int i = 0; i < state.range(0); ++i) {
    entities.push_back(registry.CreateEntityWithBundle(CorePos{}));
  }

  for (auto _ : state) {
    for (const Entity e : entities) {
      registry.AddComponent(e, CoreVel{1.0f, 1.0f});
    }
    for (const Entity e : entities) {
      registry.RemoveComponent<CoreVel>(e);
    }
  }
  state.SetItemsProcessed(state.iterations() * state.range(0) * 2);
}
BENCHMARK(BM_ArchetypeTransitionAddRemove)->Range(64, 8192);

// --- Random component access ------------------------------------------------------------------

static void BM_GetComponentRandomAccess(benchmark::State& state) {
  Registry registry;
  auto entities = PopulateSpread(registry, static_cast<int>(state.range(0)), 8);
  std::mt19937 rng(42);
  std::shuffle(entities.begin(), entities.end(), rng);

  for (auto _ : state) {
    float sum = 0.0f;
    for (const Entity e : entities) {
      sum += registry.GetComponent<CorePos>(e).x;
    }
    benchmark::DoNotOptimize(sum);
  }
  state.SetItemsProcessed(state.iterations() * state.range(0));
}
BENCHMARK(BM_GetComponentRandomAccess)->Range(64, 65536);

// --- TransformSystem: flat vs poisoned-by-small-hierarchy -------------------------------------

namespace {

Registry* SetupTransformRegistry(Registry& registry, int flatCount) {
  registry.RegisterBulkSystem<GlobalTransformComponent>(TransformSystem());
  for (int i = 0; i < flatCount; ++i) {
    registry.CreateEntityWithBundle(GlobalTransformComponent{},
                                    PositionComponent{glm::vec2(static_cast<float>(i), 0.0f)});
  }
  return &registry;
}

}  // namespace

static void BM_TransformAllFlat(benchmark::State& state) {
  Registry registry;
  SetupTransformRegistry(registry, static_cast<int>(state.range(0)));

  for (auto _ : state) {
    registry.Update(1.0f / 60.0f);
  }
  state.SetItemsProcessed(state.iterations() * state.range(0));
}
BENCHMARK(BM_TransformAllFlat)->Range(64, 65536);

// A 16-entity parent-chain on top of N flat entities. Today one ChildOf pair anywhere drops
// the whole population onto the serial hierarchical path — this measures that cliff.
static void BM_TransformSmallHierarchy(benchmark::State& state) {
  Registry registry;
  SetupTransformRegistry(registry, static_cast<int>(state.range(0)));

  constexpr int kHierarchySize = 16;
  Entity parent = registry.CreateEntityWithBundle(GlobalTransformComponent{}, PositionComponent{glm::vec2(1.0f, 1.0f)});
  for (int i = 1; i < kHierarchySize; ++i) {
    Entity child =
        registry.CreateEntityWithBundle(GlobalTransformComponent{}, PositionComponent{glm::vec2(1.0f, 1.0f)});
    registry.SetParent(child, parent);
    parent = child;
  }

  for (auto _ : state) {
    registry.Update(1.0f / 60.0f);
  }
  state.SetItemsProcessed(state.iterations() * (state.range(0) + kHierarchySize));
}
BENCHMARK(BM_TransformSmallHierarchy)->Range(64, 65536);

// --- Entity creation: bundle vs per-component chain -------------------------------------------

static void BM_CreateEntityWithBundle(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    Registry registry;
    state.ResumeTiming();

    for (int i = 0; i < state.range(0); ++i) {
      registry.CreateEntityWithBundle(CorePos{1.0f, 1.0f}, CoreVel{1.0f, 1.0f});
    }
  }
  state.SetItemsProcessed(state.iterations() * state.range(0));
}
BENCHMARK(BM_CreateEntityWithBundle)->Range(64, 8192);

static void BM_CreateEntityAddChain(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    Registry registry;
    state.ResumeTiming();

    for (int i = 0; i < state.range(0); ++i) {
      Entity e = registry.CreateEntity();
      registry.AddComponent(e, CorePos{1.0f, 1.0f});
      registry.AddComponent(e, CoreVel{1.0f, 1.0f});
    }
  }
  state.SetItemsProcessed(state.iterations() * state.range(0));
}
BENCHMARK(BM_CreateEntityAddChain)->Range(64, 8192);
