#pragma once

#include <SDL3/SDL.h>

#include <sol/sol.hpp>

#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/UIButtonComponent.h"
#include "../ECS/ECS.h"
#include "../Events/MouseInputEvent.h"

class UIButtonSystem : public System {
 public:
  UIButtonSystem() { RequireComponent<UIButtonComponent>(); }

  ~UIButtonSystem() = default;

  void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
    eventBus->SubscribeEvent<UIButtonSystem, MouseInputEvent>(
        this, &UIButtonSystem::OnMouseInput);
  }

  void OnMouseInput(MouseInputEvent& event) {
    if (event.event.type != SDL_EVENT_MOUSE_BUTTON_DOWN || event.event.button != 1) {
      return;
    }

    for (auto entity : GetEntities()) {
      auto button = entity.GetComponent<UIButtonComponent>();
      if (button.clickFunction == sol::lua_nil ||
          !entity.HasComponent<BoxColliderComponent>() ||
          !entity.HasComponent<TransformComponent>()) {
        continue;
      }

      const auto boxCollider = entity.GetComponent<BoxColliderComponent>();
      const auto transform = entity.GetComponent<TransformComponent>();
      const int mouseX = event.event.x;
      const int mouseY = event.event.y;
      const bool isClick =
          transform.position.x <= mouseX &&
          (transform.position.x + boxCollider.width * transform.scale.x) >=
              mouseX &&
          transform.position.y <= mouseY &&
          (transform.position.y + boxCollider.height * transform.scale.y) >=
              mouseY;
      if (isClick) {
        button.clickFunction(button.buttonTable, entity);
      }
    }
  }
};