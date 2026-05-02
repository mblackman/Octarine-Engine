#pragma once

#include "../Components/ProjectileComponent.h"
#include "ECS/Iterable.h"
#include "ECS/Registry.h"

class ProjectileLifecycleSystem {
 public:
  void operator()(const ContextFacade& context, ProjectileComponent& projectile) const {
    projectile.timer += context.DeltaTime();
    if (projectile.timer > projectile.duration) {
      context.Registry()->QueueBlamEntity(context.Entity());
    }
  }
};
