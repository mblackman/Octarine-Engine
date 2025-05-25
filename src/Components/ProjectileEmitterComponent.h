#pragma once

#include <SDL2/SDL.h>

#include <glm/glm.hpp>

struct ProjectileEmitterComponent {
    glm::vec2 velocity;
    int duration;
    int frequency;
    int damage;
    bool isFriendly;
    int lastEmissionTime;

    ProjectileEmitterComponent(
        glm::vec2 velocity = glm::vec2(0, 0),
        int duration = 10000,
        int frequency = 1000,
        int damage = 10,
        bool isFriendly = true) : velocity(velocity),
                                  duration(duration),
                                  frequency(frequency),
                                  damage(damage),
                                  isFriendly(isFriendly),
                                  lastEmissionTime(SDL_GetTicks()) {
    }
};