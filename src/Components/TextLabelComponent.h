#pragma once

#include <SDL3/SDL.h>

#include <glm/glm.hpp>
#include <string>
#include <utility>
#include <utility>

struct TextLabelComponent {
  glm::vec2 position;
  int layer;
  std::string text;
  std::string fontId;
  SDL_Color color;
  bool isFixed;

  explicit TextLabelComponent(glm::vec2 position = glm::vec2(0, 0), int layer = 0,
                     std::string text = "", std::string fontId = "",
                     SDL_Color color = {255, 255, 255, 255},
                     bool isFixed = true)
      : position(position),
        layer(layer),
        text(std::move(std::move(text))),
        fontId(std::move(std::move(fontId))),
        color(color),
        isFixed(isFixed) {}
};