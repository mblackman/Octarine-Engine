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
  // Entity mask assigned to spawned projectiles.  Determines what OTHER
  // entities' collision masks can match against the projectile.  Defaults to
  // empty (no layer) so that projectiles are "invisible" — only the
  // projectile's own collisionMask drives hits.
  EntityMask projectileMask;

  explicit ProjectileEmitterComponent(const glm::vec2 t_velocity = glm::vec2(0, 0), const float t_duration = 1.0f,
                                      const float t_frequency = 1.0f, const int t_damage = 10,
                                      const EntityMask t_collisionMask = Constants::kDefaultEntityMask,
                                      const EntityMask t_projectileMask = EntityMask{})
      : velocity(t_velocity),
        duration(t_duration),
        frequency(t_frequency),
        damage(t_damage),
        countDownTimer(t_frequency),
        collisionMask(t_collisionMask),
        projectileMask(t_projectileMask) {}
};