#pragma once

#include <SDL3/SDL.h>

#include <string>

#include "EventBus/Event.h"

struct GamepadButtonEvent : Event {
  SDL_JoystickID which = 0;
  SDL_GamepadButton button = SDL_GAMEPAD_BUTTON_INVALID;
  std::string buttonName = {};
  bool isPressed = false;

  GamepadButtonEvent(const SDL_JoystickID t_which, const SDL_GamepadButton t_button, std::string t_buttonName,
                     const bool t_isPressed)
      : which(t_which), button(t_button), buttonName(std::move(t_buttonName)), isPressed(t_isPressed) {}
};
