#pragma once

#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/Entity.h"
#include "../ECS/Registry.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"
#include "../Game/GameConfig.h"
#include "ECS/EntityCommandBuffer.h"

class MovementSystem {
 public:
  EntityCommandBuffer& GetCommandBuffer() { return cmd_buffer_; }

  void Init(Registry* registry) {
    registry_ = registry;
    const auto& gameConfig = registry_->Get<GameConfig>();
    const auto& eventBus = registry_->Get<EventBus*>();

    enemies_ = registry_->TagId("enemies");
    obstacles_ = registry_->TagId("obstacles");
    player_ = registry_->TagId("player");

    // TODO Fetch these again if the config changes.
    windowWidth_ = static_cast<float>(gameConfig.windowWidth);
    windowHeight_ = static_cast<float>(gameConfig.windowHeight);
    playableAreaWidth_ = static_cast<float>(gameConfig.playableAreaWidth);
    playableAreaHeight_ = static_cast<float>(gameConfig.playableAreaHeight);

    eventBus->SubscribeEvent<MovementSystem, CollisionEvent>(this, &MovementSystem::OnCollision);
  }

  void operator()(const Entity entity, const float deltaTime, TransformComponent& transform,
                  const RigidBodyComponent& rigidBody, const SpriteComponent& spriteComponent) {
    const bool isPlayer = registry_->HasTag(entity, player_);

    if (!isPlayer) {
      // Use global position so children of moving parents respect off-screen bounds.
      const float right = transform.globalPosition.x + spriteComponent.width * transform.globalScale.x;
      const float bottom = transform.globalPosition.y + spriteComponent.height * transform.globalScale.y;
      if (transform.globalPosition.x > windowWidth_ || transform.globalPosition.y > windowHeight_ || right < 0 ||
          bottom < 0) {
        cmd_buffer_.QueueBlam(entity);
        return;
      }
    }

    transform.position.x += rigidBody.velocity.x * deltaTime;
    transform.position.y += rigidBody.velocity.y * deltaTime;

    if (isPlayer) {
      if (transform.position.x < 0) transform.position.x = 0;
      if (transform.position.y < 0) transform.position.y = 0;
      if (transform.position.x + spriteComponent.width * transform.scale.x > playableAreaWidth_) {
        transform.position.x = playableAreaWidth_ - spriteComponent.width * transform.scale.x;
      }
      if (transform.position.y + spriteComponent.height * transform.scale.y > playableAreaHeight_) {
        transform.position.y = playableAreaHeight_ - spriteComponent.height * transform.scale.y;
      }
    }
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
  EntityCommandBuffer cmd_buffer_;
  Entity enemies_{};
  Entity obstacles_{};
  Entity player_{};
  float windowWidth_{};
  float windowHeight_{};
  float playableAreaWidth_{};
  float playableAreaHeight_{};
};
