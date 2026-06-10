#pragma once

#include <SDL3/SDL.h>

#include <atomic>
#include <memory>
#include <optional>

#include "AssetManager/AssetManager.h"
#include "Components/GlobalTransformComponent.h"
#include "Components/SpriteComponent.h"
#include "ECS/Query.h"
#include "ECS/Registry.h"
#include "Game/GameConfig.h"
#include "General/PerfUtils.h"
#include "General/Rect.h"
#include "General/SpriteFlip.h"
#include "Renderer/RenderCulling.h"
#include "Renderer/RenderQueue.h"
#include "Renderer/SpriteRenderCache.h"

class RenderSpriteSystem {
 public:
  void Prepare(Registry* registry) {
    const auto& gameConfig = registry->Get<GameConfig>();
    camera_ = registry->Get<CameraComponent>().viewport;
    assetManager_ = &registry->Get<AssetManager>();
    renderQueue_ = &registry->Get<RenderQueue>();
    spriteCache_ = &registry->Get<SpriteRenderCache>();
    windowWidth_ = static_cast<float>(gameConfig.windowWidth);
    windowHeight_ = static_cast<float>(gameConfig.windowHeight);

    // Single-threaded cache warm: walk every sprite entity once and resolve the SDL_Texture* +
    // atlas slice now. operator() (which runs in parallel via ParallelForEach) then does a
    // race-free read-only Lookup. SpriteRenderCache is an std::unordered_map and cannot tolerate
    // concurrent inserts.
    if (!warmingQuery_) {
      warmingQuery_ = registry->CreateQuery<SpriteComponent>();
    }
    warmingQuery_->Update();
    const auto assetGen = assetManager_->TextureGeneration();
    warmingQuery_->ForEach([this, assetGen](Entity entity, const SpriteComponent& sprite) {
      if (spriteCache_->Lookup(entity, assetGen) == nullptr) {
        SDL_Texture* texture = assetManager_->GetTexture(sprite.assetId);
        const auto slice = assetManager_->GetAtlasSlice(sprite.assetId);
        const SDL_FRect atlasOffset = slice.has_value() ? *slice : SDL_FRect{0, 0, 0, 0};
        spriteCache_->Store(entity, texture, atlasOffset, assetGen);
      }
    });

#ifdef OCTARINE_PROFILING
    if (!culledCounter_) culledCounter_ = PROFILE_COUNTER_HANDLE("RenderSprite: Culled");
    if (!emplacedCounter_) emplacedCounter_ = PROFILE_COUNTER_HANDLE("RenderSprite: Emplaced");
#endif
  }

  void operator()(Entity entity, const GlobalTransformComponent& transform, const SpriteComponent& sprite) const {
    const bool isOutsideCamera = IsRenderableOutsideViewport(
        transform.position.x, transform.position.y, sprite.width * transform.scale.x, sprite.height * transform.scale.y,
        sprite.isFixed, camera_, windowWidth_, windowHeight_);

    if (isOutsideCamera) {
      PROFILE_COUNTER_INC(culledCounter_);
      return;
    }

    PROFILE_COUNTER_INC(emplacedCounter_);

    const SpriteRenderCache::Entry* entry = spriteCache_->Lookup(entity, assetManager_->TextureGeneration());
    SDL_Texture* texture = entry ? entry->texture : nullptr;
    const SDL_FRect atlasOffset = entry ? entry->atlasOffset : SDL_FRect{0, 0, 0, 0};

    const float x = sprite.isFixed ? transform.position.x : transform.position.x - camera_.x;
    const float y = sprite.isFixed ? transform.position.y : transform.position.y - camera_.y;

    auto& cmd = renderQueue_->EmplaceSprite(static_cast<unsigned int>(sprite.layer), transform.position.y, texture,
                                            sprite.blendMode);
    cmd.destX = x;
    cmd.destY = y;
    cmd.destW = sprite.width * transform.scale.x;
    cmd.destH = sprite.height * transform.scale.y;
    // Atlas-aware src_rect compose: a loose texture leaves atlasOffset at zero (no-op add); an
    // atlas member adds the slice origin so the sprite's logical (sx, sy) selects inside the
    // packed atlas instead of the standalone source PNG.
    cmd.srcRect.x = sprite.srcRect.x + atlasOffset.x;
    cmd.srcRect.y = sprite.srcRect.y + atlasOffset.y;
    cmd.srcRect.w = sprite.srcRect.w;
    cmd.srcRect.h = sprite.srcRect.h;
    cmd.rotation = transform.rotation;
    cmd.pivot = {cmd.destW * 0.5f, cmd.destH * 0.5f};
    cmd.flip = static_cast<SDL_FlipMode>(sprite.flip);
    cmd.texture = texture;
    cmd.colorMod = SDL_Color{sprite.colorMod.r, sprite.colorMod.g, sprite.colorMod.b, sprite.colorMod.a};
    cmd.blendMode = octarine::ToSdlBlendMode(sprite.blendMode);
  }

 private:
  AssetManager* assetManager_ = nullptr;
  RenderQueue* renderQueue_ = nullptr;
  SpriteRenderCache* spriteCache_ = nullptr;
  std::unique_ptr<ComponentQuery<SpriteComponent>> warmingQuery_;
  float windowWidth_ = 0;
  float windowHeight_ = 0;
  octarine::Rect camera_{};
#ifdef OCTARINE_PROFILING
  std::atomic<long long>* culledCounter_ = nullptr;
  std::atomic<long long>* emplacedCounter_ = nullptr;
#endif
};
