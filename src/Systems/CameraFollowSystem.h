#pragma once

#include <SDL3/SDL.h>

#include "../Components/CameraFollowComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"
#include "Game/GameConfig.h"
#include "General/Constants.h"

class CameraFollowSystem : public System {
 public:
  CameraFollowSystem() {
    RequireComponent<CameraFollowComponent>();
    RequireComponent<TransformComponent>();
  }

  void Update(SDL_FRect& camera) const {
    for (auto entity : GetEntities()) {
      const auto transform = entity.GetComponent<TransformComponent>();

      if (transform.position.x + camera.w / Constants::kHalf < GameConfig::GetInstance().playableAreaWidth) {
        camera.x = transform.position.x - static_cast<float>(GameConfig::GetInstance().windowWidth) / Constants::kHalf;
      }

      if (transform.position.y + camera.h / Constants::kHalf < GameConfig::GetInstance().playableAreaHeight) {
        camera.y = transform.position.y - static_cast<float>(GameConfig::GetInstance().windowHeight) / Constants::kHalf;
      }

      camera.x = std::max(0.0f, camera.x);
      camera.y = std::max(0.0f, camera.y);
      camera.x = std::min(camera.w, camera.x);
      camera.y = std::min(camera.h, camera.y);
    }
  }
};