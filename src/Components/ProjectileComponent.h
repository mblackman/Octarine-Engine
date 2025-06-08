#pragma once

struct ProjectileComponent {
  int damage;
  int spawnTime;
  int duration;
  bool isFriendly;

  explicit ProjectileComponent(const int damage = 0, const int spawnTime = 0, const int duration = 0,
                               const bool isFriendly = false)
      : damage(damage), spawnTime(spawnTime), duration(duration), isFriendly(isFriendly) {}
};
