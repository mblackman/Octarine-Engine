#pragma once

#include <SDL3/SDL.h>

#include <memory>
#include <sol/sol.hpp>

#include "../Components/BoxColliderComponent.h"
#include "../Components/GlobalTransformComponent.h"
#include "../Components/UIButtonComponent.h"
#include "../EventBus/EventBus.h"
#include "../Components/ViewportInfo.h"
#include "../Events/MouseInputEvent.h"
#include "../Game/GameConfig.h"
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

    if (registry_) {
      const auto& viewport = registry_->Get<ViewportInfo>();
      if (!viewport.isHovered) {
        return;
      }
    }

    float mouseX = event.event.x;
    float mouseY = event.event.y;

    if (registry_) {
      const auto& viewport = registry_->Get<ViewportInfo>();
      const auto& config = registry_->Get<GameConfig>();
      const glm::vec2 transformed = viewport.TransformCoordinates(mouseX, mouseY, config.windowWidth, config.windowHeight);
      mouseX = transformed.x;
      mouseY = transformed.y;
    }

    auto query = registry_->CreateQuery<UIButtonComponent, GlobalTransformComponent, BoxColliderComponent>();
    auto handler = [&](Entity entity, UIButtonComponent& button, const GlobalTransformComponent& transform,
                       const BoxColliderComponent& collider) {
      if (!button.isActive || button.clickFunction == sol::lua_nil) {
        return;
      }

      // transform.position is top-left. apply collider offset (scaled).
      const float x = transform.position.x + collider.offset.x * transform.scale.x;
      const float y = transform.position.y + collider.offset.y * transform.scale.y;

      const float w = static_cast<float>(collider.width) * transform.scale.x;
      const float h = static_cast<float>(collider.height) * transform.scale.y;

      const bool isClick = mouseX >= x && mouseX <= x + w && mouseY >= y && mouseY <= y + h;

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
