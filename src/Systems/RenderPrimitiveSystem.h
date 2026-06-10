#pragma once

#include <atomic>

#include "Components/CameraComponents.h"
#include "Components/GlobalTransformComponent.h"
#include "Components/SquarePrimitiveComponent.h"
#include "ECS/Registry.h"
#include "Game/GameConfig.h"
#include "General/PerfUtils.h"
#include "General/Rect.h"
#include "Renderer/RenderCulling.h"
#include "Renderer/RenderQueue.h"

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

  void operator()(const SquarePrimitiveComponent& square, const GlobalTransformComponent& transform) const {
    const glm::vec2 origin = square.position + transform.position;

    const bool isOutsideCamera = IsRenderableOutsideViewport(origin.x, origin.y, square.width, square.height,
                                                             square.isFixed, camera_, windowWidth_, windowHeight_);

    if (isOutsideCamera) {
      PROFILE_COUNTER_INC(culledCounter_);
      return;
    }

    PROFILE_COUNTER_INC(emplacedCounter_);

    const float x = square.isFixed ? origin.x : origin.x - camera_.x;
    const float y = square.isFixed ? origin.y : origin.y - camera_.y;

    auto& cmd = renderQueue_->EmplaceSquare(static_cast<unsigned int>(square.layer), square.position.y, nullptr,
                                            square.blendMode);
    cmd.destRect = {x, y, square.width, square.height};
    cmd.color = SDL_Color{square.color.r, square.color.g, square.color.b, square.color.a};
    cmd.rotation = transform.rotation;
    cmd.blendMode = octarine::ToSdlBlendMode(square.blendMode);
  }

 private:
  RenderQueue* renderQueue_ = nullptr;
  float windowWidth_ = 0;
  float windowHeight_ = 0;
  octarine::Rect camera_{};
#ifdef OCTARINE_PROFILING
  std::atomic<long long>* culledCounter_ = nullptr;
  std::atomic<long long>* emplacedCounter_ = nullptr;
#endif
};
