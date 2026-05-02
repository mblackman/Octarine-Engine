#pragma once

#include <SDL3/SDL_keycode.h>

#include <glm/glm.hpp>
#include <memory>

#include "../Components/KeyboardControlComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyInputEvent.h"
#include "ECS/Iterable.h"
#include "ECS/Registry.h"

class KeyboardControlSystem {
 public:
  void operator()(const ContextFacade& /*context*/, const KeyboardControlComponent& keyboard,
                  RigidBodyComponent& rigidBody, SpriteComponent& sprite) const {
    if (!isPressed_) {
      rigidBody.velocity = glm::vec2(0, 0);
      return;
    }

    const auto velocity = static_cast<float>(keyboard.velocity);
    switch (inputKey_) {
      case SDLK_UP:
        rigidBody.velocity = glm::vec2(0, -velocity);
        sprite.srcRect.y = sprite.height * 0;
        break;
      case SDLK_DOWN:
        rigidBody.velocity = glm::vec2(0, velocity);
        sprite.srcRect.y = sprite.height * 2;
        break;
      case SDLK_LEFT:
        rigidBody.velocity = glm::vec2(-velocity, 0);
        sprite.srcRect.y = sprite.height * 3;
        break;
      case SDLK_RIGHT:
        rigidBody.velocity = glm::vec2(velocity, 0);
        sprite.srcRect.y = sprite.height * 1;
        break;
      default:
        rigidBody.velocity = glm::vec2(0, 0);
        break;
    }
  }

  void SubscribeToEvents(const std::unique_ptr<EventBus>& eventBus) {
    eventBus->SubscribeEvent<KeyboardControlSystem, KeyInputEvent>(this, &KeyboardControlSystem::OnKeyInput);
  }

  void OnKeyInput(const KeyInputEvent& event) {
    inputKey_ = event.inputKey;
    isPressed_ = event.isPressed;
  }

 private:
  SDL_Keycode inputKey_ = 0;
  bool isPressed_ = false;
};
