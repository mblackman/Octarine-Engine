#pragma once

#include <SDL2/SDL_keycode.h>

#include <glm/glm.hpp>

#include "../Components/KeyboardControlComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyInputEvent.h"
#include "../General/Logger.h"

class KeyboardControlSystem : public System {
   public:
    KeyboardControlSystem() {
        RequireComponent<KeyboardControlComponent>();
        RequireComponent<RigidBodyComponent>();
        RequireComponent<SpriteComponent>();
    }

    ~KeyboardControlSystem() = default;

    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
        eventBus->SubscribeEvent<KeyboardControlSystem, KeyInputEvent>(this, &KeyboardControlSystem::OnKeyInput);
    }

    void OnKeyInput(KeyInputEvent& event) {
        for (auto entity : GetEntities()) {
            auto& keyboardComponent = entity.GetComponent<KeyboardControlComponent>();
            auto& spriteComponent = entity.GetComponent<SpriteComponent>();
            auto& rigidBodyComponent = entity.GetComponent<RigidBodyComponent>();

            if (!event.isPressed) {
                rigidBodyComponent.velocity = glm::vec2(0, 0);
                continue;
            }

            switch (event.inputKey) {
                case SDLK_UP:
                    rigidBodyComponent.velocity = glm::vec2(0, -keyboardComponent.velocity);
                    spriteComponent.srcRect.y = spriteComponent.height * 0;
                    break;
                case SDLK_DOWN:
                    rigidBodyComponent.velocity = glm::vec2(0, keyboardComponent.velocity);
                    spriteComponent.srcRect.y = spriteComponent.height * 2;
                    break;
                case SDLK_LEFT:
                    rigidBodyComponent.velocity = glm::vec2(-keyboardComponent.velocity, 0);
                    spriteComponent.srcRect.y = spriteComponent.height * 3;
                    break;
                case SDLK_RIGHT:
                    rigidBodyComponent.velocity = glm::vec2(keyboardComponent.velocity, 0);
                    spriteComponent.srcRect.y = spriteComponent.height * 1;
                    break;
            }
        }
    }

    void Update() {
    }
};