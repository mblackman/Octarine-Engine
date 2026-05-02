#pragma once

#include <SDL3/SDL.h>

#include <string>

struct MouseInputEvent : public Event {
 public:
  SDL_MouseButtonEvent event;

  explicit MouseInputEvent(SDL_MouseButtonEvent& event) : event(event) {}
};