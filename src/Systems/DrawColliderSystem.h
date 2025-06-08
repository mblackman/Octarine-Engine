#pragma once

#include <SDL3/SDL.h>

#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"

class DrawColliderSystem : public System {
 public:
  DrawColliderSystem() {
    RequireComponent<TransformComponent>();
    RequireComponent<BoxColliderComponent>();
  }

  ~DrawColliderSystem() = default;

  void Update(SDL_Renderer* renderer, const SDL_Rect& camera) {
    for (auto entity : GetEntities()) {
      const auto& transform = entity.GetComponent<TransformComponent>();
      const auto& collider = entity.GetComponent<BoxColliderComponent>();
      constexpr Uint8 uint8Max = 255;

      const SDL_FRect rect = {transform.position.x - static_cast<float>(camera.x),
                              transform.position.y - static_cast<float>(camera.y), collider.width * transform.scale.x,
                              collider.height * transform.scale.y};

      SDL_SetRenderDrawColor(renderer, uint8Max, 0, 0, uint8Max);
      SDL_RenderRect(renderer, &rect);
    }
  }
};