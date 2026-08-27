#pragma once

#include <glm/glm.hpp>

struct PositionComponent {
  glm::vec2 value;

  explicit PositionComponent(const glm::vec2 t_value = glm::vec2(0.0f, 0.0f)) : value(t_value) {}

  [[nodiscard]] float get_x() const { return value.x; }
  void set_x(const float t_x) { value.x = t_x; }
  [[nodiscard]] float get_y() const { return value.y; }
  void set_y(const float t_y) { value.y = t_y; }
};
