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
  void operator()(const ContextFacade& context, const Iterable& iterable) const {
    auto* registry = context.Registry();
    const auto& gameConfig = registry->Get<GameConfig>();
    auto& renderQueue = registry->Get<RenderQueue>();
    const auto& [x, y, w, h] = registry->Get<CameraComponent>().viewport;
    const auto windowWidth = static_cast<float>(gameConfig.windowWidth);
    const auto windowHeight = static_cast<float>(gameConfig.windowHeight);

    for (auto&& ctx : iterable) {
      const auto& transform = ctx.Component<TransformComponent>();
      const auto& sprite = ctx.Component<SpriteComponent>();

      bool isOutsideCamera = false;

      if (sprite.isFixed) {
        isOutsideCamera = transform.globalPosition.x + sprite.width * transform.globalScale.x < 0 ||
                          transform.globalPosition.x > windowWidth ||
                          transform.globalPosition.y + sprite.height * transform.globalScale.y < 0 ||
                          transform.globalPosition.y > windowHeight;
      } else {
        isOutsideCamera = transform.globalPosition.x + sprite.width * transform.globalScale.x < x ||
                          transform.globalPosition.x > x + w ||
                          transform.globalPosition.y + sprite.height * transform.globalScale.y < y ||
                          transform.globalPosition.y > y + h;
      }

      if (!isOutsideCamera) {
        const RenderKey renderKey(sprite.layer, transform.globalPosition.y, SPRITE, ctx.Entity());
        renderQueue.AddRenderKey(renderKey);
      }
    }
  }
};