#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <string>
#include <utility>

#include "General/Color.h"
#include "General/Constants.h"

// Alignment of the rasterized text within its layout rectangle (UIRectComponent). Only consulted on
// the UI path; world-space labels (positioned by transform) have no box to align within. Start is
// the historical behavior: the texture's top-left sits at the rect's top-left.
enum class TextAlign : std::uint8_t { Start, Center, End };

struct TextLabelComponent {
  glm::vec2 position;
  int layer;
  std::string text;
  std::string fontId;
  octarine::Color color;
  bool isFixed;
  TextAlign hAlign = TextAlign::Start;
  TextAlign vAlign = TextAlign::Start;

  explicit TextLabelComponent(const glm::vec2 t_position = glm::vec2(0, 0), const int t_layer = 0,
                              std::string t_text = "", std::string t_fontId = "",
                              const octarine::Color t_color = {Constants::kUint8Max, Constants::kUint8Max,
                                                               Constants::kUint8Max, Constants::kUint8Max},
                              const bool t_isFixed = true, const TextAlign t_hAlign = TextAlign::Start,
                              const TextAlign t_vAlign = TextAlign::Start)
      : position(t_position),
        layer(t_layer),
        text(std::move(t_text)),
        fontId(std::move(t_fontId)),
        color(t_color),
        isFixed(t_isFixed),
        hAlign(t_hAlign),
        vAlign(t_vAlign) {}
};
