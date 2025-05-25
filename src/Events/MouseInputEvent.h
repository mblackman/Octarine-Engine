#pragma once

#include <SDL2/SDL.h>

#include <string>

struct MouseInputEvent : public Event {
   public:
    SDL_MouseButtonEvent event;

    MouseInputEvent(SDL_MouseButtonEvent& event)
        : event(event) {}
};