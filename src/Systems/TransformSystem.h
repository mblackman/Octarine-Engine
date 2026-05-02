#pragma once

#include <queue>

#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"

struct TransformUpdateJob {
  Entity entity;
  glm::vec2 parent_global_position;
  glm::vec2 parent_global_scale;
  double parent_global_rotation;
};

class TransformSystem : public System {
 public:
  TransformSystem() { RequireComponent<TransformComponent>(); }

  TransformSystem(const TransformSystem&) = delete;
  TransformSystem& operator=(const TransformSystem&) = delete;

  TransformSystem(TransformSystem&&) = delete;
  TransformSystem& operator=(TransformSystem&&) = delete;

  ~TransformSystem() = default;

  void Update() const {
    std::stack<TransformUpdateJob> jobs;

    for (const auto& rootEntity : GetRootEntities()) {
      jobs.push({rootEntity, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0.0});
    }

    while (!jobs.empty()) {
      const auto [entity, parent_global_position, parent_global_scale, parent_global_rotation] = jobs.top();
      jobs.pop();

      if (!entity.HasComponent<TransformComponent>()) {
        continue;
      }

      auto& transform = entity.GetComponent<TransformComponent>();

      const glm::vec2 globalPosition = parent_global_position + transform.position;
      const glm::vec2 globalScale = parent_global_scale * transform.scale;
      const double globalRotation = parent_global_rotation + transform.rotation;

      transform.globalPosition = globalPosition;
      transform.globalScale = globalScale;
      transform.globalRotation = globalRotation;

      if (auto children = entity.GetChildren(); children.has_value()) {
        for (const auto& child : children.value()) {
          jobs.push({child, globalPosition, globalScale, globalRotation});
        }
      }
    }
  }
};