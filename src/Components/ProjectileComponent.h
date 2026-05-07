#pragma once

struct ProjectileComponent {
  int damage;
  float timer;
  float duration;

  explicit ProjectileComponent(const int t_damage = 0, const float t_duration = 0.0f)
      : damage(t_damage), timer(0.0f), duration(t_duration) {}
};
