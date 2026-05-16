#pragma once

#include <SDL3/SDL_keycode.h>

#include <glm/glm.hpp>

#include "../Components/BoxColliderComponent.h"
#include "../Components/EntityMaskComponent.h"
#include "../Components/GlobalTransformComponent.h"
#include "../Components/NameComponent.h"
#include "../Components/PositionComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/RotationComponent.h"
#include "../Components/ScaleComponent.h"
#include "../Components/SpriteComponent.h"
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
          reg.Tag<PoolableTag>();
          reg.Tag<ProjectileTag>();
          // NameComponent baked into the archetype so emitter-assigned names take the cheap
          // re-add path in Registry::AddComponent (assign-over) instead of an archetype transition,
          // which would break pool reuse (pool routes by archetype id).
          return reg.CreateEntityWithBundle(EntityMaskComponent(), PositionComponent(), ScaleComponent(),
                                            RotationComponent(), GlobalTransformComponent{}, RigidBodyComponent(),
                                            BoxColliderComponent(4, 4, glm::vec2(0, 0), EntityMask{}),
                                            ProjectileComponent(), SpriteComponent("bullet-texture", 4.0f, 4.0f, 4),
                                            NameComponent(), PoolableTag{}, ProjectileTag{});
        });
    eventBus->SubscribeEvent<ProjectileEmitSystem, KeyInputEvent>(this, &ProjectileEmitSystem::OnKeyInput);
  }

  void operator()(const ContextFacade& context, const PositionComponent& position,
                  ProjectileEmitterComponent& emitter) {
    const auto registry = context.GetRegistry();
    const bool isPlayer = registry->HasTag(context.GetEntity(), "player");

    if (!isPlayer) {
      emitter.countDownTimer -= context.GetDeltaTime();

      if (emitter.countDownTimer <= 0.0f) {
        SpawnProjectile(position, context.GetEntity(), registry, emitter);
        emitter.countDownTimer = emitter.frequency;
      }
    } else if (spawnFriendlyProjectiles_) {
      SpawnProjectile(position, context.GetEntity(), registry, emitter, true);
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

  void SpawnProjectile(const PositionComponent& position, const Entity& entity, Registry* registry,
                       ProjectileEmitterComponent& emitter, bool isPlayer = false) const {
    auto projectilePosition = position.value;
    auto velocity = emitter.velocity;

    if (registry->HasComponent<SpriteComponent>(entity)) {
      const auto& sprite = registry->GetComponent<SpriteComponent>(entity);
      const glm::vec2 emitterScale = registry->HasComponent<ScaleComponent>(entity)
                                         ? registry->GetComponent<ScaleComponent>(entity).value
                                         : glm::vec2(1.0f, 1.0f);
      projectilePosition.x += emitterScale.x * static_cast<float>(sprite.width) / 2;
      projectilePosition.y += emitterScale.y * static_cast<float>(sprite.height) / 2;
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

    // Use the emitter's dedicated projectile mask — NOT the emitter entity's own
    // EntityMaskComponent.  Inheriting the emitter's identity caused projectiles
    // to be "seen" by the emitter's own collision mask, destroying themselves.
    const Entity projectile = registry->Get<EntityPoolManager>().Spawn(*registry, projectile_pool_id_);
    registry->GetComponent<EntityMaskComponent>(projectile).mask = emitter.projectileMask;
    registry->GetComponent<PositionComponent>(projectile).value = projectilePosition;
    registry->GetComponent<ScaleComponent>(projectile).value = glm::vec2(1.0f, 1.0f);
    registry->GetComponent<RotationComponent>(projectile).value = 0.0;
    registry->GetComponent<RigidBodyComponent>(projectile).velocity = velocity;
    registry->GetComponent<BoxColliderComponent>(projectile).collisionMask = emitter.collisionMask;
    auto& projectileComponent = registry->GetComponent<ProjectileComponent>(projectile);
    projectileComponent.damage = emitter.damage;
    projectileComponent.duration = emitter.duration;
    registry->GetComponent<NameComponent>(projectile).name = emitter.projectileName;
  }
};
