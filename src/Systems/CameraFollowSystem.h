#pragma once

#include <SDL3/SDL.h>

#include <algorithm>

#include "../Components/CameraFollowComponent.h"
#include "../Components/PositionComponent.h"
#include "Components/CameraComponents.h"
#include "ECS/Iterable.h"
#include "ECS/Registry.h"
#include "Game/GameConfig.h"
#include "General/Constants.h"

class CameraFollowSystem {
 public:
  void operator()(const ContextFacade& context, const PositionComponent& position,
                  const CameraFollowComponent& /*follow*/) const {
    auto& [viewport] = context.GetRegistry()->Get<CameraComponent>();
    const auto& gameConfig = context.GetRegistry()->Get<GameConfig>();

    if (position.value.x + viewport.w / Constants::kTwo < gameConfig.playableAreaWidth) {
      viewport.x = position.value.x - static_cast<float>(gameConfig.windowWidth) / Constants::kTwo;
    }

    if (position.value.y + viewport.h / Constants::kTwo < gameConfig.playableAreaHeight) {
      viewport.y = position.value.y - static_cast<float>(gameConfig.windowHeight) / Constants::kTwo;
    }

    const float maxX = std::max(0.0f, gameConfig.playableAreaWidth - viewport.w);
    const float maxY = std::max(0.0f, gameConfig.playableAreaHeight - viewport.h);
    viewport.x = std::clamp(viewport.x, 0.0f, maxX);
    viewport.y = std::clamp(viewport.y, 0.0f, maxY);
  }
};