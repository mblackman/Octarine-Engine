#pragma once

#include <SDL3/SDL.h>

#include <glm/glm.hpp>

struct SquarePrimitiveComponent {
  glm::vec2 position;
  int layer;
  float width;
  float height;
  SDL_Color color;
  bool isFixed;

  explicit SquarePrimitiveComponent(glm::vec2 position = glm::vec2(0, 0), int layer = 0,
                                    float width = 0, float height = 0,
                                    SDL_Color color = {255, 255, 255, 255},
                           bool isFixed = true)
      : position(position),
        layer(layer),
        width(width),
        height(height),
        color(color),
        isFixed(isFixed) {}
};