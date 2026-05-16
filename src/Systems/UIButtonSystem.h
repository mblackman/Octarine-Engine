#pragma once

#include <SDL3/SDL.h>

#include <memory>
#include <sol/sol.hpp>

#include "../Components/BoxColliderComponent.h"
#include "../Components/GlobalTransformComponent.h"
#include "../Components/UIButtonComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/MouseInputEvent.h"
#include "../General/Logger.h"
#include "ECS/Query.h"
#include "ECS/Registry.h"

class UIButtonSystem {
 public:
  void Init(Registry* registry, const std::unique_ptr<EventBus>& eventBus) {
    registry_ = registry;
    eventBus->SubscribeEvent<UIButtonSystem, MouseInputEvent>(this, &UIButtonSystem::OnMouseInput);
  }

  void OnMouseInput(const MouseInputEvent& event) {
    if (event.event.type != SDL_EVENT_MOUSE_BUTTON_DOWN || event.event.button != 1) {
      return;
    }

    const float mouseX = event.event.x;
    const float mouseY = event.event.y;

    auto query = registry_->CreateQuery<UIButtonComponent, GlobalTransformComponent, BoxColliderComponent>();
    auto handler = [&](Entity entity, UIButtonComponent& button, const GlobalTransformComponent& transform,
                       const BoxColliderComponent& collider) {
      if (!button.isActive || button.clickFunction == sol::lua_nil) {
        return;
      }

      const float w = static_cast<float>(collider.width) * transform.scale.x;
      const float h = static_cast<float>(collider.height) * transform.scale.y;
      const bool isClick = transform.position.x <= mouseX && transform.position.x + w >= mouseX &&
                           transform.position.y <= mouseY && transform.position.y + h >= mouseY;

      if (!isClick) return;

      if (auto result = button.clickFunction(button.buttonTable, entity); !result.valid()) {
        const sol::error err = result;
        Logger::ErrorLua(std::string(err.what()));
      }
    };
    query->ForEach(handler);
  }

 private:
  Registry* registry_ = nullptr;
};
