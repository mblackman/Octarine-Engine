#pragma once

#include <SDL3/SDL.h>

#include <memory>
#include <string>
#include <unordered_map>

#include "../AssetManager/AssetManager.h"
#include "./RenderQueue.h"

class Registry;

class Renderer {
 public:
  Renderer() = default;

  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;

  Renderer(Renderer&&) = delete;
  Renderer& operator=(Renderer&&) = delete;

  ~Renderer();

  void Render(const Registry* registry, SDL_Renderer* renderer) const;

 private:
  struct TextCacheEntry {
    SDL_Texture* texture;
    float width;
    float height;
  };

  static void RenderSprite(const RenderKey& key, SDL_Renderer* renderer);
  static void RenderSquare(const Entity& entity, const Registry* registry, SDL_Renderer* renderer,
                           const SDL_FRect& camera);
  void RenderText(const Entity& entity, const Registry* registry, SDL_Renderer* renderer,
                  const AssetManager& assetManager, const SDL_FRect& camera) const;

  // (fontId|text|packedColor) → cached SDL_Texture. Avoids re-rasterizing every frame.
  // Invalidated wholesale on Renderer destruction; no per-entry eviction yet.
  mutable std::unordered_map<std::string, TextCacheEntry> text_cache_;
};