#pragma once

#include "../Components/PositionComponent.h"
#include "../Components/RigidBodyComponent.h"

class VelocityIntegrationSystem {
 public:
  void operator()(const float deltaTime, PositionComponent& position, const RigidBodyComponent& rigidBody) const {
    position.value.x += rigidBody.velocity.x * deltaTime;
    position.value.y += rigidBody.velocity.y * deltaTime;
  }
};
