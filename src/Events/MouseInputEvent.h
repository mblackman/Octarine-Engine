#pragma once

#include <SDL3/SDL.h>

struct MouseInputEvent : Event {
  SDL_MouseButtonEvent event;

  explicit MouseInputEvent(const SDL_MouseButtonEvent& t_event) : event(t_event) {}
};