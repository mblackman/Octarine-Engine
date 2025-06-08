#pragma once

#include <SDL3/SDL_keycode.h>

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