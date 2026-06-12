#pragma once

struct UICanvasComponent {
  bool isFixed = true;
  float width = 0.f;   // 0 = use viewport width at layout time
  float height = 0.f;  // 0 = use viewport height at layout time

  explicit UICanvasComponent(const bool t_isFixed = true, const float t_width = 0.f, const float t_height = 0.f)
      : isFixed(t_isFixed), width(t_width), height(t_height) {}
};
