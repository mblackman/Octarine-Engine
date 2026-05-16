#pragma once

#include <SDL3/SDL.h>

#include <atomic>

#include "../AssetManager/AssetManager.h"
#include "../Components/GlobalTransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../General/PerfUtils.h"
#include "../Renderer/RenderCulling.h"
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
#ifdef OCTARINE_PROFILING
    if (!culledCounter_) culledCounter_ = PROFILE_COUNTER_HANDLE("RenderSprite: Culled");
    if (!emplacedCounter_) emplacedCounter_ = PROFILE_COUNTER_HANDLE("RenderSprite: Emplaced");
#endif
  }

  void operator()(const GlobalTransformComponent& transform, const SpriteComponent& sprite) const {
    const bool isOutsideCamera = IsRenderableOutsideViewport(
        transform.position.x, transform.position.y, sprite.width * transform.scale.x,
        sprite.height * transform.scale.y, sprite.isFixed, camera_, windowWidth_, windowHeight_);

    if (isOutsideCamera) {
      PROFILE_COUNTER_INC(culledCounter_);
      return;
    }

    PROFILE_COUNTER_INC(emplacedCounter_);

    // Re-resolve when first seen or when AssetManager has loaded/replaced any texture
    // since this sprite cached its pointer. Stale cache would draw a destroyed handle.
    const auto assetGen = assetManager_->TextureGeneration();
    if (!sprite.cachedTexture || sprite.cachedTextureGeneration != assetGen) {
      sprite.cachedTexture = assetManager_->GetTexture(sprite.assetId);
      sprite.cachedTextureGeneration = assetGen;
    }

    const float x = sprite.isFixed ? transform.position.x : transform.position.x - camera_.x;
    const float y = sprite.isFixed ? transform.position.y : transform.position.y - camera_.y;

    auto& cmd = renderQueue_->EmplaceSprite(static_cast<unsigned int>(sprite.layer), transform.position.y,
                                            sprite.cachedTexture);
    cmd.destX = x;
    cmd.destY = y;
    cmd.destW = sprite.width * transform.scale.x;
    cmd.destH = sprite.height * transform.scale.y;
    cmd.srcRect = sprite.srcRect;
    cmd.rotation = transform.rotation;
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
#ifdef OCTARINE_PROFILING
  // Cached counter handles — pointer is stable across frames because PerfCounters never
  // erases slots; ResetValues() only zeroes the atomic. operator() is called per-entity in
  // parallel chunks, so the only hot-path cost is an atomic fetch_add.
  std::atomic<long long>* culledCounter_ = nullptr;
  std::atomic<long long>* emplacedCounter_ = nullptr;
#endif
};
