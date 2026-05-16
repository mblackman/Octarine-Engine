#pragma once

#include <cmath>
#include <glm/glm.hpp>
#include <stack>
#include <vector>

#include "../Components/GlobalTransformComponent.h"
#include "../Components/PositionComponent.h"
#include "../Components/RotationComponent.h"
#include "../Components/ScaleComponent.h"
#include "../ECS/Iterable.h"
#include "../ECS/Query.h"
#include "../ECS/Registry.h"
#include "../General/Logger.h"
#include "../General/PerfUtils.h"

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

    if (!globalQuery_) {
      globalQuery_ = registry->CreateQuery<GlobalTransformComponent>();
      positionQuery_ = registry->CreateQuery<PositionComponent, GlobalTransformComponent>();
      scaleQuery_ = registry->CreateQuery<ScaleComponent, GlobalTransformComponent>();
      rotationQuery_ = registry->CreateQuery<RotationComponent, GlobalTransformComponent>();
    }
    globalQuery_->Update();
    positionQuery_->Update();
    scaleQuery_->Update();
    rotationQuery_->Update();

    // Fast path: no ChildOf hierarchy live. Unrelated relationship pairs do not disable it.
    if (!registry->HasAnyChildPairs()) {
      PROFILE_NAMED_SCOPE("TransformSystem: Fast");
      if (!loggedPath_) {
        Logger::Info("TransformSystem: FAST path (no hierarchy)");
        loggedPath_ = true;
      }
      // Reset all globals to identity, then overlay any locals that are present. Entities missing
      // a local component default to identity for that field.
      globalQuery_->ParallelForEach([](GlobalTransformComponent& global) {
        global.position = glm::vec2(0.0f, 0.0f);
        global.scale = glm::vec2(1.0f, 1.0f);
        global.rotation = 0.0;
      });
      positionQuery_->ParallelForEach([](const PositionComponent& position, GlobalTransformComponent& global) {
        global.position = position.value;
      });
      scaleQuery_->ParallelForEach([](const ScaleComponent& scale, GlobalTransformComponent& global) {
        global.scale = scale.value;
      });
      rotationQuery_->ParallelForEach([](const RotationComponent& rotation, GlobalTransformComponent& global) {
        global.rotation = rotation.value;
      });
      return;
    }
    PROFILE_NAMED_SCOPE("TransformSystem: Slow");
    if (!loggedPath_) {
      Logger::Info("TransformSystem: SLOW path (hierarchy detected)");
      loggedPath_ = true;
    }

    // Slow path: walk top-down from roots. Rebuild the roots list every frame — caching
    // by ArchetypeGeneration misses entities created in an existing archetype (e.g. pool
    // factory growth that reuses a registered projectile archetype), leaving their globals
    // stuck at the default (0,0). The roots scan is a single O(N) pass over entities with
    // GlobalTransformComponent and is cheap relative to the actual walk below.
    {
      PROFILE_NAMED_SCOPE("TransformSystem: Slow (roots rebuild)");
      roots_.clear();
      globalQuery_->ForEach([&](const Entity entity, GlobalTransformComponent& /*global*/) {
        if (!registry->GetParent(entity).has_value()) {
          roots_.push_back(entity);
        }
      });
    }

    PROFILE_NAMED_SCOPE("TransformSystem: Slow (walk)");
    std::stack<TransformUpdateJob> jobs;
    for (const Entity root : roots_) {
      jobs.push({root, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0.0});
    }

    while (!jobs.empty()) {
      const auto [entity, parentPos, parentScale, parentRot] = jobs.top();
      jobs.pop();

      if (!registry->HasComponent<GlobalTransformComponent>(entity)) {
        continue;
      }

      const glm::vec2 localPos = registry->HasComponent<PositionComponent>(entity)
                                     ? registry->GetComponent<PositionComponent>(entity).value
                                     : glm::vec2(0.0f, 0.0f);
      const glm::vec2 localScale = registry->HasComponent<ScaleComponent>(entity)
                                       ? registry->GetComponent<ScaleComponent>(entity).value
                                       : glm::vec2(1.0f, 1.0f);
      const double localRot = registry->HasComponent<RotationComponent>(entity)
                                  ? registry->GetComponent<RotationComponent>(entity).value
                                  : 0.0;

      const float c = std::cos(static_cast<float>(parentRot));
      const float s = std::sin(static_cast<float>(parentRot));
      const glm::vec2 scaled = localPos * parentScale;
      const glm::vec2 rotated = {scaled.x * c - scaled.y * s, scaled.x * s + scaled.y * c};
      const glm::vec2 globalPosition = parentPos + rotated;

      const glm::vec2 globalScale = parentScale * localScale;
      const double globalRotation = parentRot + localRot;

      auto& global = registry->GetComponent<GlobalTransformComponent>(entity);
      global.position = globalPosition;
      global.scale = globalScale;
      global.rotation = globalRotation;

      for (const auto& child : registry->GetChildren(entity)) {
        jobs.push({child, globalPosition, globalScale, globalRotation});
      }
    }
  }

 private:
  std::unique_ptr<ComponentQuery<GlobalTransformComponent>> globalQuery_;
  std::unique_ptr<ComponentQuery<PositionComponent, GlobalTransformComponent>> positionQuery_;
  std::unique_ptr<ComponentQuery<ScaleComponent, GlobalTransformComponent>> scaleQuery_;
  std::unique_ptr<ComponentQuery<RotationComponent, GlobalTransformComponent>> rotationQuery_;
  std::vector<Entity> roots_;
  bool loggedPath_ = false;
};
