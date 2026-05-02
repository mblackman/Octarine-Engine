#pragma once

#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

class CollisionSystem : public System {
 public:
  CollisionSystem() {
    RequireComponent<TransformComponent>();
    RequireComponent<BoxColliderComponent>();
  }

  CollisionSystem(const CollisionSystem&) = delete;
  CollisionSystem& operator=(const CollisionSystem&) = delete;

  CollisionSystem(CollisionSystem&&) = delete;
  CollisionSystem& operator=(CollisionSystem&&) = delete;

  ~CollisionSystem() = default;

  void Update(const std::unique_ptr<EventBus>& eventBus) const {
    auto entities = GetEntities();

    for (auto i = entities.begin(); i != entities.end(); i++) {
      auto entityA = *i;
      const auto& transformA = entityA.GetComponent<TransformComponent>();
      const auto& colliderA = entityA.GetComponent<BoxColliderComponent>();

      for (auto j = i + 1; j != entities.end(); j++) {
        auto entityB = *j;
        const auto& transformB = entityB.GetComponent<TransformComponent>();
        const auto& colliderB = entityB.GetComponent<BoxColliderComponent>();
        const bool isColliding = CheckAABBCollision(transformA, colliderA, transformB, colliderB);
        if (isColliding) {
          eventBus->EmitEvent<CollisionEvent>(entityA, entityB);
        }
      }
    }
  }

  static bool CheckAABBCollision(const TransformComponent& transformA, const BoxColliderComponent& colliderA,
                                 const TransformComponent& transformB, const BoxColliderComponent& colliderB) {
    // Calculate AABB for entity A
    const float A_x_min = transformA.position.x + colliderA.offset.x;
    const float A_y_min = transformA.position.y + colliderA.offset.y;
    const float A_x_max = A_x_min + static_cast<float>(colliderA.width) * transformA.scale.x;
    const float A_y_max = A_y_min + static_cast<float>(colliderA.height) * transformA.scale.y;

    // Calculate AABB for entity B
    const float B_x_min = transformB.position.x + colliderB.offset.x;
    const float B_y_min = transformB.position.y + colliderB.offset.y;
    const float B_x_max = B_x_min + static_cast<float>(colliderB.width) * transformB.scale.x;
    const float B_y_max = B_y_min + static_cast<float>(colliderB.height) * transformB.scale.y;

    // Check for overlap on both axes
    return (A_x_min < B_x_max && A_x_max > B_x_min && A_y_min < B_y_max && A_y_max > B_y_min);
  }
};