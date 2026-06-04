#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdint>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

#include "AssetManager/AssetManager.h"
#include "AssetManager/GlyphAtlas.h"
#include "Components/CameraComponents.h"
#include "Components/GlobalTransformComponent.h"
#include "Components/TextLabelComponent.h"
#include "ECS/Entity.h"
#include "ECS/Iterable.h"
#include "ECS/Registry.h"
#include "Engine/EngineContext.h"
#include "Game/GameConfig.h"
#include "General/Logger.h"
#include "General/PerfUtils.h"
#include "General/Rect.h"
#include "Renderer/RenderCommands.h"
#include "Renderer/RenderCulling.h"
#include "Renderer/RenderQueue.h"

class RenderTextSystem {
 public:
  RenderTextSystem() = default;

  RenderTextSystem(const RenderTextSystem&) = delete;
  RenderTextSystem& operator=(const RenderTextSystem&) = delete;
  RenderTextSystem(RenderTextSystem&&) = default;
  RenderTextSystem& operator=(RenderTextSystem&&) = default;

  ~RenderTextSystem() {
    for (auto& [key, entry] : text_cache_) {
      if (entry.texture) {
        SDL_DestroyTexture(entry.texture);
      }
    }
  }

  void operator()(const ContextFacade& ctx, const TextLabelComponent& text) const {
    if (text.text.empty()) return;

    Registry* registry = ctx.GetRegistry();
    const auto& assetManager = registry->Get<AssetManager>();
    auto* sdlRenderer = registry->Get<EngineContext>().sdlRenderer;
    auto& renderQueue = registry->Get<RenderQueue>();
    const octarine::Rect camera = registry->Get<CameraComponent>().viewport;

    TTF_Font* font = assetManager.GetFont(text.fontId);
    if (!font) return;

    const auto packedColor = static_cast<Uint32>(text.color.r) << 24 | static_cast<Uint32>(text.color.g) << 16 |
                             static_cast<Uint32>(text.color.b) << 8 | static_cast<Uint32>(text.color.a);

    // Entity-keyed cache (mirrors Renderer/SpriteRenderCache): one rasterized texture per text
    // entity, re-rasterized only when its (fontId, text, color) changes. Steady-state lookups do
    // no heap allocation, and a label that updates every frame (score/timer) frees its old texture
    // instead of leaking a fresh one per frame.
    const Entity entity = ctx.GetEntity();
    auto it = text_cache_.find(entity.GetId());
    const bool needsRaster = it == text_cache_.end() || it->second.color != packedColor ||
                             it->second.fontId != text.fontId || it->second.text != text.text;
    if (needsRaster) {
      float w = 0;
      float h = 0;
      SDL_Texture* texture = RasterizeLabel(assetManager, sdlRenderer, font, text, w, h);
      if (texture == nullptr) return;
      it = StoreEntry(it, entity, text, packedColor, texture, w, h);
    }

    glm::vec2 origin = text.position;
    if (registry->HasComponent<GlobalTransformComponent>(entity)) {
      const auto& transform = registry->GetComponent<GlobalTransformComponent>(entity);
      origin += transform.position;
    }

    const auto& gameConfig = registry->Get<GameConfig>();
    const bool isOutsideCamera = IsRenderableOutsideViewport(
        origin.x, origin.y, it->second.width, it->second.height, text.isFixed, camera,
        static_cast<float>(gameConfig.windowWidth), static_cast<float>(gameConfig.windowHeight));

#ifdef OCTARINE_PROFILING
    static auto* culledCounter = PROFILE_COUNTER_HANDLE("RenderText: Culled");
    static auto* emplacedCounter = PROFILE_COUNTER_HANDLE("RenderText: Emplaced");
#endif

    if (isOutsideCamera) {
      PROFILE_COUNTER_INC(culledCounter);
      return;
    }
    PROFILE_COUNTER_INC(emplacedCounter);

    const float x = text.isFixed ? origin.x : origin.x - camera.x;
    const float y = text.isFixed ? origin.y : origin.y - camera.y;

    auto& cmd = renderQueue.EmplaceText(static_cast<unsigned int>(text.layer), text.position.y, it->second.texture);
    cmd.destRect = {x, y, it->second.width, it->second.height};
    cmd.texture = it->second.texture;
  }

 private:
  struct TextCacheEntry {
    std::string fontId;
    std::string text;
    Uint32 color = 0;
    SDL_Texture* texture = nullptr;
    float width = 0.0F;
    float height = 0.0F;
  };

  // Entity → last-rasterized label. Re-rasterized only when (fontId, text, color) changes, so a
  // label that updates every frame holds a single texture rather than leaking one per frame. Two
  // entities with identical text no longer share a texture (entity-keyed, not content-keyed) —
  // fine for typical UI label counts. Entries for despawned text entities persist until the system
  // is destroyed (its destructor frees every cached texture).
  mutable std::unordered_map<EcsId, TextCacheEntry> text_cache_;

  // Store the freshly rasterized texture as this entity's cache entry, freeing any texture it was
  // previously holding, and return the iterator to the live entry. `it` is this entity's current
  // slot (text_cache_.end() if it had none).
  std::unordered_map<EcsId, TextCacheEntry>::iterator StoreEntry(std::unordered_map<EcsId, TextCacheEntry>::iterator it,
                                                                 Entity entity, const TextLabelComponent& text,
                                                                 Uint32 color, SDL_Texture* texture, float width,
                                                                 float height) const {
    if (it != text_cache_.end()) {
      if (it->second.texture != nullptr) SDL_DestroyTexture(it->second.texture);
      it->second = {text.fontId, text.text, color, texture, width, height};
      return it;
    }
    return text_cache_.emplace(entity.GetId(), TextCacheEntry{text.fontId, text.text, color, texture, width, height})
        .first;
  }

  // Rasterize `text` to a freshly created SDL_Texture, writing its size into outW/outH. text.color
  // is octarine::Color (Stage 2 POD-no-SDL); convert to SDL_Color once here at the render seam. The
  // atlas fast path composes the surface by blitting resident glyphs from the font's GlyphAtlas and
  // falls back to TTF_RenderText_Blended when no atlas is baked or the string hits an uncovered
  // glyph (extended Latin, emoji, etc.). Returns nullptr (and logs) on failure; the caller owns the
  // returned texture.
  static SDL_Texture* RasterizeLabel(const AssetManager& assetManager, SDL_Renderer* sdlRenderer, TTF_Font* font,
                                     const TextLabelComponent& text, float& outW, float& outH) {
    const SDL_Color sdlColor{text.color.r, text.color.g, text.color.b, text.color.a};
    SDL_Surface* surface = nullptr;
    const GlyphAtlas* atlas = assetManager.GetGlyphAtlas(text.fontId);
    if (atlas != nullptr && atlas->IsLoaded()) {
      surface = ComposeFromAtlas(*atlas, text.text, sdlColor);
    }
    if (surface == nullptr) {
      surface = TTF_RenderText_Blended(font, text.text.c_str(), 0, sdlColor);
    }
    if (surface == nullptr) {
      Logger::Error("RenderText: surface compose failed: " + std::string(SDL_GetError()));
      return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(sdlRenderer, surface);
    SDL_DestroySurface(surface);
    if (texture == nullptr) {
      Logger::Error("SDL_CreateTextureFromSurface failed: " + std::string(SDL_GetError()));
      return nullptr;
    }
    outW = 0;
    outH = 0;
    SDL_GetTextureSize(texture, &outW, &outH);
    return texture;
  }

  // Atlas compose: build an RGBA destination surface, blit each codepoint's rect from the atlas
  // into it, color-mod the source surface to tint the white-rasterized atlas glyphs to the
  // requested color. Returns nullptr on any missing codepoint (caller falls back to TTF). Layout
  // is top-aligned — not pixel-perfect for descenders, but readable for ASCII at typical UI sizes;
  // ascent-aware baseline alignment is a follow-up.
  static SDL_Surface* ComposeFromAtlas(const GlyphAtlas& atlas, const std::string& text, SDL_Color color) {
    float totalW = 0.0F;
    for (const char c : text) {
      const auto cp = static_cast<std::uint32_t>(static_cast<unsigned char>(c));
      const auto* g = atlas.Find(cp);
      if (g == nullptr) return nullptr;
      totalW += g->advance;
    }
    const int destW = std::max(1, static_cast<int>(std::ceil(totalW)));
    const int destH = std::max(1, atlas.LineSkip());
    SDL_Surface* dst = SDL_CreateSurface(destW, destH, SDL_PIXELFORMAT_RGBA32);
    if (dst == nullptr) return nullptr;

    SDL_Surface* src = atlas.SourceSurface();
    SDL_SetSurfaceColorMod(src, color.r, color.g, color.b);
    SDL_SetSurfaceAlphaMod(src, color.a);
    SDL_SetSurfaceBlendMode(src, SDL_BLENDMODE_BLEND);

    float pen = 0.0F;
    for (const char c : text) {
      const auto cp = static_cast<std::uint32_t>(static_cast<unsigned char>(c));
      const auto* g = atlas.Find(cp);
      SDL_Rect srcR{static_cast<int>(g->x), static_cast<int>(g->y), static_cast<int>(g->w), static_cast<int>(g->h)};
      SDL_Rect dstR{static_cast<int>(pen), 0, static_cast<int>(g->w), static_cast<int>(g->h)};
      SDL_BlitSurface(src, &srcR, dst, &dstR);
      pen += g->advance;
    }

    // Reset mods so the next compose (or a parallel one for a differently-colored label) starts
    // from a clean state on the shared atlas surface.
    SDL_SetSurfaceColorMod(src, 255, 255, 255);
    SDL_SetSurfaceAlphaMod(src, 255);
    return dst;
  }
};
