#pragma once

struct ProjectileComponent {
  int damage;
  float timer;
  float duration;

  explicit ProjectileComponent(const int damage = 0, const float duration = 0.0f)
      : damage(damage), timer(0.0f), duration(duration) {}
};
