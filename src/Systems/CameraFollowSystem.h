#pragma once

#include <SDL3/SDL.h>

#include "../Components/CameraFollowComponent.h"
#include "../Components/TransformComponent.h"
#include "Components/CameraComponents.h"
#include "ECS/Iterable.h"
#include "ECS/Registry.h"
#include "Game/GameConfig.h"
#include "General/Constants.h"

class CameraFollowSystem {
 public:
  void operator()(const ContextFacade& context, const TransformComponent& transform,
                  CameraFollowComponent& cameraFollow) const {
    auto& [viewport] = context.Registry()->Get<CameraComponent>();
    const auto& gameConfig = context.Registry()->Get<GameConfig>();

    if (transform.position.x + viewport.w / Constants::kHalf < gameConfig.playableAreaWidth) {
      viewport.x = transform.position.x - static_cast<float>(gameConfig.windowWidth) / Constants::kHalf;
    }

    if (transform.position.y + viewport.h / Constants::kHalf < gameConfig.playableAreaHeight) {
      viewport.y = transform.position.y - static_cast<float>(gameConfig.windowHeight) / Constants::kHalf;
    }

    viewport.x = std::max(0.0f, viewport.x);
    viewport.y = std::max(0.0f, viewport.y);
    viewport.x = std::min(viewport.w, viewport.x);
    viewport.y = std::min(viewport.h, viewport.y);
  }
};