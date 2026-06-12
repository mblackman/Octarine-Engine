#pragma once

#include <SDL3/SDL.h>

#include <memory>
#include <optional>

#include "AssetManager/AssetManager.h"
#include "Components/SpriteComponent.h"
#include "Components/UIRectComponent.h"
#include "ECS/Query.h"
#include "ECS/Registry.h"
#include "General/PerfUtils.h"
#include "General/SpriteFlip.h"
#include "Renderer/RenderQueue.h"
#include "Renderer/SpriteRenderCache.h"

// Renders SpriteComponent entities whose position is driven by UIRectComponent (layout-driven UI
// sprites). Runs as a serial RegisterSystem after UILayoutSystem. World sprites without
// UIRectComponent continue through the parallel RenderSpriteSystem path unchanged.
class RenderUISpriteSystem {
 public:
  void Prepare(Registry* registry) {
    assetManager_ = &registry->Get<AssetManager>();
    renderQueue_ = &registry->Get<RenderQueue>();
    spriteCache_ = &registry->Get<SpriteRenderCache>();
  }

  void operator()(Entity entity, const UIRectComponent& rect, const SpriteComponent& sprite) const {
    const SpriteRenderCache::Entry* entry = spriteCache_->Lookup(entity, assetManager_->TextureGeneration());
    if (!entry || !entry->texture) {
      // Warm the cache entry on demand (should rarely happen; the world sprite warming path
      // covers most entities, but pure-UI sprites without GlobalTransformComponent are missed).
      SDL_Texture* texture = assetManager_->GetTexture(sprite.assetId);
      const auto slice = assetManager_->GetAtlasSlice(sprite.assetId);
      const SDL_FRect atlasOffset = slice.has_value() ? *slice : SDL_FRect{0, 0, 0, 0};
      spriteCache_->Store(entity, texture, atlasOffset, assetManager_->TextureGeneration());
      entry = spriteCache_->Lookup(entity, assetManager_->TextureGeneration());
      if (!entry) return;
    }

    SDL_Texture* texture = entry->texture;
    const SDL_FRect atlasOffset = entry->atlasOffset;

    const float destW = rect.Width();
    const float destH = rect.Height();

    auto& cmd = renderQueue_->EmplaceSprite(static_cast<unsigned int>(rect.layer), rect.top, texture, sprite.blendMode);
    cmd.destX = rect.left;
    cmd.destY = rect.top;
    cmd.destW = destW;
    cmd.destH = destH;
    cmd.srcRect.x = sprite.srcRect.x + atlasOffset.x;
    cmd.srcRect.y = sprite.srcRect.y + atlasOffset.y;
    cmd.srcRect.w = sprite.srcRect.w;
    cmd.srcRect.h = sprite.srcRect.h;
    static constexpr float kHalf = 0.5f;
    cmd.rotation = 0.0;
    cmd.pivot = {destW * kHalf, destH * kHalf};
    cmd.flip = static_cast<SDL_FlipMode>(sprite.flip);
    cmd.texture = texture;
    cmd.colorMod = SDL_Color{sprite.colorMod.r, sprite.colorMod.g, sprite.colorMod.b, sprite.colorMod.a};
    cmd.blendMode = octarine::ToSdlBlendMode(sprite.blendMode);
  }

 private:
  AssetManager* assetManager_ = nullptr;
  RenderQueue* renderQueue_ = nullptr;
  SpriteRenderCache* spriteCache_ = nullptr;
};
