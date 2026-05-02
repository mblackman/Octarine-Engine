#pragma once

struct HealthComponent {
  int currentHealth;
  int maxHealth;

  HealthComponent() : currentHealth(), maxHealth() {}

  explicit HealthComponent(int maxHealth)
      : currentHealth(maxHealth), maxHealth(maxHealth) {}

  HealthComponent(int maxHealth, int currentHealth)
      : currentHealth(currentHealth), maxHealth(maxHealth) {}
};
