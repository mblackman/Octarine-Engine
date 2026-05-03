#pragma once

#include "../Components/ProjectileComponent.h"
#include "ECS/EntityCommandBuffer.h"

class ProjectileLifecycleSystem {
 public:
  EntityCommandBuffer& GetCommandBuffer() { return cmd_buffer_; }

  void operator()(const Entity entity, const float deltaTime, ProjectileComponent& projectile) {
    projectile.timer += deltaTime;
    if (projectile.timer > projectile.duration) {
      cmd_buffer_.QueueBlam(entity);
    }
  }

 private:
  EntityCommandBuffer cmd_buffer_;
};
