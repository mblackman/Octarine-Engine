#pragma once

#include <glm/glm.hpp>

struct ProjectileEmitterComponent {
  glm::vec2 velocity;
  float duration;
  int frequency;
  int damage;
  bool isFriendly;
  double countDownTimer;

  explicit ProjectileEmitterComponent(const glm::vec2 velocity = glm::vec2(0, 0), const float duration = 1.0f,
                                      const int frequency = 1000, const int damage = 10, const bool isFriendly = true)
      : velocity(velocity),
        duration(duration),
        frequency(frequency),
        damage(damage),
        isFriendly(isFriendly),
        countDownTimer(static_cast<double>(frequency)) {}
};