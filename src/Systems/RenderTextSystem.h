#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

#include <atomic>

#include "../AssetManager/AssetManager.h"
#include "../Components/GlobalTransformComponent.h"
#include "../Components/TextLabelComponent.h"
#include "../General/Logger.h"
#include "../General/PerfUtils.h"
#include "../Renderer/RenderCommands.h"
#include "../Renderer/RenderCulling.h"
#include "../Renderer/RenderQueue.h"
#include "Components/CameraComponents.h"
#include "ECS/Iterable.h"
#include "ECS/Registry.h"
#include "Game/GameConfig.h"

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
    auto* sdlRenderer = registry->Get<SDL_Renderer*>();
    auto& renderQueue = registry->Get<RenderQueue>();
    const SDL_FRect camera = registry->Get<CameraComponent>().viewport;

    TTF_Font* font = assetManager.GetFont(text.fontId);
    if (!font) return;

    const auto packedColor = static_cast<Uint32>(text.color.r) << 24 | static_cast<Uint32>(text.color.g) << 16 |
                             static_cast<Uint32>(text.color.b) << 8 | static_cast<Uint32>(text.color.a);
    const std::string cacheKey = text.fontId + "|" + text.text + "|" + std::to_string(packedColor);

    auto it = text_cache_.find(cacheKey);
    if (it == text_cache_.end()) {
      SDL_Surface* surface = TTF_RenderText_Blended(font, text.text.c_str(), 0, text.color);
      if (!surface) {
        Logger::Error("TTF_RenderText_Blended failed: " + std::string(SDL_GetError()));
        return;
      }
      SDL_Texture* texture = SDL_CreateTextureFromSurface(sdlRenderer, surface);
      SDL_DestroySurface(surface);
      if (!texture) {
        Logger::Error("SDL_CreateTextureFromSurface failed: " + std::string(SDL_GetError()));
        return;
      }
      float w = 0;
      float h = 0;
      SDL_GetTextureSize(texture, &w, &h);
      it = text_cache_.emplace(cacheKey, TextCacheEntry{texture, w, h}).first;
    }

    const Entity entity = ctx.GetEntity();
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
    SDL_Texture* texture;
    float width;
    float height;
  };

  // (fontId|text|packedColor) → cached SDL_Texture. Avoids re-rasterizing every frame.
  // Invalidated wholesale on system destruction; no per-entry eviction yet.
  mutable std::unordered_map<std::string, TextCacheEntry> text_cache_;
};
