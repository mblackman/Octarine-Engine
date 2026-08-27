#pragma once

#include <glm/glm.hpp>

struct PivotComponent {
  static constexpr float kDefaultX = 0.5F;
  static constexpr float kDefaultY = 0.5F;

  glm::vec2 value;

  explicit PivotComponent(const glm::vec2 t_value = glm::vec2(kDefaultX, kDefaultY)) : value(t_value) {}
  PivotComponent(const float x, const float y) : PivotComponent(glm::vec2(x, y)) {}

  [[nodiscard]] float get_x() const { return value.x; }
  void set_x(const float t_x) { value.x = t_x; }
  [[nodiscard]] float get_y() const { return value.y; }
  void set_y(const float t_y) { value.y = t_y; }
};
