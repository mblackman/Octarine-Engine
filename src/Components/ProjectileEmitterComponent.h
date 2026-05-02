#pragma once

#include <glm/glm.hpp>

struct ProjectileEmitterComponent {
  glm::vec2 velocity;
  float duration;
  float frequency;
  int damage;
  bool isFriendly;
  float countDownTimer;

  explicit ProjectileEmitterComponent(const glm::vec2 velocity = glm::vec2(0, 0), const float duration = 1.0f,
                                      const float frequency = 1.0f, const int damage = 10, const bool isFriendly = true)
      : velocity(velocity),
        duration(duration),
        frequency(frequency),
        damage(damage),
        isFriendly(isFriendly),
        countDownTimer(frequency) {}
};