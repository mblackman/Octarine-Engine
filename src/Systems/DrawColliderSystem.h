#pragma once

#include <SDL3/SDL.h>

#include <cmath>

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

    const float w = static_cast<float>(collider.width) * transform.scale.x;
    const float h = static_cast<float>(collider.height) * transform.scale.y;
    const float hx = w * 0.5f;
    const float hy = h * 0.5f;
    const float cx = transform.position.x + hx - camera.x;
    const float cy = transform.position.y + hy - camera.y;

    SDL_SetRenderDrawColor(renderer, Constants::kUint8Max, 0, 0, Constants::kUint8Max);

    if (transform.rotation == 0.0) {
      const SDL_FRect rect = {cx - hx, cy - hy, w, h};
      SDL_RenderRect(renderer, &rect);
      return;
    }

    const auto c = static_cast<float>(std::cos(transform.rotation));
    const auto s = static_cast<float>(std::sin(transform.rotation));
    const SDL_FPoint corners[5] = {
        {cx + (-hx) * c - (-hy) * s, cy + (-hx) * s + (-hy) * c},
        {cx + (hx)*c - (-hy) * s, cy + (hx)*s + (-hy) * c},
        {cx + (hx)*c - (hy)*s, cy + (hx)*s + (hy)*c},
        {cx + (-hx) * c - (hy)*s, cy + (-hx) * s + (hy)*c},
        {cx + (-hx) * c - (-hy) * s, cy + (-hx) * s + (-hy) * c},
    };
    SDL_RenderLines(renderer, corners, 5);
  }
};
