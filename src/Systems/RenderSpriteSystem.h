#pragma once

#include <SDL3/SDL.h>

#include "../AssetManager/AssetManager.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../Renderer/RenderKey.h"
#include "../Renderer/RenderQueue.h"
#include "../Renderer/RenderableType.h"
#include "Components/CameraComponents.h"
#include "ECS/Iterable.h"
#include "ECS/Query.h"
#include "ECS/Registry.h"
#include "Game/GameConfig.h"

class RenderSpriteSystem {
 public:
  void operator()(const ContextFacade& context, const Iterable& /*iterable*/) {
    auto* registry = context.Registry();
    // Hoist all singleton lookups before the per-entity loop.
    const auto& gameConfig = registry->Get<GameConfig>();
    auto& renderQueue = registry->Get<RenderQueue>();
    const auto& camera = registry->Get<CameraComponent>().viewport;
    const auto& assetManager = registry->Get<AssetManager>();
    const auto windowWidth = static_cast<float>(gameConfig.windowWidth);
    const auto windowHeight = static_cast<float>(gameConfig.windowHeight);

    if (!query_) {
      query_ = registry->CreateQuery<TransformComponent, SpriteComponent>();
    }
    query_->Update();

    query_->ForEach([&](const Entity entity, const TransformComponent& transform, SpriteComponent& sprite) {
      bool isOutsideCamera = false;

      if (sprite.isFixed) {
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
        // Cache the texture pointer on the sprite if not yet resolved.
        if (!sprite.cachedTexture) {
          sprite.cachedTexture = assetManager.GetTexture(sprite.assetId);
        }

        RenderKey renderKey(sprite.layer, transform.globalPosition.y, SPRITE, entity);
        // Pre-compute render data so Renderer doesn't need GetComponent lookups.
        const float x = sprite.isFixed ? transform.globalPosition.x : transform.globalPosition.x - camera.x;
        const float y = sprite.isFixed ? transform.globalPosition.y : transform.globalPosition.y - camera.y;
        renderKey.destX = x;
        renderKey.destY = y;
        renderKey.destW = sprite.width * transform.scale.x;
        renderKey.destH = sprite.height * transform.scale.y;
        renderKey.srcRect = sprite.srcRect;
        renderKey.rotation = transform.rotation;
        renderKey.flip = sprite.flip;
        renderKey.texture = sprite.cachedTexture;
        renderKey.isFixed = sprite.isFixed;

        renderQueue.AddRenderKey(renderKey);
      }
    });
  }

 private:
  std::unique_ptr<ComponentQuery<TransformComponent, SpriteComponent>> query_;
};