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

  DrawColliderSystem(const DrawColliderSystem&) = delete;
  DrawColliderSystem& operator=(const DrawColliderSystem&) = delete;

  DrawColliderSystem(DrawColliderSystem&&) = delete;
  DrawColliderSystem& operator=(DrawColliderSystem&&) = delete;

  ~DrawColliderSystem() = default;

  void Update(SDL_Renderer* renderer, const SDL_FRect& camera) const {
    for (auto entity : GetEntities()) {
      const auto& transform = entity.GetComponent<TransformComponent>();
      const auto& collider = entity.GetComponent<BoxColliderComponent>();

      const SDL_FRect rect = {transform.position.x - static_cast<float>(camera.x),
                              transform.position.y - static_cast<float>(camera.y),
                              static_cast<float>(collider.width) * transform.scale.x,
                              static_cast<float>(collider.height) * transform.scale.y};

      SDL_SetRenderDrawColor(renderer, Constants::kUnt8Max, 0, 0, Constants::kUnt8Max);
      SDL_RenderRect(renderer, &rect);
    }
  }
};