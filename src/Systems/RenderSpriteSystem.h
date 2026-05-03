#pragma once

#include <SDL3/SDL.h>

#include "../AssetManager/AssetManager.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../Renderer/RenderKey.h"
#include "../Renderer/RenderableType.h"
#include "ECS/Registry.h"
#include "Game/GameConfig.h"

class RenderSpriteSystem {
 public:
  CommandBuffer& GetCommandBuffer() { return cmd_buffer_; }

  void Prepare(Registry* registry) {
    const auto& gameConfig = registry->Get<GameConfig>();
    camera_ = registry->Get<CameraComponent>().viewport;
    assetManager_ = &registry->Get<AssetManager>();
    windowWidth_ = static_cast<float>(gameConfig.windowWidth);
    windowHeight_ = static_cast<float>(gameConfig.windowHeight);
  }

  void operator()(const Entity entity, const TransformComponent& transform, const SpriteComponent& sprite) const {
    bool isOutsideCamera = false;

    if (sprite.isFixed) {
      isOutsideCamera = transform.globalPosition.x + sprite.width * transform.globalScale.x < 0 ||
                        transform.globalPosition.x > windowWidth_ ||
                        transform.globalPosition.y + sprite.height * transform.globalScale.y < 0 ||
                        transform.globalPosition.y > windowHeight_;
    } else {
      isOutsideCamera = transform.globalPosition.x + sprite.width * transform.globalScale.x < camera_.x ||
                        transform.globalPosition.x > camera_.x + camera_.w ||
                        transform.globalPosition.y + sprite.height * transform.globalScale.y < camera_.y ||
                        transform.globalPosition.y > camera_.y + camera_.h;
    }

    if (!isOutsideCamera) {
      // Re-resolve when first seen or when AssetManager has loaded/replaced any texture
      // since this sprite cached its pointer. Stale cache would draw a destroyed handle.
      const auto assetGen = assetManager_->TextureGeneration();
      if (!sprite.cachedTexture || sprite.cachedTextureGeneration != assetGen) {
        sprite.cachedTexture = assetManager_->GetTexture(sprite.assetId);
        sprite.cachedTextureGeneration = assetGen;
      }

      RenderKey renderKey(sprite.layer, transform.globalPosition.y, SPRITE, entity);
      // Pre-compute render data so Renderer doesn't need GetComponent lookups.
      const float x = sprite.isFixed ? transform.globalPosition.x : transform.globalPosition.x - camera_.x;
      const float y = sprite.isFixed ? transform.globalPosition.y : transform.globalPosition.y - camera_.y;
      renderKey.destX = x;
      renderKey.destY = y;
      renderKey.destW = sprite.width * transform.globalScale.x;
      renderKey.destH = sprite.height * transform.globalScale.y;
      renderKey.srcRect = sprite.srcRect;
      renderKey.rotation = transform.rotation;
      renderKey.flip = sprite.flip;
      renderKey.texture = sprite.cachedTexture;
      renderKey.isFixed = sprite.isFixed;

      cmd_buffer_.QueueAddRenderKey(renderKey);
    }
  }

 private:
  CommandBuffer cmd_buffer_;
  AssetManager* assetManager_ = nullptr;
  float windowWidth_ = 0;
  float windowHeight_ = 0;
  SDL_FRect camera_{};
};