#pragma once

#include <sol/sol.hpp>
#include <string>
#include <utility>

#include "Components/UIButtonComponent.h"
#include "ECS/Entity.h"
#include "General/Logger.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<UIButtonComponent> {
  static constexpr const char* kLuaKey = "ui_button";
  static constexpr const char* kUsertypeName = "ui_button_component";

  static UIButtonComponent fromLua(const sol::object& data) {
    const auto t = data.as<sol::table>();
    using namespace LuaComponentHelpers;
    const bool isActive = SafeGetOptionalValue<bool>(t, "is_active", true);
    const bool isFixed = SafeGetOptionalValue<bool>(t, "is_fixed", true);
    const sol::protected_function clickFn = SafeGetProtectedFunction(t, "on_click");

    auto key = SafeGetOptionalValue<std::string>(t, "key", "");
    if (key.empty()) {
      key = SafeGetOptionalValue<std::string>(t, "input_key", "");
    }
    auto controllerButton = SafeGetOptionalValue<std::string>(t, "controller_button", "");
    if (controllerButton.empty()) {
      controllerButton = SafeGetOptionalValue<std::string>(t, "gamepad_button", "");
    }
    auto action = SafeGetOptionalValue<std::string>(t, "action", "");

    return UIButtonComponent(isActive, isFixed, t, clickFn, std::move(key), std::move(controllerButton),
                             std::move(action));
  }

  static void bindUsertype(sol::state& lua) {
    lua.new_usertype<UIButtonComponent>(
        kUsertypeName, "is_active", &UIButtonComponent::isActive, "is_fixed", &UIButtonComponent::isFixed, "key",
        &UIButtonComponent::key, "input_key", &UIButtonComponent::key, "controller_button",
        &UIButtonComponent::controllerButton, "gamepad_button", &UIButtonComponent::controllerButton, "action",
        &UIButtonComponent::action, "on_click", &UIButtonComponent::clickFunction, "trigger",
        [](UIButtonComponent& self, sol::optional<Entity> entity) {
          if (!self.isActive) return false;
          if (self.clickFunction != sol::lua_nil) {
            auto btnTable = self.buttonTable ? *self.buttonTable : sol::lua_nil;
            auto result = entity ? self.clickFunction(btnTable, *entity) : self.clickFunction(btnTable);
            if (!result.valid()) {
              const sol::error err = result;
              Logger::ErrorLua(std::string(err.what()));
              return false;
            }
            return true;
          }
          return false;
        });
  }
};
