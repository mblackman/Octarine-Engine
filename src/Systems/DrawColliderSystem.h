#pragma once

#include <SDL3/SDL.h>

#include "../Components/BoxColliderComponent.h"
#include "../Components/GlobalTransformComponent.h"
#include "../General/Constants.h"
#include "Components/CameraComponents.h"
#include "ECS/Iterable.h"
#include "ECS/Registry.h"

class DrawColliderSystem {
 public:
  void operator()(const ContextFacade& context, const GlobalTransformComponent& transform,
                  const BoxColliderComponent& collider) const {
    auto* renderer = context.GetRegistry()->Get<SDL_Renderer*>();
    const auto& camera = context.GetRegistry()->Get<CameraComponent>().viewport;

    const SDL_FRect rect = {transform.position.x - camera.x, transform.position.y - camera.y,
                            static_cast<float>(collider.width) * transform.scale.x,
                            static_cast<float>(collider.height) * transform.scale.y};

    SDL_SetRenderDrawColor(renderer, Constants::kUint8Max, 0, 0, Constants::kUint8Max);
    SDL_RenderRect(renderer, &rect);
  }
};
