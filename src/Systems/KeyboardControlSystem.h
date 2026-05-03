#pragma once

#include <SDL3/SDL_keycode.h>

#include <glm/glm.hpp>
#include <memory>

#include "../Components/KeyboardControlComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyInputEvent.h"
#include "ECS/Registry.h"

class KeyboardControlSystem {
 public:
  void operator()(const ContextFacade& /*context*/, const KeyboardControlComponent& keyboard,
                  RigidBodyComponent& rigidBody, SpriteComponent& sprite) const {
    const auto velocity = static_cast<float>(keyboard.velocity);

    glm::vec2 dir(0.0f, 0.0f);
    if (held_left_) dir.x -= 1.0f;
    if (held_right_) dir.x += 1.0f;
    if (held_up_) dir.y -= 1.0f;
    if (held_down_) dir.y += 1.0f;

    if (dir.x == 0.0f && dir.y == 0.0f) {
      rigidBody.velocity = glm::vec2(0, 0);
      return;
    }

    // Normalize so diagonal movement isn't faster.
    const float len = glm::length(dir);
    rigidBody.velocity = (dir / len) * velocity;

    // Sprite row: prefer the last-pressed direction so the animation tracks player intent.
    switch (last_dir_) {
      case Direction::Up:
        sprite.srcRect.y = sprite.height * 0;
        break;
      case Direction::Right:
        sprite.srcRect.y = sprite.height * 1;
        break;
      case Direction::Down:
        sprite.srcRect.y = sprite.height * 2;
        break;
      case Direction::Left:
        sprite.srcRect.y = sprite.height * 3;
        break;
      default:
        break;
    }
  }

  void SubscribeToEvents(const std::unique_ptr<EventBus>& eventBus) {
    eventBus->SubscribeEvent<KeyboardControlSystem, KeyInputEvent>(this, &KeyboardControlSystem::OnKeyInput);
  }

  void OnKeyInput(const KeyInputEvent& event) {
    const bool pressed = event.isPressed;
    switch (event.inputKey) {
      case SDLK_UP:
        held_up_ = pressed;
        if (pressed) last_dir_ = Direction::Up;
        break;
      case SDLK_DOWN:
        held_down_ = pressed;
        if (pressed) last_dir_ = Direction::Down;
        break;
      case SDLK_LEFT:
        held_left_ = pressed;
        if (pressed) last_dir_ = Direction::Left;
        break;
      case SDLK_RIGHT:
        held_right_ = pressed;
        if (pressed) last_dir_ = Direction::Right;
        break;
      default:
        break;
    }
  }

 private:
  enum class Direction { None, Up, Down, Left, Right };

  bool held_up_ = false;
  bool held_down_ = false;
  bool held_left_ = false;
  bool held_right_ = false;
  Direction last_dir_ = Direction::None;
};
