#pragma once

#include <memory>

#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/Entity.h"
#include "../ECS/Iterable.h"
#include "../ECS/Query.h"
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
    // Query includes SpriteComponent so we get direct SoA access for bounds checking
    // instead of per-entity random GetComponent lookups.
    query_ = registry_->CreateQuery<TransformComponent, RigidBodyComponent, SpriteComponent>();
    eventBus->SubscribeEvent<MovementSystem, CollisionEvent>(this, &MovementSystem::OnCollision);
  }

  void operator()(const ContextFacade& context, const Iterable& /*iterable*/) {
    const float dt = context.DeltaTime();
    const auto& gameConfig = registry_->Get<GameConfig>();
    const auto windowWidth = static_cast<float>(gameConfig.windowWidth);
    const auto windowHeight = static_cast<float>(gameConfig.windowHeight);

    query_->Update();
    query_->ForEach([&](const Entity entity, TransformComponent& transform,
                        const RigidBodyComponent& rigidBody, const SpriteComponent& sprite) {
      const bool isPlayer = registry_->HasTag(entity, player_);

      if (!isPlayer) {
        const float right = transform.position.x + sprite.width * transform.scale.x;
        const float bottom = transform.position.y + sprite.height * transform.scale.y;
        if (transform.position.x > windowWidth || transform.position.y > windowHeight || right < 0 || bottom < 0) {
          registry_->QueueBlamEntity(entity);
          return;
        }
      }

      transform.position.x += rigidBody.velocity.x * dt;
      transform.position.y += rigidBody.velocity.y * dt;

      if (isPlayer) {
        if (transform.position.x < 0) transform.position.x = 0;
        if (transform.position.y < 0) transform.position.y = 0;
        if (transform.position.x + sprite.width * transform.scale.x > gameConfig.playableAreaWidth) {
          transform.position.x = gameConfig.playableAreaWidth - sprite.width * transform.scale.x;
        }
        if (transform.position.y + sprite.height * transform.scale.y > gameConfig.playableAreaHeight) {
          transform.position.y = gameConfig.playableAreaHeight - sprite.height * transform.scale.y;
        }
      }
    });
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
  Entity player_{};
  std::unique_ptr<ComponentQuery<TransformComponent, RigidBodyComponent, SpriteComponent>> query_;
};
