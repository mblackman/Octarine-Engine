#pragma once

#include <glm/glm.hpp>

#include "General/Constants.h"

struct ProjectileEmitterComponent {
  glm::vec2 velocity;
  float duration;
  float frequency;
  int damage;
  float countDownTimer;
  EntityMask collisionMask;

  explicit ProjectileEmitterComponent(const glm::vec2 velocity = glm::vec2(0, 0), const float duration = 1.0f,
                                      const float frequency = 1.0f, const int damage = 10,
                                      const EntityMask collisionMask = Constants::kDefaultEntityMask)
      : velocity(velocity),
        duration(duration),
        frequency(frequency),
        damage(damage),
        countDownTimer(frequency),
        collisionMask(collisionMask) {}
};