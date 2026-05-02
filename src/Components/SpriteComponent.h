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

  explicit SpriteComponent(std::string assetId = "", float width = 0,
                           float height = 0,
                           int layer = 0, bool isFixed = false,
                           float srcRectX = 0,
                           float srcRectY = 0)
    : assetId(std::move(std::move(assetId))),
      width(width),
      height(height),
      layer(layer),
      isFixed(isFixed),
      flip(SDL_FLIP_NONE) {
    this->srcRect = {srcRectX, srcRectY, width, height};
  }
};