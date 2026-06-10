// Correctness checks for the Registry's relationship + ChildOf hierarchy machinery. These have
// generation-aware storage and cascade semantics that are easy to regress. gtest-free; exit code =
// failed-check count. Registered with ctest as EcsHierarchyTest. Links the ECS core only.

#include <algorithm>
#include <cmath>

#include "ECS/Registry.h"
#include "Systems/TransformSystem.h"
#include "TestHarness.h"

using octarine::test::Check;

namespace {
bool ChildrenContain(const std::vector<Entity>& children, const Entity needle) {
  return std::any_of(children.begin(), children.end(), [&](const Entity c) { return c.GetId() == needle.GetId(); });
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

  return octarine::test::Result();
}
