#pragma once

#include <SDL3/SDL.h>

#include "../Components/SquarePrimitiveComponent.h"
#include "../Components/TransformComponent.h"
#include "../Renderer/RenderQueue.h"
#include "Components/CameraComponents.h"
#include "ECS/Registry.h"

class RenderPrimitiveSystem {
 public:
  void Prepare(Registry* registry) {
    camera_ = registry->Get<CameraComponent>().viewport;
    renderQueue_ = &registry->Get<RenderQueue>();
  }

  void operator()(const SquarePrimitiveComponent& square, const TransformComponent& transform) const {
    const glm::vec2 origin = square.position + transform.globalPosition;

    const float x = square.isFixed ? origin.x : origin.x - camera_.x;
    const float y = square.isFixed ? origin.y : origin.y - camera_.y;

    auto& cmd = renderQueue_->EmplaceSquare(static_cast<unsigned int>(square.layer), square.position.y);
    cmd.destRect = {x, y, square.width, square.height};
    cmd.color = square.color;
  }

 private:
  RenderQueue* renderQueue_ = nullptr;
  SDL_FRect camera_{};
};
