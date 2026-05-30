#pragma once

#include <SDL3/SDL.h>

#include <cstdint>
#include <string>
#include <utility>

struct SpriteComponent {
  std::string assetId;
  float width;
  float height;
  int layer;
  bool isFixed;
  SDL_FRect srcRect{};
  SDL_FlipMode flip;
  mutable SDL_Texture* cachedTexture = nullptr;
  mutable std::uint64_t cachedTextureGeneration = 0;
  // Cached origin of this asset inside its atlas, or {0,0,0,0} when not atlas-backed. Re-resolved
  // alongside cachedTexture (same generation gate) so a hot animation tick reads a single FRect
  // instead of walking the catalog every frame.
  mutable SDL_FRect cachedAtlasOffset{};

  explicit SpriteComponent(std::string t_assetId = "", const float t_width = 0, const float t_height = 0,
                           const int t_layer = 0, const bool t_isFixed = false, const float t_srcRectX = 0,
                           const float t_srcRectY = 0)
      : assetId(std::move(t_assetId)),
        width(t_width),
        height(t_height),
        layer(t_layer),
        isFixed(t_isFixed),
        flip(SDL_FLIP_NONE) {
    this->srcRect = {t_srcRectX, t_srcRectY, t_width, t_height};
  }
};