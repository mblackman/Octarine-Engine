#pragma once

#include <glm/glm.hpp>

struct PositionComponent {
  glm::vec2 value;

  explicit PositionComponent(const glm::vec2 t_value = glm::vec2(0.0f, 0.0f)) : value(t_value) {}
};
