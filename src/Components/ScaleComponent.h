#pragma once

#include <glm/glm.hpp>

struct ScaleComponent {
  glm::vec2 value;

  explicit ScaleComponent(const glm::vec2 t_value = glm::vec2(1.0f, 1.0f)) : value(t_value) {}
};
