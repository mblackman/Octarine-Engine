#pragma once

#include <SDL3/SDL_keycode.h>

#include <glm/glm.hpp>

#include "../Components/BoxColliderComponent.h"
#include "../Components/EntityMaskComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"
#include "../ECS/Registry.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyInputEvent.h"
#include "EntityPoolSystem.h"

class ProjectileEmitSystem {
 public:
  void Init(const std::unique_ptr<EventBus>& eventBus, Registry& registry) {
    projectile_pool_id_ =
        registry.Get<EntityPoolManager>().RegisterPool<ProjectileComponent>(registry, [](Registry& reg) {
          const auto entity =
              reg.CreateEntityWithBundle(EntityMaskComponent(), TransformComponent(), RigidBodyComponent(),
                                         BoxColliderComponent(4, 4, glm::vec2(0, 0), EntityMask{}),
                                         ProjectileComponent(), SpriteComponent("bullet-texture", 4.0f, 4.0f, 4));
          reg.AddTag<PoolableTag>(entity);
          reg.AddTag(entity, "projectiles");
          return entity;
        });
    eventBus->SubscribeEvent<ProjectileEmitSystem, KeyInputEvent>(this, &ProjectileEmitSystem::OnKeyInput);
  }

  void operator()(const ContextFacade& context, const TransformComponent& transform,
                  ProjectileEmitterComponent& emitter) {
    const auto registry = context.GetRegistry();
    const bool isPlayer = registry->HasTag(context.GetEntity(), "player");

    if (!isPlayer) {
      emitter.countDownTimer -= context.GetDeltaTime();

      if (emitter.countDownTimer <= 0.0f) {
        SpawnProjectile(transform, context.GetEntity(), registry, emitter);
        emitter.countDownTimer = emitter.frequency;
      }
    } else if (spawnFriendlyProjectiles_) {
      SpawnProjectile(transform, context.GetEntity(), registry, emitter, true);
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
  ArchetypeID projectile_pool_id_{};
  bool spawnFriendlyProjectiles_ = false;

  void SpawnProjectile(const TransformComponent& transform, const Entity& entity, Registry* registry,
                       ProjectileEmitterComponent& emitter, bool isPlayer = false) const {
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

    // Inherit the emitter's collision mask. Default-construct if the emitter has no mask
    // component (e.g. a scene-authored emitter that doesn't participate in collision filtering).
    const EntityMask emitterMask = registry->HasComponent<EntityMaskComponent>(entity)
                                       ? registry->GetComponent<EntityMaskComponent>(entity).mask
                                       : EntityMask();

    // Pool returns either a recycled entity (timer reset, shape intact) or a freshly built one
    // from the factory registered in Init. Either way we rewrite the per-spawn fields. Use the
    // TransformComponent constructor so globalPosition/globalScale/globalRotation are reset too —
    // a recycled projectile would otherwise carry stale globals from its prior life, and
    // MovementSystem's out-of-bounds check reads globalPosition and would despawn it instantly.
    const Entity projectile = registry->Get<EntityPoolManager>().Spawn(*registry, projectile_pool_id_);
    registry->GetComponent<EntityMaskComponent>(projectile).mask = emitterMask;
    registry->GetComponent<TransformComponent>(projectile) =
        TransformComponent(projectilePosition, glm::vec2(1.0f, 1.0f), 0.0);
    registry->GetComponent<RigidBodyComponent>(projectile).velocity = velocity;
    registry->GetComponent<BoxColliderComponent>(projectile).collisionMask = emitter.collisionMask;
    auto& projectileComponent = registry->GetComponent<ProjectileComponent>(projectile);
    projectileComponent.damage = emitter.damage;
    projectileComponent.duration = emitter.duration;
  }
};
