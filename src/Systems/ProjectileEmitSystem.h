#pragma once

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
#include "EntityPoolSystem.h"

class ProjectileEmitSystem {
 public:
  void Init(Registry& registry) {
    projectile_pool_id_ =
        registry.Get<EntityPoolManager>().RegisterPool<ProjectileComponent>(registry, [](Registry& reg) {
          reg.Tag<PoolableTag>();
          reg.Tag<ProjectileTag>();
          // NameComponent baked into the archetype so emitter-assigned names take the cheap
          // re-add path in Registry::AddComponent (assign-over) instead of an archetype transition,
          // which would break pool reuse (pool routes by archetype id).
          return reg.CreateEntityWithBundle(EntityMaskComponent(), PositionComponent(), ScaleComponent(),
                                            RotationComponent(), GlobalTransformComponent{}, RigidBodyComponent(),
                                            BoxColliderComponent(4, 4, glm::vec2(0, 0), false, EntityMask{}),
                                            ProjectileComponent(), SpriteComponent("bullet-texture", 4.0f, 4.0f, 4),
                                            NameComponent(), PoolableTag{}, ProjectileTag{});
        });
  }

  // Lua entry point. `direction` is a non-normalized aim vector; zero falls back to the emitter's
  // configured velocity vector. Component-wise multiply matches the legacy player-fire semantics.
  void Fire(Registry& registry, Entity emitter, glm::vec2 direction) const {
    if (!registry.HasComponent<PositionComponent>(emitter) ||
        !registry.HasComponent<ProjectileEmitterComponent>(emitter)) {
      return;
    }
    const auto& position = registry.GetComponent<PositionComponent>(emitter);
    auto& emitterComp = registry.GetComponent<ProjectileEmitterComponent>(emitter);

    glm::vec2 velocity = emitterComp.velocity;
    if (glm::length(direction) > 1e-4f) {
      velocity = glm::normalize(direction) * emitterComp.velocity;
    }
    SpawnProjectile(registry, emitter, position.value, emitterComp, velocity);
  }

 private:
  ArchetypeID projectile_pool_id_{};

  void SpawnProjectile(Registry& registry, const Entity& entity, glm::vec2 spawnPosition,
                       const ProjectileEmitterComponent& emitter, glm::vec2 velocity) const {
    if (registry.HasComponent<SpriteComponent>(entity)) {
      const auto& sprite = registry.GetComponent<SpriteComponent>(entity);
      const glm::vec2 emitterScale = registry.HasComponent<ScaleComponent>(entity)
                                         ? registry.GetComponent<ScaleComponent>(entity).value
                                         : glm::vec2(1.0f, 1.0f);
      spawnPosition.x += emitterScale.x * static_cast<float>(sprite.width) / 2;
      spawnPosition.y += emitterScale.y * static_cast<float>(sprite.height) / 2;
    }

    // Use the emitter's dedicated projectile mask — NOT the emitter entity's own
    // EntityMaskComponent.  Inheriting the emitter's identity caused projectiles
    // to be "seen" by the emitter's own collision mask, destroying themselves.
    const Entity projectile = registry.Get<EntityPoolManager>().Spawn(registry, projectile_pool_id_);
    registry.GetComponent<EntityMaskComponent>(projectile).mask = emitter.projectileMask;
    registry.GetComponent<PositionComponent>(projectile).value = spawnPosition;
    registry.GetComponent<ScaleComponent>(projectile).value = glm::vec2(1.0f, 1.0f);
    registry.GetComponent<RotationComponent>(projectile).value = 0.0;
    registry.GetComponent<RigidBodyComponent>(projectile).velocity = velocity;
    registry.GetComponent<BoxColliderComponent>(projectile).collisionMask = emitter.collisionMask;
    auto& projectileComponent = registry.GetComponent<ProjectileComponent>(projectile);
    projectileComponent.damage = emitter.damage;
    projectileComponent.duration = emitter.duration;
    registry.GetComponent<NameComponent>(projectile).name = emitter.projectileName;
  }
};
