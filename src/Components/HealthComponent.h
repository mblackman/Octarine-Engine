#pragma once

#include <algorithm>

struct HealthComponent {
  int currentHealth;
  int maxHealth;

  HealthComponent() = delete;

  explicit HealthComponent(const int t_maxHealth) : currentHealth(t_maxHealth), maxHealth(t_maxHealth) {}

  HealthComponent(const int t_maxHealth, const int t_currentHealth)
      : currentHealth(t_currentHealth), maxHealth(t_maxHealth) {}

  void Damage(const int amount) { currentHealth = std::max(0, currentHealth - amount); }
  void Heal(const int amount) { currentHealth = std::min(maxHealth, currentHealth + amount); }
  [[nodiscard]] bool IsDead() const { return currentHealth <= 0; }
  [[nodiscard]] float Fraction() const {
    return maxHealth > 0 ? static_cast<float>(currentHealth) / static_cast<float>(maxHealth) : 0.0F;
  }
};
