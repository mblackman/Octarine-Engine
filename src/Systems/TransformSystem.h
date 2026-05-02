#pragma once

#include <queue>

#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"

class TransformSystem : public System {
 public:
  TransformSystem() { RequireComponent<TransformComponent>(); }

  TransformSystem(const TransformSystem&) = delete;
  TransformSystem& operator=(const TransformSystem&) = delete;

  TransformSystem(TransformSystem&&) = delete;
  TransformSystem& operator=(TransformSystem&&) = delete;

  ~TransformSystem() = default;

  void Update() const {
    for (auto entity : GetRootEntities()) {
      auto& transform = entity.GetComponent<TransformComponent>();
      transform.globalPosition.x = transform.position.x;
      transform.globalPosition.y = transform.position.y;
      transform.globalScale.x = transform.scale.x;
      transform.globalScale.y = transform.scale.y;
      transform.globalRotation = transform.rotation;

      auto children = entity.GetChildren();

      if (children.has_value()) {
        for (const auto& child : children.value()) {
          UpdateEntityTransform(child, transform.position, transform.scale, transform.rotation);
        }
      }
    }
  }

 private:
  void UpdateEntityTransform(const Entity entity, glm::vec2 position, glm::vec2 scale, double rotation) const {
    if (entity.HasComponent<TransformComponent>()) {
      auto& transform = entity.GetComponent<TransformComponent>();
      position += transform.position;
      scale *= transform.scale;
      rotation += transform.rotation;
      transform.globalPosition.x = position.x;
      transform.globalPosition.y = position.y;
      transform.globalScale.x = scale.x;
      transform.globalScale.y = scale.y;
      transform.globalRotation = rotation;
    }

    auto children = entity.GetChildren();

    if (children.has_value()) {
      for (const auto& child : children.value()) {
        UpdateEntityTransform(child, position, scale, rotation);
      }
    }
  }
};