// Correctness checks for the Registry's relationship + ChildOf hierarchy machinery. These have
// generation-aware storage and cascade semantics that are easy to regress. gtest-free; exit code =
// failed-check count. Registered with ctest as EcsHierarchyTest. Links the ECS core only.

#include <algorithm>
#include <cmath>

#include "Components/SpriteComponent.h"
#include "ECS/Registry.h"
#include "Systems/PivotResolveSystem.h"
#include "Systems/TransformSystem.h"
#include "TestHarness.h"

using octarine::test::Check;

namespace {
bool ChildrenContain(const std::vector<Entity>& children, const Entity needle) {
  return std::any_of(children.begin(), children.end(), [&](const Entity c) { return c.GetId() == needle.GetId(); });
}

// Composition through a rotation is exact only up to float trig error, so positions that fall out
// of a rotated compose are compared with a tolerance rather than ==.
bool NearVec(const glm::vec2 got, const glm::vec2 want, const float eps = 1e-3f) {
  return std::fabs(got.x - want.x) < eps && std::fabs(got.y - want.y) < eps;
}
}  // namespace

int main() {
  // SetParent wires both directions of the hierarchy and bumps the generation.
  {
    Registry registry;
    const Entity parent = registry.CreateEntity();
    const Entity child = registry.CreateEntity();
    const uint64_t gen0 = registry.HierarchyGeneration();

    registry.SetParent(child, parent);
    Check(registry.HierarchyGeneration() > gen0, "HierarchyGeneration bumps on SetParent");
    const auto resolvedParent = registry.GetParent(child);
    Check(resolvedParent.has_value() && resolvedParent->GetId() == parent.GetId(),
          "GetParent returns the assigned parent");
    Check(ChildrenContain(registry.GetChildren(parent), child), "GetChildren lists the child");
    Check(registry.HasAnyChildPairs(), "HasAnyChildPairs true once a ChildOf relationship exists");

    // SetParent also maintains the legacy ChildOf pair, so HasPair sees it.
    Check(registry.HasPair(child, registry.ChildOfEntity(), parent),
          "SetParent maintains the ChildOf pair for HasPair queries");
  }

  // Reparenting detaches from the previous parent.
  {
    Registry registry;
    const Entity a = registry.CreateEntity();
    const Entity b = registry.CreateEntity();
    const Entity child = registry.CreateEntity();
    registry.SetParent(child, a);
    registry.SetParent(child, b);  // reparent
    Check(registry.GetParent(child)->GetId() == b.GetId(), "reparent updates GetParent");
    Check(!ChildrenContain(registry.GetChildren(a), child), "old parent no longer lists the child");
    Check(ChildrenContain(registry.GetChildren(b), child), "new parent lists the child");
  }

  // Generic pairs: add / has round-trip via the reverse index.
  {
    Registry registry;
    const Entity author = registry.CreateEntity();
    const Entity target = registry.CreateEntity();
    const Entity other = registry.CreateEntity();
    const Entity likes = registry.TagId("likes");

    Check(!registry.HasPair(author, likes, target), "no pair before AddPair");
    registry.AddPair(author, likes, target);
    Check(registry.HasPair(author, likes, target), "HasPair true after AddPair");
    Check(!registry.HasPair(author, likes, other), "HasPair false for a different target");
    Check(registry.HasAnyPairs(), "HasAnyPairs true once any pair exists");
  }

  // Blamming a parent cascade-destroys its children and bumps the generation.
  {
    Registry registry;
    const Entity parent = registry.CreateEntity();
    const Entity child = registry.CreateEntity();
    registry.SetParent(child, parent);
    const uint64_t genBeforeBlam = registry.HierarchyGeneration();

    registry.BlamEntity(parent);
    Check(!registry.IsAlive(parent), "blammed parent is destroyed");
    Check(!registry.IsAlive(child), "child is cascade-destroyed with its parent");
    Check(registry.HierarchyGeneration() > genBeforeBlam, "HierarchyGeneration bumps on cascade blam");
    Check(!registry.GetParent(child).has_value(), "no dangling parent link after cascade blam");
  }

  // TransformSystem hierarchy composition: child globals compose from the parent chain while
  // flat entities resolve global = local in the same frame.
  {
    Registry registry;
    registry.RegisterBulkSystem<GlobalTransformComponent>(TransformSystem());

    const Entity flat = registry.CreateEntityWithBundle(GlobalTransformComponent{}, PositionComponent{{5.0f, 6.0f}});
    const Entity parent = registry.CreateEntityWithBundle(GlobalTransformComponent{}, PositionComponent{{10.0f, 0.0f}},
                                                          ScaleComponent{{2.0f, 2.0f}});
    const Entity child = registry.CreateEntityWithBundle(GlobalTransformComponent{}, PositionComponent{{1.0f, 2.0f}});
    const Entity grandchild =
        registry.CreateEntityWithBundle(GlobalTransformComponent{}, PositionComponent{{1.0f, 0.0f}});
    registry.SetParent(child, parent);
    registry.SetParent(grandchild, child);

    registry.Update(1.0f / 60.0f);

    const auto& flatGlobal = registry.GetComponent<GlobalTransformComponent>(flat);
    Check(flatGlobal.position == glm::vec2(5.0f, 6.0f), "flat entity resolves global = local alongside a hierarchy");

    const auto& parentGlobal = registry.GetComponent<GlobalTransformComponent>(parent);
    Check(parentGlobal.position == glm::vec2(10.0f, 0.0f) && parentGlobal.scale == glm::vec2(2.0f, 2.0f),
          "root resolves global = local");

    // child local (1,2) scaled by parent (2,2) then offset by parent position (10,0) -> (12,4).
    const auto& childGlobal = registry.GetComponent<GlobalTransformComponent>(child);
    Check(childGlobal.position == glm::vec2(12.0f, 4.0f), "child global composes parent position and scale");
    Check(childGlobal.scale == glm::vec2(2.0f, 2.0f), "child inherits parent scale (identity local scale)");

    // grandchild local (1,0) scaled by child global scale (2,2) offset by child global (12,4) -> (14,4).
    const auto& grandchildGlobal = registry.GetComponent<GlobalTransformComponent>(grandchild);
    Check(grandchildGlobal.position == glm::vec2(14.0f, 4.0f), "grandchild composes through two levels");

    // An entity created into an existing archetype after the first Update still gets resolved
    // next frame (the pool-growth case that forbids root caching).
    const Entity late = registry.CreateEntityWithBundle(GlobalTransformComponent{}, PositionComponent{{7.0f, 7.0f}});
    registry.Update(1.0f / 60.0f);
    Check(registry.GetComponent<GlobalTransformComponent>(late).position == glm::vec2(7.0f, 7.0f),
          "entity created after first frame is resolved on the next frame");
  }

  // PivotResolveSystem bakes the normalized anchor into local pixels, and TransformSystem then
  // scales it into the world-space offset every downstream consumer rotates about.
  {
    Registry registry;
    registry.RegisterBulkSystem<RotationComponent>(PivotResolveSystem());
    registry.RegisterBulkSystem<GlobalTransformComponent>(TransformSystem());

    const Entity sprite = registry.CreateEntityWithBundle(
        GlobalTransformComponent{}, PositionComponent{{0.0f, 0.0f}}, ScaleComponent{{2.0f, 2.0f}},
        RotationComponent{0.0, glm::vec2(0.5f, 0.5f)}, SpriteComponent{"tex", 32.0f, 16.0f});

    registry.Update(1.0f / 60.0f);

    Check(registry.GetComponent<RotationComponent>(sprite).pivotOffset == glm::vec2(16.0f, 8.0f),
          "normalized pivot resolves to local pixels from the sprite size");
    Check(registry.GetComponent<GlobalTransformComponent>(sprite).pivot == glm::vec2(32.0f, 16.0f),
          "global pivot is the local offset scaled into world pixels");

    // No size source anywhere on the entity means no anchor to resolve against, so rotation
    // falls back to spinning about `position`.
    const Entity bare = registry.CreateEntityWithBundle(GlobalTransformComponent{}, PositionComponent{{1.0f, 1.0f}},
                                                        RotationComponent{0.0, glm::vec2(0.5f, 0.5f)});
    registry.Update(1.0f / 60.0f);
    Check(registry.GetComponent<GlobalTransformComponent>(bare).pivot == glm::vec2(0.0f, 0.0f),
          "entity with no size source resolves a zero pivot");
  }

  // A parent's rotation acts about its own pivot, so children orbit the point the parent is
  // visibly drawn spinning around rather than its top-left corner.
  {
    Registry registry;
    registry.RegisterBulkSystem<GlobalTransformComponent>(TransformSystem());

    constexpr double kQuarterTurn = 1.5707963267948966;  // pi/2
    // pivotOffset is set directly here: this exercises TransformSystem's composition in
    // isolation, without depending on PivotResolveSystem having run.
    const Entity parent =
        registry.CreateEntityWithBundle(GlobalTransformComponent{}, PositionComponent{{100.0f, 100.0f}},
                                        RotationComponent{kQuarterTurn, glm::vec2(0.0f, 0.0f)});
    registry.GetComponent<RotationComponent>(parent).pivotOffset = glm::vec2(10.0f, 0.0f);

    const Entity atPivot =
        registry.CreateEntityWithBundle(GlobalTransformComponent{}, PositionComponent{{10.0f, 0.0f}});
    const Entity offPivot =
        registry.CreateEntityWithBundle(GlobalTransformComponent{}, PositionComponent{{20.0f, 0.0f}});
    registry.SetParent(atPivot, parent);
    registry.SetParent(offPivot, parent);

    registry.Update(1.0f / 60.0f);

    // A child sitting exactly on the parent's pivot is the rotation's fixed point.
    Check(NearVec(registry.GetComponent<GlobalTransformComponent>(atPivot).position, glm::vec2(110.0f, 100.0f)),
          "child on the parent pivot is unmoved by the parent's rotation");
    // 10 units past the pivot, swung a quarter turn: +x becomes +y.
    Check(NearVec(registry.GetComponent<GlobalTransformComponent>(offPivot).position, glm::vec2(110.0f, 110.0f)),
          "child off the parent pivot orbits that pivot, not the parent's origin");
  }

  // A SetParent cycle must not hang or revisit entities during the level walk.
  {
    Registry registry;
    registry.RegisterBulkSystem<GlobalTransformComponent>(TransformSystem());

    const Entity flat = registry.CreateEntityWithBundle(GlobalTransformComponent{}, PositionComponent{{3.0f, 4.0f}});
    const Entity a = registry.CreateEntityWithBundle(GlobalTransformComponent{}, PositionComponent{{1.0f, 0.0f}});
    const Entity b = registry.CreateEntityWithBundle(GlobalTransformComponent{}, PositionComponent{{2.0f, 0.0f}});
    const Entity c = registry.CreateEntityWithBundle(GlobalTransformComponent{}, PositionComponent{{4.0f, 0.0f}});
    registry.SetParent(b, a);
    registry.SetParent(c, b);
    registry.SetParent(a, c);  // closes the loop — the hierarchy now has no root

    registry.Update(1.0f / 60.0f);

    Check(registry.GetComponent<GlobalTransformComponent>(flat).position == glm::vec2(3.0f, 4.0f),
          "a parent cycle terminates and leaves unrelated entities correct");
  }

  // Reparenting bumps HierarchyGeneration, which is what invalidates the cached depth buckets.
  {
    Registry registry;
    registry.RegisterBulkSystem<GlobalTransformComponent>(TransformSystem());

    const Entity p1 = registry.CreateEntityWithBundle(GlobalTransformComponent{}, PositionComponent{{10.0f, 0.0f}});
    const Entity p2 = registry.CreateEntityWithBundle(GlobalTransformComponent{}, PositionComponent{{50.0f, 0.0f}});
    const Entity child = registry.CreateEntityWithBundle(GlobalTransformComponent{}, PositionComponent{{1.0f, 0.0f}});

    registry.SetParent(child, p1);
    registry.Update(1.0f / 60.0f);
    Check(registry.GetComponent<GlobalTransformComponent>(child).position == glm::vec2(11.0f, 0.0f),
          "child composes under its first parent");

    registry.SetParent(child, p2);
    registry.Update(1.0f / 60.0f);
    Check(registry.GetComponent<GlobalTransformComponent>(child).position == glm::vec2(51.0f, 0.0f),
          "reparenting rebuilds the cached depth buckets");
  }

  // Destroying a hierarchy member must invalidate the cached buckets. If it did not, the stale
  // handle would resolve through GetEntityLocation onto whatever entity recycled that id.
  {
    Registry registry;
    registry.RegisterBulkSystem<GlobalTransformComponent>(TransformSystem());

    const Entity parent = registry.CreateEntityWithBundle(GlobalTransformComponent{}, PositionComponent{{20.0f, 0.0f}});
    const Entity keep = registry.CreateEntityWithBundle(GlobalTransformComponent{}, PositionComponent{{1.0f, 0.0f}});
    const Entity drop = registry.CreateEntityWithBundle(GlobalTransformComponent{}, PositionComponent{{2.0f, 0.0f}});
    registry.SetParent(keep, parent);
    registry.SetParent(drop, parent);

    registry.Update(1.0f / 60.0f);
    Check(registry.GetComponent<GlobalTransformComponent>(keep).position == glm::vec2(21.0f, 0.0f),
          "both children compose before the blam");

    registry.BlamEntity(drop);
    registry.Update(1.0f / 60.0f);
    Check(registry.GetComponent<GlobalTransformComponent>(keep).position == glm::vec2(21.0f, 0.0f),
          "surviving sibling still composes after a hierarchy member is blammed");
  }

  return octarine::test::Result();
}
