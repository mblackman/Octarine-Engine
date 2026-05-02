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

  void Update(SDL_Rect& camera) const {
    for (auto entity : GetEntities()) {
      const auto transform = entity.GetComponent<TransformComponent>();

      const int posX = static_cast<int>(transform.position.x);
      const int posY = static_cast<int>(transform.position.y);
      if (static_cast<float>(posX + camera.w / 2) < Game::mapWidth) {
        camera.x = posX - Game::windowWidth / 2;
      }

      if (static_cast<float>(posY + camera.h / 2) < Game::mapHeight) {
        camera.y = posY - Game::windowHeight / 2;
      }

      camera.x = std::max(0, camera.x);
      camera.y = std::max(0, camera.y);
      camera.x = std::min(camera.w, camera.x);
      camera.y = std::min(camera.h, camera.y);
    }
  }
};