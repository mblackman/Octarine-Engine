// Correctness checks for the Registry's relationship + ChildOf hierarchy machinery. These have
// generation-aware storage and cascade semantics that are easy to regress. gtest-free; exit code =
// failed-check count. Registered with ctest as EcsHierarchyTest. Links the ECS core only.

#include <algorithm>
#include <cmath>

#include "Components/PivotComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/SquarePrimitiveComponent.h"
#include "ECS/Registry.h"
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

  // TransformSystem resolves the normalized anchor against the entity's size and scales it into
  // the world-space offset every downstream consumer rotates about.
  {
    Registry registry;
    registry.RegisterBulkSystem<GlobalTransformComponent>(TransformSystem());

    const Entity sprite = registry.CreateEntityWithBundle(
        GlobalTransformComponent{}, PositionComponent{{0.0f, 0.0f}}, ScaleComponent{{2.0f, 2.0f}},
        PivotComponent{glm::vec2(0.5f, 0.5f)}, SpriteComponent{"tex", 32.0f, 16.0f});

    registry.Update(1.0f / 60.0f);

    // 0.5 of a 32x16 sprite is (16, 8) in local pixels, doubled into world pixels by the scale.
    Check(registry.GetComponent<GlobalTransformComponent>(sprite).pivot == glm::vec2(32.0f, 16.0f),
          "normalized pivot resolves against the sprite size and scales into world pixels");

    // No size source anywhere on the entity means no anchor to resolve against, so rotation
    // falls back to spinning about `position`.
    const Entity bare = registry.CreateEntityWithBundle(GlobalTransformComponent{}, PositionComponent{{1.0f, 1.0f}},
                                                        PivotComponent{glm::vec2(0.5f, 0.5f)});
    registry.Update(1.0f / 60.0f);
    Check(registry.GetComponent<GlobalTransformComponent>(bare).pivot == glm::vec2(0.0f, 0.0f),
          "entity with no size source resolves a zero pivot");

    // A pivot is independent of rotation now: this entity has no RotationComponent at all, and
    // its scale still anchors to the authored corner rather than the geometry centre.
    const Entity unrotated = registry.CreateEntityWithBundle(
        GlobalTransformComponent{}, PositionComponent{{50.0f, 50.0f}}, ScaleComponent{{3.0f, 3.0f}},
        PivotComponent{glm::vec2(0.0f, 0.0f)}, SpriteComponent{"tex", 32.0f, 16.0f});
    registry.Update(1.0f / 60.0f);
    Check(NearVec(registry.GetComponent<GlobalTransformComponent>(unrotated).position, glm::vec2(50.0f, 50.0f)),
          "a pivot with no rotation component still anchors scale");
  }

  // A parent's rotation acts about its own pivot, so children orbit the point the parent is
  // visibly drawn spinning around rather than its top-left corner.
  {
    Registry registry;
    registry.RegisterBulkSystem<GlobalTransformComponent>(TransformSystem());

    constexpr float kQuarterTurn = 1.5707964F;  // pi/2
    // A 10x20 primitive anchored at (1, 0) puts the parent's anchor 10px right of its origin.
    const Entity parent = registry.CreateEntityWithBundle(
        GlobalTransformComponent{}, PositionComponent{{100.0f, 100.0f}}, RotationComponent{kQuarterTurn},
        PivotComponent{glm::vec2(1.0f, 0.0f)}, SquarePrimitiveComponent{glm::vec2(0.0f, 0.0f), 0, 10.0f, 20.0f});

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

  // A grouping node — a local transform with no GlobalTransformComponent, which is what the Lua
  // loader produces for an entity that carries no renderable — still drives its children. Its
  // local stands in as its global instead of collapsing to identity at the world origin.
  {
    Registry registry;
    registry.RegisterBulkSystem<GlobalTransformComponent>(TransformSystem());

    constexpr float kQuarterTurn = 1.5707964F;  // pi/2
    const Entity group =
        registry.CreateEntityWithBundle(PositionComponent{{200.0f, 300.0f}}, RotationComponent{kQuarterTurn});
    const Entity child = registry.CreateEntityWithBundle(GlobalTransformComponent{}, PositionComponent{{10.0f, 0.0f}});
    registry.SetParent(child, group);

    registry.Update(1.0f / 60.0f);

    const auto& childGlobal = registry.GetComponent<GlobalTransformComponent>(child);
    // Zero pivot, so the child orbits the group's origin: +x swings to +y a quarter turn round.
    Check(NearVec(childGlobal.position, glm::vec2(200.0f, 310.0f)),
          "child of a global-less grouping node inherits its position and rotation");
    Check(std::fabs(childGlobal.rotation - kQuarterTurn) < 1e-6F,
          "child of a global-less grouping node inherits its rotation");
  }

  // The same node in the middle of a chain has to pass its ancestors' composition through, not
  // just its own local — nothing writes it back to a component, so the pass carries it in memory.
  {
    Registry registry;
    registry.RegisterBulkSystem<GlobalTransformComponent>(TransformSystem());

    const Entity root = registry.CreateEntityWithBundle(GlobalTransformComponent{}, PositionComponent{{100.0f, 0.0f}},
                                                        ScaleComponent{{2.0f, 2.0f}});
    const Entity group = registry.CreateEntityWithBundle(PositionComponent{{10.0f, 0.0f}});
    const Entity child = registry.CreateEntityWithBundle(GlobalTransformComponent{}, PositionComponent{{1.0f, 0.0f}});
    registry.SetParent(group, root);
    registry.SetParent(child, group);

    registry.Update(1.0f / 60.0f);

    // group sits at 100 + 10*2 = 120; child adds its own local scaled by the inherited (2,2).
    const auto& childGlobal = registry.GetComponent<GlobalTransformComponent>(child);
    Check(NearVec(childGlobal.position, glm::vec2(122.0f, 0.0f)),
          "a global-less node mid-chain passes its ancestors' composition through");
    Check(childGlobal.scale == glm::vec2(2.0f, 2.0f), "scale survives a global-less node mid-chain");
  }

  // Scale acts about the entity's anchor. With no authored pivot that is the centre of its own
  // geometry, so a scaled sprite grows in place instead of sprawling down-right from its corner.
  {
    Registry registry;
    registry.RegisterBulkSystem<GlobalTransformComponent>(TransformSystem());

    const Entity grown =
        registry.CreateEntityWithBundle(GlobalTransformComponent{}, PositionComponent{{100.0f, 100.0f}},
                                        ScaleComponent{{2.0f, 2.0f}}, SpriteComponent{"tex", 32.0f, 16.0f});
    // Scale 1 must be a no-op: nothing unscaled anywhere in a game may shift because of this.
    const Entity unscaled =
        registry.CreateEntityWithBundle(GlobalTransformComponent{}, PositionComponent{{100.0f, 100.0f}},
                                        ScaleComponent{{1.0f, 1.0f}}, SpriteComponent{"tex", 32.0f, 16.0f});

    registry.Update(1.0f / 60.0f);

    const auto& g = registry.GetComponent<GlobalTransformComponent>(grown);
    Check(NearVec(g.position, glm::vec2(84.0f, 92.0f)), "unpivoted scale backs the top-left off by half the growth");
    Check(NearVec(g.position + glm::vec2(32.0f, 16.0f) * g.scale * 0.5f, glm::vec2(116.0f, 108.0f)),
          "the sprite centre is exactly where it sat before scaling");
    Check(NearVec(registry.GetComponent<GlobalTransformComponent>(unscaled).position, glm::vec2(100.0f, 100.0f)),
          "scale 1 leaves position untouched");
  }

  // An authored pivot overrides the centre — including restoring the old grow-from-the-corner
  // behaviour by anchoring at (0, 0).
  {
    Registry registry;
    registry.RegisterBulkSystem<GlobalTransformComponent>(TransformSystem());

    const Entity corner = registry.CreateEntityWithBundle(
        GlobalTransformComponent{}, PositionComponent{{100.0f, 100.0f}}, ScaleComponent{{2.0f, 2.0f}},
        PivotComponent{glm::vec2(0.0f, 0.0f)}, SpriteComponent{"tex", 32.0f, 16.0f});
    const Entity farCorner = registry.CreateEntityWithBundle(
        GlobalTransformComponent{}, PositionComponent{{100.0f, 100.0f}}, ScaleComponent{{2.0f, 2.0f}},
        PivotComponent{glm::vec2(1.0f, 1.0f)}, SpriteComponent{"tex", 32.0f, 16.0f});

    registry.Update(1.0f / 60.0f);

    Check(NearVec(registry.GetComponent<GlobalTransformComponent>(corner).position, glm::vec2(100.0f, 100.0f)),
          "a top-left pivot scales from the corner, leaving position untouched");
    // Anchor (32, 16) held fixed: the top-left backs off by the full extra size.
    const auto& far = registry.GetComponent<GlobalTransformComponent>(farCorner);
    Check(NearVec(far.position, glm::vec2(68.0f, 84.0f)), "a bottom-right pivot scales from that corner");
    Check(NearVec(far.position + far.pivot, glm::vec2(132.0f, 116.0f)),
          "the anchored corner itself does not move under scale");
  }

  // A child grows about its own anchor using the scale it inherited, while the parent's scale
  // still places it. The two anchor terms are independent.
  {
    Registry registry;
    registry.RegisterBulkSystem<GlobalTransformComponent>(TransformSystem());

    // No geometry on the parent, so its own anchor is zero and only the child's applies.
    const Entity parent = registry.CreateEntityWithBundle(GlobalTransformComponent{}, PositionComponent{{100.0f, 0.0f}},
                                                          ScaleComponent{{2.0f, 2.0f}});
    const Entity child =
        registry.CreateEntityWithBundle(GlobalTransformComponent{}, PositionComponent{{10.0f, 0.0f}},
                                        SquarePrimitiveComponent{glm::vec2(0.0f, 0.0f), 0, 32.0f, 16.0f});
    registry.SetParent(child, parent);

    registry.Update(1.0f / 60.0f);

    const auto& g = registry.GetComponent<GlobalTransformComponent>(child);
    Check(g.scale == glm::vec2(2.0f, 2.0f), "child inherits the parent scale");
    // The parent's scale doubles the whole subtree about the parent's anchor at (100, 0). The
    // child's unscaled box spans [110, 142] in world x, so doubling puts its corner at 120.
    Check(NearVec(g.position, glm::vec2(120.0f, 0.0f)), "parent scale places the child's corner");
    // Its anchor — the centre of a 32x16 square — is at (10 + 16) * 2 from the parent origin.
    Check(NearVec(g.position + glm::vec2(32.0f, 16.0f) * g.scale * 0.5f, glm::vec2(152.0f, 16.0f)),
          "the child's centre lands on its placed anchor");
  }

  // A child that carries both a scale and a pivot, under a rotating parent. The anchor offset
  // is a vector in the child's own frame, so it has to travel through the parent's rotation with
  // everything else — adding it unrotated puts the child badly out of place at any parent angle.
  {
    Registry registry;
    registry.RegisterBulkSystem<GlobalTransformComponent>(TransformSystem());

    constexpr float kQuarterTurn = 1.5707964F;  // pi/2
    const Entity parent = registry.CreateEntityWithBundle(
        GlobalTransformComponent{}, PositionComponent{{100.0f, 100.0f}}, RotationComponent{kQuarterTurn});
    const Entity child = registry.CreateEntityWithBundle(
        GlobalTransformComponent{}, PositionComponent{{50.0f, 0.0f}}, ScaleComponent{{2.0f, 2.0f}},
        PivotComponent{glm::vec2(0.5f, 0.5f)}, SquarePrimitiveComponent{glm::vec2(0.0f, 0.0f), 0, 40.0f, 20.0f});
    registry.SetParent(child, parent);

    registry.Update(1.0f / 60.0f);

    const auto& g = registry.GetComponent<GlobalTransformComponent>(child);
    // The child's anchor sits at (50, 0) + (20, 10) = (70, 10) in the parent's frame. A quarter
    // turn about the parent's own origin swings that to (-10, 70), landing it at (90, 170).
    Check(NearVec(g.position + g.pivot, glm::vec2(90.0f, 170.0f)),
          "a scaled, pivoted child anchors where the parent's rotation carries it");
    Check(NearVec(g.position, glm::vec2(50.0f, 150.0f)), "its drawn corner backs off by the scaled anchor");
    Check(NearVec(g.pivot, glm::vec2(40.0f, 20.0f)), "its pivot is the anchor at the inherited scale");

    // The anchor is the fixed point of the child's own scale, so changing scale must not move it.
    registry.GetComponent<ScaleComponent>(child).value = glm::vec2(5.0f, 5.0f);
    registry.Update(1.0f / 60.0f);
    const auto& scaled = registry.GetComponent<GlobalTransformComponent>(child);
    Check(NearVec(scaled.position + scaled.pivot, glm::vec2(90.0f, 170.0f)),
          "rescaling a pivoted child under a rotated parent holds its anchor still");
  }

  return octarine::test::Result();
}
