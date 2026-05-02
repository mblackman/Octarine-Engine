#pragma once

#include <SDL2/SDL.h>

#include <sol/sol.hpp>

#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/UIButtonComponent.h"
#include "../ECS/ECS.h"
#include "../Events/MouseInputEvent.h"

class UIButtonSystem : public System {
   public:
    UIButtonSystem() {
        RequireComponent<UIButtonComponent>();
    }

    ~UIButtonSystem() = default;

    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
        eventBus->SubscribeEvent<UIButtonSystem, MouseInputEvent>(this, &UIButtonSystem::OnMouseInput);
    }

    void OnMouseInput(MouseInputEvent& event) {
        if (event.event.type != SDL_MOUSEBUTTONDOWN || event.event.button != 1) {
            return;
        }

        for (auto entity : GetEntities()) {
            auto button = entity.GetComponent<UIButtonComponent>();
            if (button.clickFunction == sol::lua_nil || !entity.HasComponent<BoxColliderComponent>() || !entity.HasComponent<TransformComponent>()) {
                continue;
            }

            auto boxCollider = entity.GetComponent<BoxColliderComponent>();
            auto transform = entity.GetComponent<TransformComponent>();
            int mouseX = event.event.x;
            int mouseY = event.event.y;
            bool isClick = transform.position.x <= mouseX &&
                           (transform.position.x + boxCollider.width * transform.scale.x) >= mouseX &&
                           transform.position.y <= mouseY &&
                           (transform.position.y + boxCollider.height * transform.scale.y) >= mouseY;
            if (isClick) {
                button.clickFunction(button.buttonTable, entity);
            }
        }
    }
};