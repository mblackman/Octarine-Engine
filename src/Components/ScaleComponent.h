#pragma once

#include <glm/glm.hpp>

struct ScaleComponent {
  glm::vec2 value;

  explicit ScaleComponent(const float x, const float y) : ScaleComponent(glm::vec2(x, y)) {}
  explicit ScaleComponent(const glm::vec2 t_value = glm::vec2(1.0f, 1.0f)) : value(t_value) {}

  [[nodiscard]] float get_x() const { return value.x; }
  void set_x(const float t_x) { value.x = t_x; }
  [[nodiscard]] float get_y() const { return value.y; }
  void set_y(const float t_y) { value.y = t_y; }
  // True when both axes agree, so `scale` is a faithful summary of the pair.
  [[nodiscard]] bool is_uniform() const { return value.x == value.y; }
  // The uniform-scale view: reads x, writes both. Meaningful only when is_uniform() — on a
  // non-uniform scale it reports the x axis rather than failing, because this is a per-frame
  // accessor reachable from Lua and a script must not be able to halt the engine. Callers that
  // care about the difference should ask is_uniform() first.
  [[nodiscard]] float get_scale() const { return value.x; }
  void set_scale(const float t_scale) {
    value.x = t_scale;
    value.y = t_scale;
  }
};
