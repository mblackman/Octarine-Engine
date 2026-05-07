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

  explicit SquarePrimitiveComponent(const glm::vec2 t_position = glm::vec2(0, 0), const int t_layer = 0,
                                    const float t_width = 0, const float t_height = 0,
                                    const SDL_Color t_color = {Constants::kUint8Max, Constants::kUint8Max,
                                                               Constants::kUint8Max, Constants::kUint8Max},
                                    const bool t_isFixed = true)
      : position(t_position), layer(t_layer), width(t_width), height(t_height), color(t_color), isFixed(t_isFixed) {}
};