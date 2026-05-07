#pragma once

struct KeyboardControlComponent {
  double velocity;

  explicit KeyboardControlComponent(const double t_velocity = 0.0) : velocity(t_velocity) {}
};
