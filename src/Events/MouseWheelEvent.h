#pragma once

#include "EventBus/Event.h"

struct MouseWheelEvent : Event {
  float dx;
  float dy;

  MouseWheelEvent(const float t_dx, const float t_dy) : dx(t_dx), dy(t_dy) {}
};
