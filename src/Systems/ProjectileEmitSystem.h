#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_keycode.h>

#include <glm/glm.hpp>

#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"

class ProjectileEmitSystem : public System {
 public:
  ProjectileEmitSystem() : spawnFriendlyProjectiles_(false) {
    RequireComponent<TransformComponent>();
    RequireComponent<ProjectileEmitterComponent>();
  }

  ProjectileEmitSystem(const ProjectileEmitSystem&) = delete;
  ProjectileEmitSystem& operator=(const ProjectileEmitSystem&) = delete;

  ProjectileEmitSystem(ProjectileEmitSystem&&) = delete;
  ProjectileEmitSystem& operator=(ProjectileEmitSystem&&) = delete;

  ~ProjectileEmitSystem() = default;

  void SubscribeToEvents(const std::unique_ptr<EventBus>& eventBus) {
    eventBus->SubscribeEvent<ProjectileEmitSystem, KeyInputEvent>(this, &ProjectileEmitSystem::OnKeyInput);
  }

  void OnKeyInput(const KeyInputEvent& event) {
    if (!event.isPressed) {
      return;
    }

    if (event.inputKey == SDLK_SPACE) {
      spawnFriendlyProjectiles_ = true;
    }
  }

  void Update(const float deltaTime, const std::unique_ptr<Registry>& registry) {
    for (auto entity : GetEntities()) {
      auto transform = entity.GetComponent<TransformComponent>();
      auto& emitter = entity.GetComponent<ProjectileEmitterComponent>();

      if (!emitter.isFriendly) {
        emitter.countDownTimer -= deltaTime;

        if (emitter.countDownTimer <= 0.0f) {
          SpawnProjectile(transform, entity, registry, emitter);
          emitter.countDownTimer = emitter.frequency;
        }
      } else if (emitter.isFriendly && spawnFriendlyProjectiles_) {
        SpawnProjectile(transform, entity, registry, emitter);
        spawnFriendlyProjectiles_ = false;
      }
    }
  }

 private:
  bool spawnFriendlyProjectiles_;

  static void SpawnProjectile(const TransformComponent& transform, const Entity& entity,
                              const std::unique_ptr<Registry>& registry, ProjectileEmitterComponent& emitter) {
    auto projectilePosition = transform.position;
    auto velocity = emitter.velocity;

    if (entity.HasComponent<SpriteComponent>()) {
      const auto& sprite = entity.GetComponent<SpriteComponent>();
      projectilePosition.x += transform.scale.x * static_cast<float>(sprite.width) / 2;
      projectilePosition.y += transform.scale.y * static_cast<float>(sprite.height) / 2;
    }

    if (emitter.isFriendly && entity.HasComponent<RigidBodyComponent>()) {
      const auto rigidBody = entity.GetComponent<RigidBodyComponent>();
      const auto direction = glm::normalize(rigidBody.velocity);
      velocity = direction * emitter.velocity;
    }

    auto projectile = registry->CreateEntity();
    projectile.Group("projectiles");
    projectile.AddComponent<TransformComponent>(projectilePosition, glm::vec2(1.0, 1.0), 0.0);
    projectile.AddComponent<RigidBodyComponent>(velocity);
    projectile.AddComponent<BoxColliderComponent>(4, 4);
    projectile.AddComponent<SpriteComponent>("bullet-texture", 4.0f, 4.0f, 4);
    projectile.AddComponent<ProjectileComponent>(emitter.damage, emitter.duration, emitter.isFriendly);
  }
};