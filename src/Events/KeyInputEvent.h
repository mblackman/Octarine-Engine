#pragma once

#include <SDL2/SDL_keycode.h>

#include <string>

struct KeyInputEvent : Event {
  SDL_Keycode inputKey;
  SDL_Keymod inputModifier;
  bool isPressed;

  KeyInputEvent(const SDL_Keycode inputKey, const SDL_Keymod inputModifier,
                const bool isPressed)
    : inputKey(inputKey),
      inputModifier(inputModifier),
      isPressed(isPressed) {}
};