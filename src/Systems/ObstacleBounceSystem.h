#pragma once

#include <memory>

#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../ECS/Entity.h"
#include "../ECS/Registry.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

class ObstacleBounceSystem {
 public:
  void Init(Registry* registry, const std::unique_ptr<EventBus>& eventBus) {
    registry_ = registry;
    enemies_ = registry_->TagId("enemies");
    obstacles_ = registry_->TagId("obstacles");
    eventBus->SubscribeEvent<ObstacleBounceSystem, CollisionEvent>(this, &ObstacleBounceSystem::OnCollision);
  }

  void OnCollision(const CollisionEvent& event) {
    const Entity a = event.entityA;
    const Entity b = event.entityB;

    if (registry_->HasTag(a, enemies_) && registry_->HasTag(b, obstacles_)) {
      OnObstacleCollision(a);
    }
    if (registry_->HasTag(b, enemies_) && registry_->HasTag(a, obstacles_)) {
      OnObstacleCollision(b);
    }
  }

 private:
  void OnObstacleCollision(const Entity enemy) const {
    auto& rigidBody = registry_->GetComponent<RigidBodyComponent>(enemy);
    rigidBody.velocity = rigidBody.velocity * -1.0f;

    if (registry_->HasComponent<SpriteComponent>(enemy)) {
      auto& sprite = registry_->GetComponent<SpriteComponent>(enemy);
      sprite.flip = sprite.flip == SDL_FLIP_NONE ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    }
  }

  Registry* registry_ = nullptr;
  Entity enemies_{};
  Entity obstacles_{};
};
