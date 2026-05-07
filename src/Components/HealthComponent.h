#pragma once

struct HealthComponent {
  int currentHealth;
  int maxHealth;

  HealthComponent() = delete;

  explicit HealthComponent(const int t_maxHealth) : currentHealth(t_maxHealth), maxHealth(t_maxHealth) {}

  HealthComponent(const int t_maxHealth, const int t_currentHealth)
      : currentHealth(t_currentHealth), maxHealth(t_maxHealth) {}
};
