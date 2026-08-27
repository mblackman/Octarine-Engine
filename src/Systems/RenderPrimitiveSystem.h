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
    // square.position is a local offset from the entity's transform, so it scales with the
    // entity the same way BoxColliderComponent::offset does in CollisionSystem — otherwise a
    // scaled entity's primitive grows but stays pinned at its unscaled offset.
    const glm::vec2 scaledOffset = square.position * transform.scale;
    const glm::vec2 origin = transform.position + scaledOffset;
    const float w = square.width * transform.scale.x;
    const float h = square.height * transform.scale.y;

    const bool isOutsideCamera =
        IsRenderableOutsideViewport(origin.x, origin.y, w, h, square.isFixed, camera_, windowWidth_, windowHeight_);

    if (isOutsideCamera) {
      PROFILE_COUNTER_INC(culledCounter_);
      return;
    }

    PROFILE_COUNTER_INC(emplacedCounter_);

    const float x = square.isFixed ? origin.x : origin.x - camera_.x;
    const float y = square.isFixed ? origin.y : origin.y - camera_.y;

    auto& cmd = renderQueue_->EmplaceSquare(static_cast<unsigned int>(square.layer), square.position.y, nullptr,
                                            square.blendMode);
    cmd.destRect = {x, y, w, h};
    cmd.color = SDL_Color{square.color.r, square.color.g, square.color.b, square.color.a};
    cmd.rotation = transform.rotation;
    // The quad's top-left is `origin`, but pivot is an offset from transform.position, so rebase
    // it onto the destRect the renderer receives. Both are in scaled pixels.
    cmd.pivot = {.x = transform.pivot.x - scaledOffset.x, .y = transform.pivot.y - scaledOffset.y};
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
