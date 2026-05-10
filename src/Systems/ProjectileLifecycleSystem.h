#pragma once

#include "../Components/ProjectileComponent.h"
#include "ECS/CommandBuffer.h"

class ProjectileLifecycleSystem {
 public:
  CommandBuffer& GetCommandBuffer() { return cmd_buffer_; }

  void operator()(const Entity entity, const float deltaTime, ProjectileComponent& projectile) {
    projectile.timer += deltaTime;
    if (projectile.timer > projectile.duration) {
      cmd_buffer_.EmplaceDespawn(entity);
    }
  }

 private:
  CommandBuffer cmd_buffer_;
};
