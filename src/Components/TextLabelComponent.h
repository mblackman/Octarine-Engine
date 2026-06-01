#pragma once

#include <glm/glm.hpp>
#include <string>
#include <utility>

#include "General/Color.h"
#include "General/Constants.h"

struct TextLabelComponent {
  glm::vec2 position;
  int layer;
  std::string text;
  std::string fontId;
  octarine::Color color;
  bool isFixed;

  explicit TextLabelComponent(const glm::vec2 t_position = glm::vec2(0, 0), const int t_layer = 0,
                              std::string t_text = "", std::string t_fontId = "",
                              const octarine::Color t_color = {Constants::kUint8Max, Constants::kUint8Max,
                                                               Constants::kUint8Max, Constants::kUint8Max},
                              const bool t_isFixed = true)
      : position(t_position),
        layer(t_layer),
        text(std::move(t_text)),
        fontId(std::move(t_fontId)),
        color(t_color),
        isFixed(t_isFixed) {}
};
