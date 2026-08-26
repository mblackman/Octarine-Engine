#pragma once

#include <cmath>
#include <cstdint>
#include <glm/glm.hpp>
#include <memory>
#include <unordered_set>
#include <vector>

#include "Components/GlobalTransformComponent.h"
#include "Components/PositionComponent.h"
#include "Components/RotationComponent.h"
#include "Components/ScaleComponent.h"
#include "ECS/Iterable.h"
#include "ECS/Query.h"
#include "ECS/Registry.h"
#include "General/PerfUtils.h"
#include "General/Rotation2D.h"

class TransformSystem {
 public:
  void operator()(const ContextFacade& ctx, const Iterable& /*iter*/) {
    auto* registry = ctx.GetRegistry();
    EnsureInitialized(registry);
    optionalQuery_->Update();

    // The flat pass is unconditional: it finalises every entity outside a hierarchy, and it
    // leaves hierarchy roots holding exactly their local transform, which is what the descend
    // pass wants to read out of them.
    UpdateFlat();

    if (registry->HasAnyChildPairs()) {
      UpdateHierarchical(registry);
    }
  }

 private:
  using TransformQuery =
      ComponentQuery<GlobalTransformComponent, Opt<PositionComponent>, Opt<ScaleComponent>, Opt<RotationComponent>>;

  struct LocalTransform {
    glm::vec2 position{0.0f, 0.0f};
    glm::vec2 scale{1.0f, 1.0f};
    double rotation{0.0};
    glm::vec2 pivotOffset{0.0f, 0.0f};  // local pixels, resolved by PivotResolveSystem
  };

  struct GlobalTransform {
    glm::vec2 position{0.0f, 0.0f};
    glm::vec2 scale{1.0f, 1.0f};
    double rotation{0.0};
    glm::vec2 pivot{0.0f, 0.0f};  // world pixels, offset from position
  };

  // One parent paired with the contiguous span of its children inside children_. Grouping by
  // parent is what makes the descend pass cheap: the parent's global is resolved once and reused
  // across every child in the span, instead of being re-resolved per child.
  struct ParentGroup {
    Entity parent{};
    uint32_t firstChild{0};
    uint32_t childCount{0};
  };

  void EnsureInitialized(Registry* registry) {
    if (optionalQuery_) return;
    optionalQuery_ = registry->CreateQuery<GlobalTransformComponent, Opt<PositionComponent>, Opt<ScaleComponent>,
                                           Opt<RotationComponent>>();
    posEntity_ = registry->Component<PositionComponent>();
    scaleEntity_ = registry->Component<ScaleComponent>();
    rotEntity_ = registry->Component<RotationComponent>();
    globalEntity_ = registry->Component<GlobalTransformComponent>();
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
           const RotationComponent* r) {
          const glm::vec2 scale = s ? s->value : glm::vec2(1.0f, 1.0f);
          global.position = p ? p->value : glm::vec2(0.0f, 0.0f);
          global.scale = scale;
          global.rotation = r ? r->value : 0.0;
          global.pivot = r ? r->pivotOffset * scale : glm::vec2(0.0f, 0.0f);
        },
        kFlatSerialBelowEntities);
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

    std::vector<Entity> frontier;
    registry->ForEachHierarchyRoot([&](const Entity root) {
      if (visited_.insert(root.id).second) frontier.push_back(root);
    });

    while (!frontier.empty()) {
      frontier = BuildLevel(registry, frontier);
    }

    cachedHierarchyGeneration_ = generation;
    levelsBuilt_ = true;
  }

  // Append one depth bucket for the given parents and return the next frontier (their children).
  std::vector<Entity> BuildLevel(const Registry* registry, const std::vector<Entity>& parents) {
    std::vector<ParentGroup> level;
    std::vector<Entity> next;

    for (const Entity parent : parents) {
      const auto first = static_cast<uint32_t>(children_.size());
      registry->ForEachChild(parent, [&](const Entity child) {
        // A SetParent cycle would otherwise loop forever here. visited_ also keeps an entity
        // reachable by two routes from being composed twice.
        if (!visited_.insert(child.id).second) return;
        children_.push_back(child);
        next.push_back(child);
      });
      if (const auto count = static_cast<uint32_t>(children_.size()) - first; count > 0) {
        level.push_back({parent, first, count});
      }
    }

    if (!level.empty()) levels_.push_back(std::move(level));
    return next;
  }

  void Descend(const Registry* registry) const {
    PROFILE_NAMED_SCOPE("TransformSystem: Descend");
    for (const auto& level : levels_) {
      for (const ParentGroup& group : level) {
        const GlobalTransform parentGlobal = LoadGlobal(registry, group.parent);
        // Every child in the group shares this one parent, so its cos/sin pair is resolved once
        // per family rather than once per child. This is the payoff for grouping by parent: a
        // hundred-child emitter costs one trig pair instead of a hundred.
        const octarine::Rotation2D parentRotation = octarine::Rotation2D::FromRadians(parentGlobal.rotation);
        for (uint32_t i = 0; i < group.childCount; ++i) {
          ComposeChild(registry, children_[group.firstChild + i], parentGlobal, parentRotation);
        }
      }
    }
  }

  void ComposeChild(const Registry* registry, const Entity child, const GlobalTransform& parentGlobal,
                    const octarine::Rotation2D parentRotation) const {
    const auto [archetype, chunkIdx, indexInChunk] = registry->GetEntityLocation(child);
    if (!archetype) return;
    const GlobalTransform global = Compose(parentGlobal, parentRotation, LoadLocal(archetype, chunkIdx, indexInChunk));
    WriteGlobal(archetype, chunkIdx, indexInChunk, global);
  }

  // A parent's global was finalised either by the flat pass (roots) or by the previous depth
  // bucket. A parent without a GlobalTransformComponent — a bare grouping node — contributes
  // identity, so its children compose as their own locals.
  GlobalTransform LoadGlobal(const Registry* registry, const Entity parent) const {
    const auto [archetype, chunkIdx, indexInChunk] = registry->GetEntityLocation(parent);
    if (!archetype || !archetype->HasComponent(globalEntity_.GetId())) {
      return {glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0.0, glm::vec2(0.0f, 0.0f)};
    }
    const auto* gArr = archetype->GetComponentArray<GlobalTransformComponent>(chunkIdx, globalEntity_.GetId());
    const auto& g = gArr[indexInChunk];
    return {g.position, g.scale, g.rotation, g.pivot};
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
    return {
        pArr ? pArr[indexInChunk].value : glm::vec2(0.0f, 0.0f),
        sArr ? sArr[indexInChunk].value : glm::vec2(1.0f, 1.0f),
        rArr ? rArr[indexInChunk].value : 0.0,
        rArr ? rArr[indexInChunk].pivotOffset : glm::vec2(0.0f, 0.0f),
    };
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
    return {
        .position =
            octarine::RotateAround(parent.position + (local.position * parent.scale), pivotWorld, parentRotation),
        .scale = scale,
        .rotation = parent.rotation + local.rotation,
        // Pivot is a per-entity property, not an inherited one: a child spins about its own
        // anchor. Only the parent's pivot influences where the child is placed, above.
        .pivot = local.pivotOffset * scale,
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
  std::unordered_set<EntityID> visited_;
  uint64_t cachedHierarchyGeneration_ = 0;
  bool levelsBuilt_ = false;
  Entity posEntity_ = {};
  Entity scaleEntity_ = {};
  Entity rotEntity_ = {};
  Entity globalEntity_ = {};
};
