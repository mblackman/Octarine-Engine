// Correctness checks for the Registry's relationship + ChildOf hierarchy machinery. These have
// generation-aware storage and cascade semantics that are easy to regress. gtest-free; exit code =
// failed-check count. Registered with ctest as EcsHierarchyTest. Links the ECS core only.

#include <algorithm>

#include "ECS/Registry.h"
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

  return octarine::test::Result();
}
