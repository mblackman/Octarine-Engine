#pragma once

#include <SDL3/SDL_keycode.h>

struct KeyInputEvent : Event {
  SDL_Keycode inputKey;
  SDL_Keymod inputModifier;
  bool isPressed;

  KeyInputEvent(const SDL_Keycode t_inputKey, const SDL_Keymod t_inputModifier, const bool t_isPressed)
      : inputKey(t_inputKey), inputModifier(t_inputModifier), isPressed(t_isPressed) {}
};