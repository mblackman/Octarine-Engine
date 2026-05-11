#pragma once

#include <SDL3/SDL.h>

#include "../AssetManager/AssetManager.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../Renderer/RenderQueue.h"
#include "ECS/Registry.h"
#include "Game/GameConfig.h"

class RenderSpriteSystem {
 public:
  void Prepare(Registry* registry) {
    const auto& gameConfig = registry->Get<GameConfig>();
    camera_ = registry->Get<CameraComponent>().viewport;
    assetManager_ = &registry->Get<AssetManager>();
    renderQueue_ = &registry->Get<RenderQueue>();
    windowWidth_ = static_cast<float>(gameConfig.windowWidth);
    windowHeight_ = static_cast<float>(gameConfig.windowHeight);
  }

  void operator()(const TransformComponent& transform, const SpriteComponent& sprite) const {
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

    if (isOutsideCamera) return;

    // Re-resolve when first seen or when AssetManager has loaded/replaced any texture
    // since this sprite cached its pointer. Stale cache would draw a destroyed handle.
    const auto assetGen = assetManager_->TextureGeneration();
    if (!sprite.cachedTexture || sprite.cachedTextureGeneration != assetGen) {
      sprite.cachedTexture = assetManager_->GetTexture(sprite.assetId);
      sprite.cachedTextureGeneration = assetGen;
    }

    const float x = sprite.isFixed ? transform.globalPosition.x : transform.globalPosition.x - camera_.x;
    const float y = sprite.isFixed ? transform.globalPosition.y : transform.globalPosition.y - camera_.y;

    auto& cmd = renderQueue_->EmplaceSprite(static_cast<unsigned int>(sprite.layer), transform.globalPosition.y,
                                            sprite.cachedTexture);
    cmd.destX = x;
    cmd.destY = y;
    cmd.destW = sprite.width * transform.globalScale.x;
    cmd.destH = sprite.height * transform.globalScale.y;
    cmd.srcRect = sprite.srcRect;
    cmd.rotation = transform.globalRotation;
    cmd.pivot = {cmd.destW * 0.5f, cmd.destH * 0.5f};
    cmd.flip = sprite.flip;
    cmd.texture = sprite.cachedTexture;
  }

 private:
  AssetManager* assetManager_ = nullptr;
  RenderQueue* renderQueue_ = nullptr;
  float windowWidth_ = 0;
  float windowHeight_ = 0;
  SDL_FRect camera_{};
};
