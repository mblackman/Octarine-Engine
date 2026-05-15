#pragma once

#include <SDL3/SDL.h>

#include <atomic>

#include "../Components/SquarePrimitiveComponent.h"
#include "../Components/TransformComponent.h"
#include "../General/PerfUtils.h"
#include "../Renderer/RenderCulling.h"
#include "../Renderer/RenderQueue.h"
#include "Components/CameraComponents.h"
#include "ECS/Registry.h"
#include "Game/GameConfig.h"

class RenderPrimitiveSystem {
 public:
  void Prepare(Registry* registry) {
    const auto& gameConfig = registry->Get<GameConfig>();
    camera_ = registry->Get<CameraComponent>().viewport;
    renderQueue_ = &registry->Get<RenderQueue>();
    windowWidth_ = static_cast<float>(gameConfig.windowWidth);
    windowHeight_ = static_cast<float>(gameConfig.windowHeight);
#ifdef OCTARINE_PROFILING
    if (!culledCounter_) culledCounter_ = PROFILE_COUNTER_HANDLE("RenderPrimitive: Culled");
    if (!emplacedCounter_) emplacedCounter_ = PROFILE_COUNTER_HANDLE("RenderPrimitive: Emplaced");
#endif
  }

  void operator()(const SquarePrimitiveComponent& square, const TransformComponent& transform) const {
    const glm::vec2 origin = square.position + transform.globalPosition;

    const bool isOutsideCamera = IsRenderableOutsideViewport(origin.x, origin.y, square.width, square.height,
                                                             square.isFixed, camera_, windowWidth_, windowHeight_);

    if (isOutsideCamera) {
      PROFILE_COUNTER_INC(culledCounter_);
      return;
    }

    PROFILE_COUNTER_INC(emplacedCounter_);

    const float x = square.isFixed ? origin.x : origin.x - camera_.x;
    const float y = square.isFixed ? origin.y : origin.y - camera_.y;

    auto& cmd = renderQueue_->EmplaceSquare(static_cast<unsigned int>(square.layer), square.position.y);
    cmd.destRect = {x, y, square.width, square.height};
    cmd.color = square.color;
  }

 private:
  RenderQueue* renderQueue_ = nullptr;
  float windowWidth_ = 0;
  float windowHeight_ = 0;
  SDL_FRect camera_{};
#ifdef OCTARINE_PROFILING
  std::atomic<long long>* culledCounter_ = nullptr;
  std::atomic<long long>* emplacedCounter_ = nullptr;
#endif
};
