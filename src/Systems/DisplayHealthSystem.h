#pragma once

#include <SDL3/SDL.h>

#include <string>

#include "../Components/HealthComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/SquarePrimitiveComponent.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/Iterable.h"
#include "../ECS/Registry.h"

class DisplayHealthSystem {
 public:
  void operator()(const ContextFacade& ctx, const HealthComponent& health, TextLabelComponent& textLabel,
                  SquarePrimitiveComponent& square) const {
    const auto* registry = ctx.Registry();

    if (health.maxHealth <= 0) {
      return;
    }

    const float healthPercentage = static_cast<float>(health.currentHealth) / static_cast<float>(health.maxHealth);
    const int healthAmount = static_cast<int>(healthPercentage * 100);
    textLabel.text = std::to_string(healthAmount) + "%";
    textLabel.color = GetHealthColor(healthPercentage);

    int healthWidth = healthAmount;

    if (const auto parent = registry->GetParent(ctx.Entity());
        parent.has_value() && registry->HasComponent<SpriteComponent>(parent.value())) {
      const auto& sprite = registry->GetComponent<SpriteComponent>(parent.value());
      const auto& transform = registry->GetComponent<TransformComponent>(parent.value());
      healthWidth = static_cast<int>(sprite.width * healthPercentage * transform.scale.x);
    }

    square.width = static_cast<float>(healthWidth);
    square.color = GetHealthColor(healthPercentage);
  }

 private:
  static constexpr SDL_Color kLowHealthColor = {255, 0, 0, 255};
  static constexpr SDL_Color kMediumHealthColor = {255, 255, 0, 255};
  static constexpr SDL_Color kHighHealthColor = {0, 255, 0, 255};

  [[nodiscard]] static SDL_Color GetHealthColor(const float healthPercentage) {
    if (healthPercentage > 0.66F) {
      return kHighHealthColor;
    }
    if (healthPercentage > 0.33F) {
      return kMediumHealthColor;
    }
    return kLowHealthColor;
  }
};
