#pragma once

#include <SDL2/SDL_keycode.h>

#include <string>

struct KeyInputEvent : public Event {
   public:
    SDL_Keycode inputKey;
    SDL_Keymod inputModifier;
    bool isPressed;

    KeyInputEvent(SDL_Keycode inputKey, SDL_Keymod inputModifier, bool isPressed)
        : inputKey(inputKey),
          inputModifier(inputModifier),
          isPressed(isPressed) {}
};