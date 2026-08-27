#pragma once

#include <cmath>
#include <cstdint>
#include <glm/glm.hpp>
#include <limits>
#include <memory>
#include <unordered_set>
#include <vector>

#include "Components/BoxColliderComponent.h"
#include "Components/GlobalTransformComponent.h"
#include "Components/PivotComponent.h"
#include "Components/PositionComponent.h"
#include "Components/RotationComponent.h"
#include "Components/ScaleComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/SquarePrimitiveComponent.h"
#include "ECS/Iterable.h"
#include "ECS/Query.h"
#include "ECS/Registry.h"
#include "General/PerfUtils.h"
#include "General/Rotation2D.h"
#include "Systems/LocalSize.h"

class TransformSystem {
 public:
  void operator()(const ContextFacade& ctx, const Iterable& /*iter*/) {
    auto* registry = ctx.GetRegistry();
    EnsureInitialized(registry);
    optionalQuery_->Update();

    // The flat pass is unconditional: it finalises every entity outside a hierarchy, and it
    // leaves hierarchy roots holding their local transform already resolved about their own
    // anchor — a root has nothing above it, so that composition is final and the descend pass
    // reads it back as-is.
    UpdateFlat();

    if (registry->HasAnyChildPairs()) {
      UpdateHierarchical(registry);
    }
  }

 private:
  using TransformQuery = ComponentQuery<GlobalTransformComponent, Opt<PositionComponent>, Opt<ScaleComponent>,
                                        Opt<RotationComponent>, Opt<PivotComponent>, Opt<SpriteComponent>,
                                        Opt<SquarePrimitiveComponent>, Opt<BoxColliderComponent>>;

  struct LocalTransform {
    glm::vec2 position{0.0f, 0.0f};
    glm::vec2 scale{1.0f, 1.0f};
    float rotation{0.0f};
    glm::vec2 anchor{0.0f, 0.0f};  // what scale and rotation act about, unscaled local pixels
  };

  struct GlobalTransform {
    glm::vec2 position{0.0f, 0.0f};
    glm::vec2 scale{1.0f, 1.0f};
    float rotation{0.0f};
    glm::vec2 pivot{0.0f, 0.0f};  // world pixels, offset from position
  };

  // Slot of a hierarchy root, which owns no entry in children_ of its own.
  static constexpr uint32_t kNoParentSlot = std::numeric_limits<uint32_t>::max();

  // One parent paired with the contiguous span of its children inside children_. Grouping by
  // parent is what makes the descend pass cheap: the parent's global is resolved once and reused
  // across every child in the span, instead of being re-resolved per child. parentSlot indexes
  // children_ for the parent itself, where the previous level left its composed global.
  struct ParentGroup {
    Entity parent{};
    uint32_t parentSlot{kNoParentSlot};
    uint32_t firstChild{0};
    uint32_t childCount{0};
  };

  // An entity in the breadth-first frontier, with the slot it was assigned in children_.
  struct FrontierNode {
    Entity entity{};
    uint32_t slot{kNoParentSlot};
  };

  void EnsureInitialized(Registry* registry) {
    if (optionalQuery_) return;
    optionalQuery_ = registry->CreateQuery<GlobalTransformComponent, Opt<PositionComponent>, Opt<ScaleComponent>,
                                           Opt<RotationComponent>, Opt<PivotComponent>, Opt<SpriteComponent>,
                                           Opt<SquarePrimitiveComponent>, Opt<BoxColliderComponent>>();
    posEntity_ = registry->Component<PositionComponent>();
    scaleEntity_ = registry->Component<ScaleComponent>();
    rotEntity_ = registry->Component<RotationComponent>();
    pivotEntity_ = registry->Component<PivotComponent>();
    globalEntity_ = registry->Component<GlobalTransformComponent>();
    spriteEntity_ = registry->Component<SpriteComponent>();
    squareEntity_ = registry->Component<SquarePrimitiveComponent>();
    colliderEntity_ = registry->Component<BoxColliderComponent>();
  }

  // The per-entity body is a handful of copies, so thread-pool dispatch (~13.5 us fixed) only
  // pays for itself near the measured serial/parallel crossover of ~16k entities. Half that,
  // to stay parallel where the win is real and serial where dispatch dominates.
  static constexpr size_t kFlatSerialBelowEntities = 8192;

  // Each entity's global is its local (identity for missing slots), in a single parallel pass.
  // Final for everything outside a hierarchy, and the seed value for hierarchy roots.
  void UpdateFlat() {
    PROFILE_NAMED_SCOPE("TransformSystem: Flat");
    optionalQuery_->ParallelForEach(
        [](GlobalTransformComponent& global, const PositionComponent* p, const ScaleComponent* s,
           const RotationComponent* r, const PivotComponent* pivot, const SpriteComponent* sprite,
           const SquarePrimitiveComponent* square, const BoxColliderComponent* collider) {
          const glm::vec2 scale = s ? s->value : glm::vec2(1.0f, 1.0f);
          const glm::vec2 anchor = ResolveAnchor(pivot, sprite, square, collider);
          global.position = (p ? p->value : glm::vec2(0.0f, 0.0f)) + AnchorScaleShift(anchor, scale);
          global.scale = scale;
          global.rotation = r ? r->value : 0.0f;
          global.pivot = anchor * scale;
        },
        kFlatSerialBelowEntities);
  }

  // A normalized pivot resolved against the entity's own size. No pivot means the centre, which
  // is what the default (0.5, 0.5) resolves to anyway, so the two cases share one expression.
  static glm::vec2 ResolveAnchor(const PivotComponent* pivot, const SpriteComponent* sprite,
                                 const SquarePrimitiveComponent* square, const BoxColliderComponent* collider) {
    const glm::vec2 normalized = pivot ? pivot->value : glm::vec2(PivotComponent::kDefaultX, PivotComponent::kDefaultY);
    return octarine::LocalSize(sprite, square, collider) * normalized;
  }

  // Scaling about the anchor is the same as sliding the origin by this, then scaling about the
  // origin. Every consumer reads `position` as the top-left and multiplies its own offsets by
  // `scale`, so folding it in here fixes the sprite quad, primitive quad, and collider at once.
  static glm::vec2 AnchorScaleShift(const glm::vec2 anchor, const glm::vec2 scale) {
    return anchor * (glm::vec2(1.0f, 1.0f) - scale);
  }

  // Hierarchy members are re-composed in breadth-first depth order. Every entity at depth N is
  // written before any entity at depth N+1 is read, so a parent's global is always final by the
  // time its children consume it — no recursion, no per-entity stack, and no revisiting.
  void UpdateHierarchical(const Registry* registry) {
    PROFILE_NAMED_SCOPE("TransformSystem: Hierarchical");
    RebuildLevelsIfStale(registry);
    Descend(registry);
  }

  // The depth buckets depend only on hierarchy topology, which the registry versions for us, so
  // they survive across frames and are rebuilt only when a SetParent / detach / blam changes the
  // shape. Entity locations are deliberately not cached — an archetype transition moves an
  // entity without touching the topology, so those are resolved fresh each frame.
  //
  // Holding entity handles across frames is safe here on two counts. Every entity that can land
  // in a bucket either has a parent or has children, so destroying one always sets the
  // registry's hierarchyMutated flag and bumps the generation. And destruction is deferred to
  // the end of Update, after every system has run, so a bucket can never go stale mid-pass —
  // which matters because GetEntityLocation keys on the 32-bit id and would happily resolve a
  // recycled handle to whatever entity now occupies that slot.
  void RebuildLevelsIfStale(const Registry* registry) {
    const uint64_t generation = registry->HierarchyGeneration();
    if (levelsBuilt_ && generation == cachedHierarchyGeneration_) return;

    PROFILE_NAMED_SCOPE("TransformSystem: Rebuild levels");
    levels_.clear();
    children_.clear();
    visited_.clear();

    std::vector<FrontierNode> frontier;
    registry->ForEachHierarchyRoot([&](const Entity root) {
      if (visited_.insert(root.id).second) frontier.push_back({root, kNoParentSlot});
    });

    while (!frontier.empty()) {
      frontier = BuildLevel(registry, frontier);
    }

    // Written at depth N before any depth N+1 group reads it, so it never needs clearing.
    childGlobals_.resize(children_.size());

    cachedHierarchyGeneration_ = generation;
    levelsBuilt_ = true;
  }

  // Append one depth bucket for the given parents and return the next frontier (their children).
  std::vector<FrontierNode> BuildLevel(const Registry* registry, const std::vector<FrontierNode>& parents) {
    std::vector<ParentGroup> level;
    std::vector<FrontierNode> next;

    for (const FrontierNode& parent : parents) {
      const auto first = static_cast<uint32_t>(children_.size());
      registry->ForEachChild(parent.entity, [&](const Entity child) {
        // A SetParent cycle would otherwise loop forever here. visited_ also keeps an entity
        // reachable by two routes from being composed twice.
        if (!visited_.insert(child.id).second) return;
        next.push_back({child, static_cast<uint32_t>(children_.size())});
        children_.push_back(child);
      });
      if (const auto count = static_cast<uint32_t>(children_.size()) - first; count > 0) {
        level.push_back({parent.entity, parent.slot, first, count});
      }
    }

    if (!level.empty()) levels_.push_back(std::move(level));
    return next;
  }

  // Globals thread down through childGlobals_ rather than being read back out of the components,
  // so a grouping node with no GlobalTransformComponent still passes composition to its children.
  // Only the write-out is gated on the component existing.
  void Descend(const Registry* registry) {
    PROFILE_NAMED_SCOPE("TransformSystem: Descend");
    for (const auto& level : levels_) {
      for (const ParentGroup& group : level) {
        const GlobalTransform parentGlobal = group.parentSlot == kNoParentSlot ? LoadRootGlobal(registry, group.parent)
                                                                               : childGlobals_[group.parentSlot];
        // Every child in the group shares this one parent, so its cos/sin pair is resolved once
        // per family rather than once per child. This is the payoff for grouping by parent: a
        // hundred-child emitter costs one trig pair instead of a hundred.
        const octarine::Rotation2D parentRotation = octarine::Rotation2D::FromRadians(parentGlobal.rotation);
        for (uint32_t i = 0; i < group.childCount; ++i) {
          const uint32_t slot = group.firstChild + i;
          childGlobals_[slot] = ComposeChild(registry, children_[slot], parentGlobal, parentRotation);
        }
      }
    }
  }

  GlobalTransform ComposeChild(const Registry* registry, const Entity child, const GlobalTransform& parentGlobal,
                               const octarine::Rotation2D parentRotation) const {
    const auto [archetype, chunkIdx, indexInChunk] = registry->GetEntityLocation(child);
    if (!archetype) return {};
    const GlobalTransform global = Compose(parentGlobal, parentRotation, LoadLocal(archetype, chunkIdx, indexInChunk));
    WriteGlobal(archetype, chunkIdx, indexInChunk, global);
    return global;
  }

  // Whatever the flat pass finalised. A root with no GlobalTransformComponent was never in that
  // query, so its local stands in — otherwise a grouping node's own transform would be dropped.
  GlobalTransform LoadRootGlobal(const Registry* registry, const Entity root) const {
    const auto [archetype, chunkIdx, indexInChunk] = registry->GetEntityLocation(root);
    if (!archetype) return {};
    if (!archetype->HasComponent(globalEntity_.GetId())) {
      return FromLocal(LoadLocal(archetype, chunkIdx, indexInChunk));
    }
    const auto* gArr = archetype->GetComponentArray<GlobalTransformComponent>(chunkIdx, globalEntity_.GetId());
    const auto& g = gArr[indexInChunk];
    return {g.position, g.scale, g.rotation, g.pivot};
  }

  // Mirrors the flat pass: with nothing above it, a local transform is already its own global.
  static GlobalTransform FromLocal(const LocalTransform& local) {
    return {local.position + AnchorScaleShift(local.anchor, local.scale), local.scale, local.rotation,
            local.anchor * local.scale};
  }

  // Per-entity chunk fetch: each component is independently optional, so we look up the
  // archetype's array per id and default missing slots to identity.
  LocalTransform LoadLocal(Archetype* archetype, size_t chunkIdx, size_t indexInChunk) const {
    const auto* pArr = archetype->HasComponent(posEntity_.GetId())
                           ? archetype->GetComponentArray<PositionComponent>(chunkIdx, posEntity_.GetId())
                           : nullptr;
    const auto* sArr = archetype->HasComponent(scaleEntity_.GetId())
                           ? archetype->GetComponentArray<ScaleComponent>(chunkIdx, scaleEntity_.GetId())
                           : nullptr;
    const auto* rArr = archetype->HasComponent(rotEntity_.GetId())
                           ? archetype->GetComponentArray<RotationComponent>(chunkIdx, rotEntity_.GetId())
                           : nullptr;
    const auto* pivotArr = archetype->HasComponent(pivotEntity_.GetId())
                               ? archetype->GetComponentArray<PivotComponent>(chunkIdx, pivotEntity_.GetId())
                               : nullptr;
    return {
        pArr ? pArr[indexInChunk].value : glm::vec2(0.0f, 0.0f),
        sArr ? sArr[indexInChunk].value : glm::vec2(1.0f, 1.0f),
        rArr ? rArr[indexInChunk].value : 0.0f,
        LoadAnchor(archetype, chunkIdx, indexInChunk, pivotArr ? &pivotArr[indexInChunk] : nullptr),
    };
  }

  // Hierarchy-path counterpart to ResolveAnchor. The flat pass gets geometry from its query; here
  // each size component costs an archetype probe, paid only by entities inside a hierarchy.
  glm::vec2 LoadAnchor(Archetype* archetype, size_t chunkIdx, size_t indexInChunk, const PivotComponent* pivot) const {
    const auto* spriteArr = archetype->HasComponent(spriteEntity_.GetId())
                                ? archetype->GetComponentArray<SpriteComponent>(chunkIdx, spriteEntity_.GetId())
                                : nullptr;
    const auto* squareArr =
        archetype->HasComponent(squareEntity_.GetId())
            ? archetype->GetComponentArray<SquarePrimitiveComponent>(chunkIdx, squareEntity_.GetId())
            : nullptr;
    const auto* colliderArr =
        archetype->HasComponent(colliderEntity_.GetId())
            ? archetype->GetComponentArray<BoxColliderComponent>(chunkIdx, colliderEntity_.GetId())
            : nullptr;
    return ResolveAnchor(pivot, spriteArr ? &spriteArr[indexInChunk] : nullptr,
                         squareArr ? &squareArr[indexInChunk] : nullptr,
                         colliderArr ? &colliderArr[indexInChunk] : nullptr);
  }

  // Compose parent x local into world space. The parent's scale and rotation act about the
  // parent's own pivot — the same point the renderer spins its quad around — so a child orbits
  // where the parent visibly turns. A zero pivot collapses this to a plain parent-origin orbit,
  // which is what the pre-pivot engine did.
  //
  // Known limit: decomposed TRS is not closed under non-uniform scale combined with rotation.
  // A parent with scale (2, 1) rotated 45 degrees should shear its children; storing scale as a
  // vector cannot express that, so the shear is dropped. Representing it would require a full
  // affine matrix, which SDL_RenderTextureRotated cannot consume anyway.
  static GlobalTransform Compose(const GlobalTransform& parent, const octarine::Rotation2D parentRotation,
                                 const LocalTransform& local) {
    const glm::vec2 scale = parent.scale * local.scale;
    const glm::vec2 pivotWorld = parent.position + parent.pivot;
    // Place the anchor, not the corner: the anchor offset is a vector in the child's own frame,
    // so it has to travel through the parent's scale and rotation like everything else. Composing
    // the corner and adding an unrotated shift afterwards breaks at any parent angle or scale.
    const glm::vec2 anchorWorld = octarine::RotateAround(
        parent.position + ((local.position + local.anchor) * parent.scale), pivotWorld, parentRotation);
    return {
        // Backing off by the scaled anchor puts the renderer's `position + pivot` on the anchor.
        .position = anchorWorld - (local.anchor * scale),
        .scale = scale,
        .rotation = parent.rotation + local.rotation,
        // Pivot is per-entity, not inherited: a child spins about its own anchor.
        .pivot = local.anchor * scale,
    };
  }

  void WriteGlobal(Archetype* archetype, size_t chunkIdx, size_t indexInChunk, const GlobalTransform& g) const {
    // Hierarchy members are visited via the topology regardless of components, so a parented
    // entity without a GlobalTransformComponent (e.g. a UI node) reaches here. Guard before the
    // array fetch — GetComponentArray asserts HasComponent, it does not return null.
    if (!archetype->HasComponent(globalEntity_.GetId())) return;
    auto* gArray = archetype->GetComponentArray<GlobalTransformComponent>(chunkIdx, globalEntity_.GetId());
    if (!gArray) return;
    auto& global = gArray[indexInChunk];
    global.position = g.position;
    global.scale = g.scale;
    global.rotation = g.rotation;
    global.pivot = g.pivot;
  }

  std::unique_ptr<TransformQuery> optionalQuery_;
  // levels_[d] holds the parents at depth d grouped with their children; children_ is the flat
  // backing array those groups index into.
  std::vector<std::vector<ParentGroup>> levels_;
  std::vector<Entity> children_;
  // Parallel to children_, so members with no GlobalTransformComponent can still hand a result on.
  std::vector<GlobalTransform> childGlobals_;
  std::unordered_set<EntityID> visited_;
  uint64_t cachedHierarchyGeneration_ = 0;
  bool levelsBuilt_ = false;
  Entity posEntity_ = {};
  Entity scaleEntity_ = {};
  Entity rotEntity_ = {};
  Entity pivotEntity_ = {};
  Entity globalEntity_ = {};
  Entity spriteEntity_ = {};
  Entity squareEntity_ = {};
  Entity colliderEntity_ = {};
};
