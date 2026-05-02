#pragma once

#include "../Components/ProjectileComponent.h"
#include "../ECS/ECS.h"

class ProjectileLifecycleSystem : public System {
 public:
  ProjectileLifecycleSystem() { RequireComponent<ProjectileComponent>(); }

  ~ProjectileLifecycleSystem() = default;

  void Update() {
    for (auto entity : GetEntities()) {
      const auto projectile = entity.GetComponent<ProjectileComponent>();

      if (static_cast<int>(SDL_GetTicks()) - projectile.spawnTime >
          projectile.duration) {
        entity.Blam();
      }
    }
  }
};