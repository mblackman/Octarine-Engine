#pragma once

#include <glm/glm.hpp>
#include <stack>

#include "../Components/TransformComponent.h"
#include "../ECS/Iterable.h"
#include "../ECS/Query.h"
#include "../ECS/Registry.h"
#include "../General/Logger.h"

struct TransformUpdateJob {
  Entity entity;
  glm::vec2 parentGlobalPosition;
  glm::vec2 parentGlobalScale;
  double parentGlobalRotation;
};

class TransformSystem {
 public:
  void operator()(const ContextFacade& ctx, const Iterable& /*iter*/) {
    auto* registry = ctx.Registry();

    if (!query_) {
      query_ = registry->CreateQuery<TransformComponent>();
    }
    query_->Update();

    // Fast path: when no parent-child relationships exist (the common case),
    // just copy local → global in one tight loop. Skips all GetParent hash lookups.
    if (!registry->HasAnyPairs()) {
      if (!loggedPath_) {
        Logger::Info("TransformSystem: FAST path (no hierarchy)");
        loggedPath_ = true;
      }
      query_->ParallelForEach([](TransformComponent& transform) {
        transform.globalPosition = transform.position;
        transform.globalScale = transform.scale;
        transform.globalRotation = transform.rotation;
      });
      return;
    }
    if (!loggedPath_) {
      Logger::Info("TransformSystem: SLOW path (hierarchy detected)");
      loggedPath_ = true;
    }

    // Slow path: hierarchy exists — identify roots and DFS-walk children.
    std::stack<TransformUpdateJob> jobs;

    query_->ForEach([&](const Entity entity, TransformComponent& /*transform*/) {
      if (!registry->GetParent(entity).has_value()) {
        jobs.push({entity, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0.0});
      }
    });

    while (!jobs.empty()) {
      const auto [entity, parentPos, parentScale, parentRot] = jobs.top();
      jobs.pop();

      if (!registry->HasComponent<TransformComponent>(entity)) {
        continue;
      }

      auto& transform = registry->GetComponent<TransformComponent>(entity);

      const glm::vec2 globalPosition = parentPos + transform.position;
      const glm::vec2 globalScale = parentScale * transform.scale;
      const double globalRotation = parentRot + transform.rotation;

      transform.globalPosition = globalPosition;
      transform.globalScale = globalScale;
      transform.globalRotation = globalRotation;

      for (const auto& child : registry->GetChildren(entity)) {
        jobs.push({child, globalPosition, globalScale, globalRotation});
      }
    }
  }

 private:
  std::unique_ptr<ComponentQuery<TransformComponent>> query_;
  bool loggedPath_ = false;
};
