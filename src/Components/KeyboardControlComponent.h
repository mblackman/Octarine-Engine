#pragma once

struct KeyboardControlComponent {
  double velocity;

  explicit KeyboardControlComponent(double velocity = 0.0) : velocity(velocity) {}
};
