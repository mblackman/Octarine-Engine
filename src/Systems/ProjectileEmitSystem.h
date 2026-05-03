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

class ProjectileEmitSystem {
 public:
  void Init(const std::unique_ptr<EventBus>& eventBus) {
    eventBus->SubscribeEvent<ProjectileEmitSystem, KeyInputEvent>(this, &ProjectileEmitSystem::OnKeyInput);
  }

  void operator()(const ContextFacade& context, const TransformComponent& transform,
                  ProjectileEmitterComponent& emitter) {
    const auto registry = context.Registry();
    const bool isPlayer = registry->HasTag(context.Entity(), "player");

    if (!isPlayer) {
      emitter.countDownTimer -= context.DeltaTime();

      if (emitter.countDownTimer <= 0.0f) {
        SpawnProjectile(transform, context.Entity(), registry, emitter);
        emitter.countDownTimer = emitter.frequency;
      }
    } else if (spawnFriendlyProjectiles_) {
      SpawnProjectile(transform, context.Entity(), registry, emitter, true);
      spawnFriendlyProjectiles_ = false;
    }
  }

  void OnKeyInput(const KeyInputEvent& event) {
    if (!event.isPressed) {
      return;
    }

    if (event.inputKey == SDLK_SPACE) {
      spawnFriendlyProjectiles_ = true;
    }
  }

 private:
  bool spawnFriendlyProjectiles_ = false;

  static void SpawnProjectile(const TransformComponent& transform, const Entity& entity, Registry* registry,
                              ProjectileEmitterComponent& emitter, bool isPlayer = false) {
    auto projectilePosition = transform.position;
    auto velocity = emitter.velocity;

    if (registry->HasComponent<SpriteComponent>(entity)) {
      const auto& sprite = registry->GetComponent<SpriteComponent>(entity);
      projectilePosition.x += transform.scale.x * static_cast<float>(sprite.width) / 2;
      projectilePosition.y += transform.scale.y * static_cast<float>(sprite.height) / 2;
    }

    if (isPlayer && registry->HasComponent<RigidBodyComponent>(entity)) {
      const auto& rigidBody = registry->GetComponent<RigidBodyComponent>(entity);
      // Skip normalize when the player is idle — glm::normalize((0,0)) returns NaN.
      // Fall through with the emitter's default velocity vector.
      if (glm::length(rigidBody.velocity) > 1e-4f) {
        const auto direction = glm::normalize(rigidBody.velocity);
        velocity = direction * emitter.velocity;
      }
    }

    auto projectile = registry->CreateEntity();
    const auto entityMask = registry->GetComponent<EntityMaskComponent>(entity);
    registry->AddTag(projectile, "projectiles");
    registry->AddComponent(projectile, EntityMaskComponent(entityMask.mask));
    registry->AddComponent(projectile, TransformComponent(projectilePosition, glm::vec2(1.0, 1.0), 0.0));
    registry->AddComponent(projectile, RigidBodyComponent(velocity));
    registry->AddComponent(projectile, BoxColliderComponent(4, 4, glm::vec2(0, 0), emitter.collisionMask));
    registry->AddComponent(projectile, ProjectileComponent(emitter.damage, emitter.duration));
    registry->AddComponent(projectile, SpriteComponent("bullet-texture", 4.0f, 4.0f, 4));
  }
};