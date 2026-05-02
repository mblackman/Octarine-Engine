#pragma once

#include <SDL3/SDL.h>

#include <string>
#include <utility>

struct SpriteComponent {
  std::string assetId;
  int width;
  int height;
  int layer;
  bool isFixed;
  SDL_FRect srcRect{};
  SDL_FlipMode flip;

  explicit SpriteComponent(std::string assetId = "", const float width = 0, const float height = 0, const int layer = 0,
                           const bool isFixed = false, const float srcRectX = 0, const float srcRectY = 0)
      : assetId(std::move(std::move(assetId))),
        width(width),
        height(height),
        layer(layer),
        isFixed(isFixed),
        flip(SDL_FLIP_NONE) {
    this->srcRect = {srcRectX, srcRectY, width, height};
  }
};