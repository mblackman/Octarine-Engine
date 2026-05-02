#pragma once

#include <SDL3/SDL.h>

#include <glm/glm.hpp>

#include "General/Constants.h"

struct SquarePrimitiveComponent {
  glm::vec2 position;
  int layer;
  float width;
  float height;
  SDL_Color color;
  bool isFixed;

  explicit SquarePrimitiveComponent(const glm::vec2 position = glm::vec2(0, 0), const int layer = 0,
                                    const float width = 0, const float height = 0,
                                    const SDL_Color color = {Constants::kUnt8Max, Constants::kUnt8Max,
                                                             Constants::kUnt8Max, Constants::kUnt8Max},
                                    const bool isFixed = true)
      : position(position), layer(layer), width(width), height(height), color(color), isFixed(isFixed) {}
};