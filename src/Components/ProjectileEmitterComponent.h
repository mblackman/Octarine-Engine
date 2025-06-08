#pragma once

#include <SDL3/SDL.h>

#include <glm/glm.hpp>

struct ProjectileEmitterComponent {
  glm::vec2 velocity;
  int duration;
  int frequency;
  int damage;
  bool isFriendly;
  int lastEmissionTime;

  explicit ProjectileEmitterComponent(const glm::vec2 velocity = glm::vec2(0, 0), const int duration = 10000,
                                      const int frequency = 1000, const int damage = 10, const bool isFriendly = true)
      : velocity(velocity),
        duration(duration),
        frequency(frequency),
        damage(damage),
        isFriendly(isFriendly),
        lastEmissionTime(SDL_GetTicks()) {}
};