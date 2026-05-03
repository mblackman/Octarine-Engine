#pragma once

struct HealthComponent {
  int currentHealth;
  int maxHealth;

  HealthComponent() = delete;

  explicit HealthComponent(const int maxHealth) : currentHealth(maxHealth), maxHealth(maxHealth) {}

  HealthComponent(const int maxHealth, const int currentHealth) : currentHealth(currentHealth), maxHealth(maxHealth) {}
};
