#pragma once

struct UIAnchorComponent {
  float anchorLeft = 0.f;
  float anchorTop = 0.f;
  float anchorRight = 1.f;
  float anchorBottom = 1.f;
  float offsetLeft = 0.f;
  float offsetTop = 0.f;
  float offsetRight = 0.f;
  float offsetBottom = 0.f;

  explicit UIAnchorComponent(const float t_anchorLeft = 0.f, const float t_anchorTop = 0.f,
                             const float t_anchorRight = 1.f, const float t_anchorBottom = 1.f,
                             const float t_offsetLeft = 0.f, const float t_offsetTop = 0.f,
                             const float t_offsetRight = 0.f, const float t_offsetBottom = 0.f)
      : anchorLeft(t_anchorLeft),
        anchorTop(t_anchorTop),
        anchorRight(t_anchorRight),
        anchorBottom(t_anchorBottom),
        offsetLeft(t_offsetLeft),
        offsetTop(t_offsetTop),
        offsetRight(t_offsetRight),
        offsetBottom(t_offsetBottom) {}
};
