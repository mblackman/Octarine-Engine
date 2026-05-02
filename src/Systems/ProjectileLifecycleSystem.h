#pragma once

#include "../Components/ProjectileComponent.h"
#include "../ECS/ECS.h"
#include "../General/Logger.h"

class ProjectileLifecycleSystem : public System {
   public:
    ProjectileLifecycleSystem() {
        RequireComponent<ProjectileComponent>();
    }

    ~ProjectileLifecycleSystem() = default;

    void Update() {
        for (auto entity : GetEntities()) {
            auto projectile = entity.GetComponent<ProjectileComponent>();

            if (static_cast<int>(SDL_GetTicks()) - projectile.spawnTime > projectile.duration) {
                entity.Blam();
            }
        }
    }
};