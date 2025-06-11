#pragma once

#include <SDL3/SDL.h>

#include "../Components/HealthComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/SquarePrimitiveComponent.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"

class DisplayHealthSystem : public System {
  SDL_Color low_health_color = {255, 0, 0};
  SDL_Color medium_health_color = {255, 255, 0};
  SDL_Color high_health_color = {0, 255, 0};

 public:
  DisplayHealthSystem() {
    RequireComponent<HealthComponent>();
    RequireComponent<TextLabelComponent>();
    RequireComponent<SquarePrimitiveComponent>();
  }

  DisplayHealthSystem(const DisplayHealthSystem&) = delete;
  DisplayHealthSystem& operator=(const DisplayHealthSystem&) = delete;

  DisplayHealthSystem(DisplayHealthSystem&&) = delete;
  DisplayHealthSystem& operator=(DisplayHealthSystem&&) = delete;

  ~DisplayHealthSystem() = default;

  void Update(const std::unique_ptr<Registry>& registry) const {
    for (auto entity : GetEntities()) {
      const auto health = entity.GetComponent<HealthComponent>();
      auto& textLabel = entity.GetComponent<TextLabelComponent>();
      auto& square = entity.GetComponent<SquarePrimitiveComponent>();
      const auto parent = registry->GetParent(entity);

      const float healthPercentage = static_cast<float>(health.currentHealth) / health.maxHealth;
      const int healthAmount = static_cast<int>(healthPercentage * 100);
      textLabel.text = std::to_string(healthAmount) + "%";
      textLabel.color = GetHealthColor(healthPercentage);

      int healthWidth = healthAmount;

      if (parent.has_value() && parent.value().HasComponent<SpriteComponent>()) {
        const auto sprite = parent.value().GetComponent<SpriteComponent>();
        const auto transform = parent.value().GetComponent<TransformComponent>();
        healthWidth = static_cast<int>(sprite.width * healthPercentage * transform.scale.x);
      }

      square.width = healthWidth;
      square.color = GetHealthColor(healthPercentage);
    }
  }

 private:
  [[nodiscard]] SDL_Color GetHealthColor(float healthPercentage) const {
    if (healthPercentage > 0.66) {
      return high_health_color;
    }
    if (healthPercentage > 0.33) {
      return medium_health_color;
    }
    return low_health_color;
  }
};