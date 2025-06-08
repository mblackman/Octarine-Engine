#pragma once

#include <SDL3/SDL.h>

#include "../Components/CameraFollowComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"

class CameraFollowSystem : public System {
 public:
  CameraFollowSystem() {
    RequireComponent<CameraFollowComponent>();
    RequireComponent<TransformComponent>();
  }

  void Update(SDL_FRect& camera) const {
    for (auto entity : GetEntities()) {
      constexpr float half = 2.0f;
      const auto transform = entity.GetComponent<TransformComponent>();

      if (transform.position.x + camera.w / half < Game::mapWidth) {
        camera.x = transform.position.x - static_cast<float>(Game::windowWidth) / half;
      }

      if (transform.position.y + camera.h / half < Game::mapHeight) {
        camera.y = transform.position.y - static_cast<float>(Game::windowHeight) / half;
      }

      camera.x = std::max(0.0f, camera.x);
      camera.y = std::max(0.0f, camera.y);
      camera.x = std::min(camera.w, camera.x);
      camera.y = std::min(camera.h, camera.y);
    }
  }
};