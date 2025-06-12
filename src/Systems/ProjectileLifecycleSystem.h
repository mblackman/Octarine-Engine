#pragma once

#include "../Components/ProjectileComponent.h"
#include "../ECS/ECS.h"

class ProjectileLifecycleSystem : public System {
 public:
  ProjectileLifecycleSystem() { RequireComponent<ProjectileComponent>(); }

  ProjectileLifecycleSystem(const ProjectileLifecycleSystem&) = delete;
  ProjectileLifecycleSystem& operator=(const ProjectileLifecycleSystem&) = delete;

  ProjectileLifecycleSystem(ProjectileLifecycleSystem&&) = delete;
  ProjectileLifecycleSystem& operator=(ProjectileLifecycleSystem&&) = delete;

  ~ProjectileLifecycleSystem() = default;

  void Update(const float deltaTime) const {
    for (auto entity : GetEntities()) {
      auto& projectile = entity.GetComponent<ProjectileComponent>();

      projectile.timer += deltaTime;
      if (projectile.timer > projectile.duration) {
        entity.Blam();
      }
    }
  }
};