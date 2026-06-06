// Integration tests for CollisionSystem's enter/exit filtering (W2.2).
//
// CollisionSystem is async: operator() on call N gathers the current entity positions and
// starts BVH detection; call N+1 consumes that result and emits CollisionBatchEvent. A 20 ms
// sleep between Update() calls gives the async work time to complete (it finishes in µs for
// 2-3 entities). Because the detection result is one frame behind the gather, each scenario
// requires an extra tick to flush the pipeline.
//
// gtest-free; exit code = failed-check count. Links the ECS core only.

#include <chrono>
#include <glm/glm.hpp>
#include <thread>
#include <utility>
#include <vector>

#include "Components/BoxColliderComponent.h"
#include "Components/EntityMaskComponent.h"
#include "Components/GlobalTransformComponent.h"
#include "ECS/Query.h"  // full ComponentQuery definition for RegisterBulkSystem dispatch
#include "ECS/Registry.h"
#include "Engine/EngineContext.h"
#include "EventBus/EventBus.h"
#include "Events/CollisionBatchEvent.h"
#include "Systems/CollisionSystem.h"
#include "TestHarness.h"

using octarine::test::Check;
using octarine::test::CheckEq;

namespace {

// Accumulates copies of every CollisionBatchEvent pair list received (pairs is a borrowed ref
// valid only during dispatch, so it must be copied here).
struct CollisionCapture {
  std::vector<std::vector<std::pair<Entity, Entity>>> batches;
  EventBus::SubscriptionHandle subscription;

  void OnBatch(const CollisionBatchEvent& evt) { batches.emplace_back(evt.pairs); }

  int TotalPairs() const {
    int n = 0;
    for (const auto& b : batches) n += static_cast<int>(b.size());
    return n;
  }

  bool Contains(const Entity a, const Entity b) const {
    for (const auto& batch : batches) {
      for (const auto& [x, y] : batch) {
        if ((x == a && y == b) || (x == b && y == a)) return true;
      }
    }
    return false;
  }
};

struct TestScene {
  Registry reg;
  std::unique_ptr<EventBus> bus;
  CollisionCapture capture;

  TestScene() : bus(std::make_unique<EventBus>()) {
    EngineContext ctx;
    ctx.eventBus = bus.get();
    reg.Set<EngineContext>(ctx);
    reg.RegisterBulkSystem(CollisionSystem());
    capture.subscription =
        bus->SubscribeEvent<CollisionCapture, CollisionBatchEvent>(&capture, &CollisionCapture::OnBatch);
  }

  // Create a 32×32 AABB at top-left (x, y) with default masks (all entities interact).
  Entity MakeBox(const float x, const float y) {
    const Entity e = reg.CreateEntity();
    reg.AddComponent(e, GlobalTransformComponent{glm::vec2(x, y), glm::vec2(1.0f, 1.0f), 0.0});
    reg.AddComponent(e, BoxColliderComponent{32, 32});
    reg.AddComponent(e, EntityMaskComponent{});
    return e;
  }

  void MoveTo(const Entity e, const float x, const float y) {
    reg.GetComponent<GlobalTransformComponent>(e).position = glm::vec2(x, y);
  }

  // Advance one engine tick. CollisionSystem starts async detection this call;
  // the resulting pairs are emitted on the NEXT call, so most scenarios need an
  // extra trailing tick to flush the pipeline.
  void Tick() {
    reg.Update(1.0f / 60.0f);
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }
};

}  // namespace

int main() {
  // --- Entering pair is emitted on first contact.
  // Boxes at (0,0) and (16,0) with size 32 overlap by 16 px.
  {
    TestScene scene;
    const Entity a = scene.MakeBox(0.0f, 0.0f);
    const Entity b = scene.MakeBox(16.0f, 0.0f);

    scene.Tick();  // gathers overlapping boxes, starts detection
    scene.Tick();  // consumes result, emits (a, b) as entering

    CheckEq(scene.capture.TotalPairs(), 1, "entering: overlapping pair emitted on first contact");
    Check(scene.capture.Contains(a, b), "entering: correct pair (a, b) in the batch");
  }

  // --- Non-overlapping entities produce no pairs.
  // Boxes at (0,0) and (64,0): gap between maxX=32 and minX=64.
  {
    TestScene scene;
    scene.MakeBox(0.0f, 0.0f);
    scene.MakeBox(64.0f, 0.0f);

    scene.Tick();
    scene.Tick();

    CheckEq(scene.capture.TotalPairs(), 0, "no overlap: no pairs emitted");
  }

  // --- Sustained overlap is suppressed after the first contact.
  // Both boxes remain in the same position across three detection cycles; the pair
  // should appear in exactly one batch (the first).
  {
    TestScene scene;
    const Entity a = scene.MakeBox(0.0f, 0.0f);
    const Entity b = scene.MakeBox(16.0f, 0.0f);

    scene.Tick();  // gather + start detection_1 (overlap)
    scene.Tick();  // emit (a,b) entering; gather + start detection_2 (still overlap)
    scene.Tick();  // emit [] sustained; gather + start detection_3

    CheckEq(scene.capture.TotalPairs(), 1,
            "sustained: pair emitted exactly once despite two consecutive overlapping cycles");
  }

  // --- Re-entry fires the pair again after a gap.
  // Sequence: overlap → separate → overlap. The pair should appear in two separate batches.
  // Timing: detection_N consumes boxes gathered at call N, emitted at call N+1. Moving b
  // between calls N and N+1 affects detection_N+1 (gathered at call N+1).
  {
    TestScene scene;
    const Entity a = scene.MakeBox(0.0f, 0.0f);
    const Entity b = scene.MakeBox(16.0f, 0.0f);

    scene.Tick();                  // detection_1 gathers: a-b overlap
    scene.Tick();                  // emit (a,b) entering; detection_2 gathers: still overlap
    scene.MoveTo(b, 64.0f, 0.0f);  // move b apart before detection_3 gathers
    scene.Tick();                  // emit [] sustained (det_2 result); detection_3 gathers: b at 64 — no overlap
    scene.MoveTo(b, 16.0f, 0.0f);  // move b back before detection_4 gathers
    scene.Tick();                  // emit [] (det_3 no-overlap); detection_4 gathers: b at 16 — overlap
    scene.Tick();                  // emit (a,b) entering again (det_4 result; prevPairSet_ was cleared)

    CheckEq(scene.capture.TotalPairs(), 2, "re-entry: pair emitted on first contact and again after separation");
    Check(scene.capture.Contains(a, b), "re-entry: (a, b) is in the captured batches");
  }

  // --- New pair alongside a sustained pair: only the new pair fires.
  // a and b overlap from the start. c is far away, then moves to overlap a only (not b).
  // After c enters, only the (a, c) pair should appear — (a, b) must not re-fire.
  //
  // Layout: a at (0,0), b at (16,0) [overlaps a], c starts at (200,0), moves to (-20,0).
  // c at (-20,0): AABB [-20,12] overlaps a [0,32] but not b [16,48].
  {
    TestScene scene;
    const Entity a = scene.MakeBox(0.0f, 0.0f);
    const Entity b = scene.MakeBox(16.0f, 0.0f);
    const Entity c = scene.MakeBox(200.0f, 0.0f);

    scene.Tick();                   // detection_1: a-b overlap, a-c no
    scene.Tick();                   // emit (a,b) entering; detection_2: same positions
    scene.MoveTo(c, -20.0f, 0.0f);  // c now overlaps a but not b
    scene.Tick();                   // emit [] (det_2 a-b sustained); detection_3 gathers: a-b + a-c overlap
    scene.Tick();                   // emit entering pairs from det_3: only (a,c) — (a,b) already in prevPairSet_

    CheckEq(scene.capture.TotalPairs(), 2,
            "new pair: (a,b) on first entry, (a,c) when c enters — (a,b) not re-emitted");
    Check(scene.capture.Contains(a, b), "new pair: initial (a,b) entry was captured");
    Check(scene.capture.Contains(a, c), "new pair: (a,c) entry captured when c moved in");
  }

  return octarine::test::Result();
}
