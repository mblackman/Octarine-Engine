#pragma once

#include <SDL3/SDL.h>

#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../Renderer/RenderKey.h"
#include "../Renderer/RenderQueue.h"
#include "../Renderer/RenderableType.h"
#include "Components/CameraComponents.h"
#include "ECS/Iterable.h"
#include "ECS/Registry.h"
#include "Game/GameConfig.h"

class RenderSpriteSystem {
 public:
  void operator()(const ContextFacade& context, const TransformComponent& transform,
                  const SpriteComponent& sprite) const {
    const auto& gameConfig = context.Registry()->Get<GameConfig>();
    auto& renderQueue = context.Registry()->Get<RenderQueue>();
    const auto& camera = context.Registry()->Get<CameraComponent>().viewport;
    bool isOutsideCamera = false;

    if (sprite.isFixed) {
      const auto windowWidth = static_cast<float>(gameConfig.windowWidth);
      const auto windowHeight = static_cast<float>(gameConfig.windowHeight);
      isOutsideCamera = transform.globalPosition.x + sprite.width * transform.globalScale.x < 0 ||
                        transform.globalPosition.x > windowWidth ||
                        transform.globalPosition.y + sprite.height * transform.globalScale.y < 0 ||
                        transform.globalPosition.y > windowHeight;
    } else {
      isOutsideCamera = transform.globalPosition.x + sprite.width * transform.globalScale.x < camera.x ||
                        transform.globalPosition.x > camera.x + camera.w ||
                        transform.globalPosition.y + sprite.height * transform.globalScale.y < camera.y ||
                        transform.globalPosition.y > camera.y + camera.h;
    }

    if (!isOutsideCamera) {
      const RenderKey renderKey(sprite.layer, transform.globalPosition.y, SPRITE, context.Entity());

      renderQueue.AddRenderKey(renderKey);
    }
  }
};