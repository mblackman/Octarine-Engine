#pragma once

struct ProjectileComponent {
    int damage;
    int spawnTime;
    int duration;
    bool isFriendly;

    ProjectileComponent(
        int damage = 0,
        int spawnTime = 0,
        int duration = 0,
        bool isFriendly = false) : damage(damage),
                                   spawnTime(spawnTime),
                                   duration(duration),
                                   isFriendly(isFriendly) {
    }
};
