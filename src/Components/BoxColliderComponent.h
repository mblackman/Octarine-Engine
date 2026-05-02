#pragma once

#include <glm/glm.hpp>

struct BoxColliderComponent {
  int width;
  int height;
  glm::vec2 offset{};

  explicit BoxColliderComponent(const int width = 0, const int height = 0, const glm::vec2 offset = glm::vec2(0))
      : width(width), height(height), offset(offset) {}
};
