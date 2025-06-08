#pragma once

struct KeyboardControlComponent {
  double velocity;

  explicit KeyboardControlComponent(const double velocity = 0.0) : velocity(velocity) {}
};
