#pragma once

#include <glm/glm.hpp>
#include <stack>

#include "../Components/TransformComponent.h"
#include "../ECS/Iterable.h"
#include "../ECS/Registry.h"

struct TransformUpdateJob {
  Entity entity;
  glm::vec2 parentGlobalPosition;
  glm::vec2 parentGlobalScale;
  double parentGlobalRotation;
};

class TransformSystem {
 public:
  void operator()(const ContextFacade& ctx, const Iterable& iter) const {
    auto* registry = ctx.Registry();

    std::stack<TransformUpdateJob> jobs;

    for (auto&& entityCtx : iter) {
      const Entity entity = entityCtx.Entity();
      if (registry->GetParent(entity).has_value()) {
        continue;
      }
      jobs.push({entity, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0.0});
    }

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
};
