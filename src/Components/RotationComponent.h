#pragma once

// Rotation about the entity's anchor, always in radians — the project's AngleUnit is converted at
// the Lua and editor boundaries. The anchor itself lives in the optional PivotComponent.
struct RotationComponent {
  float value;

  explicit RotationComponent(const float t_value = 0.0F) : value(t_value) {}
};
