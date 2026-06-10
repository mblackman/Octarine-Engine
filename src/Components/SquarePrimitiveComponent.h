#pragma once

#include <glm/glm.hpp>

#include "General/BlendMode.h"
#include "General/Color.h"
#include "General/Constants.h"

struct SquarePrimitiveComponent {
  glm::vec2 position;
  int layer;
  float width;
  float height;
  octarine::Color color;  // a < 255 fades the square (with the default Blend mode)
  bool isFixed;
  octarine::BlendMode blendMode = octarine::BlendMode::Blend;

  explicit SquarePrimitiveComponent(const glm::vec2 t_position = glm::vec2(0, 0), const int t_layer = 0,
                                    const float t_width = 0, const float t_height = 0,
                                    const octarine::Color t_color = {Constants::kUint8Max, Constants::kUint8Max,
                                                                     Constants::kUint8Max, Constants::kUint8Max},
                                    const bool t_isFixed = true)
      : position(t_position), layer(t_layer), width(t_width), height(t_height), color(t_color), isFixed(t_isFixed) {}
};
