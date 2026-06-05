#include <benchmark/benchmark.h>

#include <cstdint>
#include <glm/glm.hpp>
#include <stdexcept>

#include "Components/BoxColliderComponent.h"
#include "Components/EntityMaskComponent.h"
#include "Components/GlobalTransformComponent.h"
#include "ECS/Iterable.h"
#include "ECS/Query.h"
#include "ECS/Registry.h"
#include "Engine/EngineContext.h"
#include "EventBus/EventBus.h"
#include "Events/CollisionBatchEvent.h"
#include "Systems/CollisionSystem.h"

// Micro-bench for the CollisionSystem async-dispatch path. The system hands each detection cycle
// to a worker and polls the returned future on later ticks; the change under test swapped a
// per-cycle std::async(launch::async) (a fresh OS thread spun up and joined every cycle) for a
// submit to the persistent ThreadPool. This drives complete dispatch->collect cycles so the
// per-cycle dispatch overhead is what's timed. At small box counts that overhead dominates the
// (trivial) detection work; at large counts the detection work dominates and any delta shrinks
// toward the noise floor. Compare against the std::async baseline by re-running with the change
// reverted (the build is identical; only CollisionSystem.h differs).

namespace {
class StubContext final : public AnyContext {
 public:
  explicit StubContext(Registry* r) : registry_(r) {}
  [[nodiscard]] Entity GetEntity() const override { return Entity{}; }
  [[nodiscard]] Registry* GetRegistry() const override { return registry_; }
  [[nodiscard]] float GetDeltaTime() const override { return 0.016f; }
  void* GetComponentPtr(EntityID) override { return nullptr; }

 private:
  Registry* registry_;
};

struct CollisionCounter {
  std::uint64_t count = 0;
  // CollisionSystem emits one batched event per collect cycle; the overlapping pair guarantees
  // pairs is non-empty, so count advances once per cycle (what the timing loop below keys on).
  void OnCollisionBatch(const CollisionBatchEvent& e) { count += e.pairs.size(); }
};

// CollisionSystem builds its own query and ignores the Iterable argument, but the call signature
// requires one. begin()/end() are never invoked, so throwing stubs are safe placeholders.
Iterable MakeUnusedIterable() {
  return {[]() -> AnyIterator { throw std::logic_error("unused"); },
          []() -> AnyIterator { throw std::logic_error("unused"); }};
}

// Two boxes overlap at the origin (guaranteeing >=1 collision per cycle, so every collect emits an
// event we can count); the rest sit on a wide diagonal grid, non-overlapping, to keep narrowphase
// cheap and the dispatch overhead visible.
void BuildBoxes(Registry& registry, int n) {
  EntityMask mask;
  mask.set(0);
  for (int i = 0; i < n; ++i) {
    Entity e = registry.CreateEntity();
    const float p = (i < 2) ? 0.0f : static_cast<float>(i) * 64.0f;
    registry.AddComponent(e, GlobalTransformComponent{glm::vec2(p, p), glm::vec2(1.0f, 1.0f), 0.0});
    registry.AddComponent(e, BoxColliderComponent(32, 32, glm::vec2(0.0f, 0.0f), false, mask));
    registry.AddComponent(e, EntityMaskComponent(mask));
  }
}
}  // namespace

static void BM_CollisionDispatch(benchmark::State& state) {
  Registry registry;
  EventBus bus;
  CollisionCounter counter;
  auto subscription =
      bus.SubscribeEvent<CollisionCounter, CollisionBatchEvent>(&counter, &CollisionCounter::OnCollisionBatch);

  EngineContext ec;
  ec.eventBus = &bus;
  registry.Set<EngineContext>(ec);

  BuildBoxes(registry, static_cast<int>(state.range(0)));

  CollisionSystem system;
  StubContext impl(&registry);
  const ContextFacade ctx(&impl);
  const Iterable iter = MakeUnusedIterable();

  // Each timed iteration advances exactly one full dispatch->collect cycle: the system polls the
  // outstanding future (cheap early-returns) until it is ready, collects + emits (bumping the
  // counter), and re-dispatches for the next iteration.
  for (auto _ : state) {
    const std::uint64_t before = counter.count;
    do {
      system(ctx, iter);
    } while (counter.count == before);
  }
  state.SetItemsProcessed(static_cast<int64_t>(state.iterations()));
}
BENCHMARK(BM_CollisionDispatch)->Range(8, 512)->UseRealTime();
