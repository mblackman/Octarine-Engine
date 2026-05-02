#pragma once

#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"
#include "../General/Logger.h"

class MovementSystem : public System {
 public:
  MovementSystem() {
    RequireComponent<TransformComponent>();
    RequireComponent<RigidBodyComponent>();
  }

  ~MovementSystem() = default;

  void SubscribeToEvents(const std::unique_ptr<EventBus>& eventBus) {
    eventBus->SubscribeEvent<MovementSystem, CollisionEvent>(
        this, &MovementSystem::OnCollision);
  }

  void OnCollision(const CollisionEvent& event) {
    const auto a = event.entityA;
    const auto b = event.entityB;
    auto aId = std::to_string(event.entityA.GetId());
    auto bId = std::to_string(event.entityB.GetId());

    if (a.InGroup("enemies") && b.InGroup("obstacles")) {
      OnObstacleCollision(a);
    }

    if (b.InGroup("enemies") && a.InGroup("obstacles")) {
      OnObstacleCollision(b);
    }
  }

  void Update(double deltaTime) {
    for (auto entity : GetEntities()) {
      auto& transform = entity.GetComponent<TransformComponent>();
      const auto rigidBody = entity.GetComponent<RigidBodyComponent>();
      const bool isPlayer = entity.HasTag("player");

      if (!isPlayer && IsEntityOutsideMap(entity)) {
        Logger::Info("Entity went outside map " +
                     std::to_string(entity.GetId()));
        entity.Blam();
      } else {
        transform.position.x += rigidBody.velocity.x * deltaTime;
        transform.position.y += rigidBody.velocity.y * deltaTime;

        if (isPlayer) {
          const auto spriteComponent = entity.GetComponent<SpriteComponent>();
          if (transform.position.x < 0) {
            transform.position.x = 0;
          }

          if (transform.position.y < 0) {
            transform.position.y = 0;
          }

          if (transform.position.x + spriteComponent.width * transform.scale.x >
              Game::mapWidth) {
            transform.position.x =
                Game::mapWidth - spriteComponent.width * transform.scale.x;
          }

          if (transform.position.y +
                  spriteComponent.height * transform.scale.y >
              Game::mapHeight) {
            transform.position.y =
                Game::mapHeight - spriteComponent.height * transform.scale.y;
          }
        }
      }
    }
  }

 private:
  static void OnObstacleCollision(Entity enemy) {
    auto& rigidBody = enemy.GetComponent<RigidBodyComponent>();

    rigidBody.velocity = rigidBody.velocity * -1.0f;

    if (enemy.HasComponent<SpriteComponent>()) {
      auto& sprite = enemy.GetComponent<SpriteComponent>();
      sprite.flip =
          sprite.flip == SDL_FLIP_NONE ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    }
  }

  static bool IsEntityOutsideMap(Entity entity) {
    const auto transform = entity.GetComponent<TransformComponent>();
    bool isEntityOutsideMap = transform.position.x > Game::windowWidth || transform.position.y > Game::windowHeight;

    if (!isEntityOutsideMap) {
      if (entity.HasComponent<SpriteComponent>()) {
        const auto sprite = entity.GetComponent<SpriteComponent>();
        isEntityOutsideMap = transform.position.x + sprite.width * transform.scale.x < 0 ||
                             transform.position.y + sprite.height * transform.scale.y < 0;
      } else {
        isEntityOutsideMap = transform.position.x < 0 || transform.position.y < 0;
      }
    }

    return isEntityOutsideMap;
  }
};