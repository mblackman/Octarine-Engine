#pragma once

#include <SDL3/SDL.h>

#include "../Components/CameraFollowComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/System.h"
#include "Components/CameraComponents.h"
#include "ECS/Iterable.h"
#include "ECS/Registry.h"
#include "Game/GameConfig.h"
#include "General/Constants.h"

class CameraFollowSystem {
 public:
  void operator()(const ContextFacade& context, const TransformComponent& transform,
                  CameraFollowComponent& cameraFollow) const {
    auto& camera = context.Registry()->Get<CameraComponent>();
    const auto& gameConfig = context.Registry()->Get<GameConfig>();

    if (transform.position.x + camera.viewport.w / Constants::kHalf < gameConfig.playableAreaWidth) {
      camera.viewport.x = transform.position.x - static_cast<float>(gameConfig.windowWidth) / Constants::kHalf;
    }

    if (transform.position.y + camera.viewport.h / Constants::kHalf < gameConfig.playableAreaHeight) {
      camera.viewport.y = transform.position.y - static_cast<float>(gameConfig.windowHeight) / Constants::kHalf;
    }

    camera.viewport.x = std::max(0.0f, camera.viewport.x);
    camera.viewport.y = std::max(0.0f, camera.viewport.y);
    camera.viewport.x = std::min(camera.viewport.w, camera.viewport.x);
    camera.viewport.y = std::min(camera.viewport.h, camera.viewport.y);
  }
};