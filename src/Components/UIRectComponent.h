#pragma once

#include <glm/glm.hpp>

struct UIRectComponent {
  float left = 0.f;
  float top = 0.f;
  float right = 0.f;
  float bottom = 0.f;
  int layer = 0;  // computed by UILayoutSystem from canvas.baseLayer + accumulated UIZIndexComponent.z

  explicit UIRectComponent(const float t_left = 0.f, const float t_top = 0.f, const float t_right = 0.f,
                           const float t_bottom = 0.f, const int t_layer = 0)
      : left(t_left), top(t_top), right(t_right), bottom(t_bottom), layer(t_layer) {}

  [[nodiscard]] float Width() const { return right - left; }
  [[nodiscard]] float Height() const { return bottom - top; }
  [[nodiscard]] glm::vec2 Center() const {
    static constexpr float kHalf = 0.5f;
    return {(left + right) * kHalf, (top + bottom) * kHalf};
  }
};
