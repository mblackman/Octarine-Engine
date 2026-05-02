#pragma once

#include <memory>

#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/Entity.h"
#include "../ECS/Iterable.h"
#include "../ECS/Registry.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"
#include "../Game/GameConfig.h"

class MovementSystem {
 public:
  void Init(Registry* registry, const std::unique_ptr<EventBus>& eventBus) {
    registry_ = registry;
    enemies_ = registry_->TagId("enemies");
    obstacles_ = registry_->TagId("obstacles");
    player_ = registry_->TagId("player");
    eventBus->SubscribeEvent<MovementSystem, CollisionEvent>(this, &MovementSystem::OnCollision);
  }

  void operator()(const ContextFacade& context, const Iterable& iterable) const {
    const float dt = context.DeltaTime();
    const auto& gameConfig = registry_->Get<GameConfig>();

    for (auto&& ctx : iterable) {
      const Entity entity = ctx.Entity();
      auto& transform = ctx.Component<TransformComponent>();
      const auto& rigidBody = ctx.Component<RigidBodyComponent>();

      const bool isPlayer = registry_->HasTag(entity, player_);

      if (!isPlayer && IsEntityOutsideMap(entity, transform, gameConfig)) {
        registry_->QueueBlamEntity(entity);
        continue;
      }

      transform.position.x += rigidBody.velocity.x * dt;
      transform.position.y += rigidBody.velocity.y * dt;

      if (isPlayer) {
        ClampPlayerToPlayableArea(entity, transform, gameConfig);
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

  [[nodiscard]] bool IsEntityOutsideMap(const Entity entity, const TransformComponent& transform,
                                        const GameConfig& gameConfig) const {
    const bool pastFarEdges = transform.position.x > static_cast<float>(gameConfig.windowWidth) ||
                              transform.position.y > static_cast<float>(gameConfig.windowHeight);
    if (pastFarEdges) return true;

    if (registry_->HasComponent<SpriteComponent>(entity)) {
      const auto& sprite = registry_->GetComponent<SpriteComponent>(entity);
      return transform.position.x + sprite.width * transform.scale.x < 0 ||
             transform.position.y + sprite.height * transform.scale.y < 0;
    }
    return transform.position.x < 0 || transform.position.y < 0;
  }

  void ClampPlayerToPlayableArea(const Entity entity, TransformComponent& transform,
                                 const GameConfig& gameConfig) const {
    if (!registry_->HasComponent<SpriteComponent>(entity)) return;

    const auto& sprite = registry_->GetComponent<SpriteComponent>(entity);

    if (transform.position.x < 0) transform.position.x = 0;
    if (transform.position.y < 0) transform.position.y = 0;

    if (transform.position.x + sprite.width * transform.scale.x > gameConfig.playableAreaWidth) {
      transform.position.x = gameConfig.playableAreaWidth - sprite.width * transform.scale.x;
    }
    if (transform.position.y + sprite.height * transform.scale.y > gameConfig.playableAreaHeight) {
      transform.position.y = gameConfig.playableAreaHeight - sprite.height * transform.scale.y;
    }
  }

  Registry* registry_ = nullptr;
  Entity enemies_{};
  Entity obstacles_{};
  Entity player_{};
};
