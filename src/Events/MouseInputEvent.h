#pragma once

#include <SDL3/SDL.h>

struct MouseInputEvent : Event {
  SDL_MouseButtonEvent event;

  explicit MouseInputEvent(const SDL_MouseButtonEvent& event) : event(event) {}
};