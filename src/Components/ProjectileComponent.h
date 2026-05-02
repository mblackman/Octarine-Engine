#pragma once

struct ProjectileComponent {
  int damage;
  float timer;
  float duration;
  bool isFriendly;

  explicit ProjectileComponent(const int damage = 0, const float duration = 0.0f, const bool isFriendly = false)
      : damage(damage), timer(0.0f), duration(duration), isFriendly(isFriendly) {}
};
