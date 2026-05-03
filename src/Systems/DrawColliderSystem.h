#pragma once

#include <SDL3/SDL.h>

#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "Components/CameraComponents.h"
#include "ECS/Iterable.h"
#include "ECS/Registry.h"
#include "Game/GameConfig.h"

class DrawColliderSystem {
 public:
  void operator()(const ContextFacade& context, const TransformComponent& transform,
                  const BoxColliderComponent& collider) const {
    const auto& gameConfig = context.Registry()->Get<GameConfig>();
    if (!gameConfig.GetEngineOptions().drawColliders) {
      return;
    }

    auto* renderer = context.Registry()->Get<SDL_Renderer*>();
    const auto& camera = context.Registry()->Get<CameraComponent>().viewport;

    const SDL_FRect rect = {transform.globalPosition.x - camera.x, transform.globalPosition.y - camera.y,
                            static_cast<float>(collider.width) * transform.globalScale.x,
                            static_cast<float>(collider.height) * transform.globalScale.y};

    SDL_SetRenderDrawColor(renderer, Constants::kUint8Max, 0, 0, Constants::kUint8Max);
    SDL_RenderRect(renderer, &rect);
  }
};
