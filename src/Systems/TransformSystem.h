#pragma once

#include <cmath>
#include <glm/glm.hpp>
#include <stack>
#include <vector>

#include "Components/GlobalTransformComponent.h"
#include "Components/PositionComponent.h"
#include "Components/RotationComponent.h"
#include "Components/ScaleComponent.h"
#include "ECS/Iterable.h"
#include "ECS/Query.h"
#include "ECS/Registry.h"
#include "General/Logger.h"
#include "General/PerfUtils.h"

struct TransformUpdateJob {
  Entity entity;
  glm::vec2 parentGlobalPosition;
  glm::vec2 parentGlobalScale;
  double parentGlobalRotation;
};

class TransformSystem {
 public:
  void operator()(const ContextFacade& ctx, const Iterable& /*iter*/) {
    auto* registry = ctx.GetRegistry();
    EnsureInitialized(registry);
    optionalQuery_->Update();

    if (!registry->HasAnyChildPairs()) {
      UpdateFlat();
    } else {
      UpdateHierarchical(registry);
    }
  }

 private:
  using TransformQuery =
      ComponentQuery<GlobalTransformComponent, Opt<PositionComponent>, Opt<ScaleComponent>, Opt<RotationComponent>>;

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

  // Fast path: no ChildOf hierarchy live. Unrelated relationship pairs do not disable it.
  // Each entity's global is its local (identity for missing slots), in a single parallel pass.
  void UpdateFlat() {
    PROFILE_NAMED_SCOPE("TransformSystem: Fast");
    LogPathOnce("TransformSystem: FAST path (no hierarchy)");
    optionalQuery_->ParallelForEach(
        [](GlobalTransformComponent& global, const PositionComponent* p, const ScaleComponent* s,
           const RotationComponent* r) {
          global.position = p ? p->value : glm::vec2(0.0f, 0.0f);
          global.scale = s ? s->value : glm::vec2(1.0f, 1.0f);
          global.rotation = r ? r->value : 0.0;
        },
        kFlatSerialBelowEntities);
  }

  // Hierarchy path, two phases. Phase 1 runs the parallel flat pass over every transform
  // entity (global = local) — final for the non-hierarchy majority, and exactly what the old
  // serial roots pass wrote for parentless entities. Phase 2 then re-composes only hierarchy
  // members, seeded from the parent index rather than a full-world scan, so a small hierarchy
  // no longer drags every entity onto a serial walk with per-entity hash lookups. No root
  // caching across frames: entities created into an existing archetype (e.g. pool factory
  // growth) are picked up because phase 1 iterates the live query every frame.
  void UpdateHierarchical(Registry* registry) {
    PROFILE_NAMED_SCOPE("TransformSystem: Slow");
    LogPathOnce("TransformSystem: SLOW path (hierarchy detected)");

    UpdateFlat();

    std::stack<TransformUpdateJob> jobs;
    SeedRootJobs(registry, jobs);
    WalkDescendants(registry, jobs);
  }

  // Queue each root's children for the descendants walk. The parent state is the root's
  // global, which phase 1 already resolved to its local. Roots without a
  // GlobalTransformComponent contribute identity, so their children compose as their own
  // locals — consistent with what the flat pass wrote for them.
  void SeedRootJobs(const Registry* registry, std::stack<TransformUpdateJob>& jobs) const {
    PROFILE_NAMED_SCOPE("TransformSystem: Slow (seed root jobs)");
    registry->ForEachHierarchyRoot([&](const Entity root) {
      glm::vec2 rootPos(0.0f, 0.0f);
      glm::vec2 rootScale(1.0f, 1.0f);
      double rootRot = 0.0;

      const auto [archetype, chunkIdx, indexInChunk] = registry->GetEntityLocation(root);
      if (archetype && archetype->HasComponent(globalEntity_.GetId())) {
        const auto* gArr = archetype->GetComponentArray<GlobalTransformComponent>(chunkIdx, globalEntity_.GetId());
        rootPos = gArr[indexInChunk].position;
        rootScale = gArr[indexInChunk].scale;
        rootRot = gArr[indexInChunk].rotation;
      }

      registry->ForEachChild(root, [&](const Entity child) { jobs.push({child, rootPos, rootScale, rootRot}); });
    });
  }

  // Drain the job stack: compose each entity's global from its parent's global and its own
  // local, then enqueue its children with the freshly-computed values as their parent state.
  void WalkDescendants(const Registry* registry, std::stack<TransformUpdateJob>& jobs) const {
    PROFILE_NAMED_SCOPE("TransformSystem: Slow (descendants walk)");
    while (!jobs.empty()) {
      const auto job = jobs.top();
      jobs.pop();

      const auto [archetype, chunkIdx, indexInChunk] = registry->GetEntityLocation(job.entity);
      if (!archetype) continue;

      const auto local = LoadLocal(archetype, chunkIdx, indexInChunk);
      const auto global = Compose(job, local);

      WriteGlobal(archetype, chunkIdx, indexInChunk, global);

      registry->ForEachChild(
          job.entity, [&](const Entity child) { jobs.push({child, global.position, global.scale, global.rotation}); });
    }
  }

  struct LocalTransform {
    glm::vec2 position;
    glm::vec2 scale;
    double rotation;
  };

  struct GlobalTransform {
    glm::vec2 position;
    glm::vec2 scale;
    double rotation;
  };

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
    };
  }

  // Compose parent×local into a world-space transform. Rotation is applied to the
  // already-scaled local position so children orbit their parent in parent space.
  static GlobalTransform Compose(const TransformUpdateJob& parent, const LocalTransform& local) {
    const float c = std::cos(static_cast<float>(parent.parentGlobalRotation));
    const float s_val = std::sin(static_cast<float>(parent.parentGlobalRotation));
    const glm::vec2 scaled = local.position * parent.parentGlobalScale;
    const glm::vec2 rotated = {scaled.x * c - scaled.y * s_val, scaled.x * s_val + scaled.y * c};
    return {
        parent.parentGlobalPosition + rotated,
        parent.parentGlobalScale * local.scale,
        parent.parentGlobalRotation + local.rotation,
    };
  }

  void WriteGlobal(Archetype* archetype, size_t chunkIdx, size_t indexInChunk, const GlobalTransform& g) const {
    auto* gArray = archetype->GetComponentArray<GlobalTransformComponent>(chunkIdx, globalEntity_.GetId());
    if (!gArray) return;
    auto& global = gArray[indexInChunk];
    global.position = g.position;
    global.scale = g.scale;
    global.rotation = g.rotation;
  }

  void LogPathOnce(const char* message) {
    if (loggedPath_) return;
    Logger::Info(message);
    loggedPath_ = true;
  }

  std::unique_ptr<TransformQuery> optionalQuery_;
  Entity posEntity_ = {};
  Entity scaleEntity_ = {};
  Entity rotEntity_ = {};
  Entity globalEntity_ = {};
  bool loggedPath_ = false;
};
