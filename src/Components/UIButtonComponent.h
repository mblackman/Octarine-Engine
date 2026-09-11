#pragma once

#include <sol/sol.hpp>
#include <string>
#include <utility>

struct UIButtonComponent {
  bool isActive;
  bool isFixed;
  sol::optional<sol::table> buttonTable;
  sol::protected_function clickFunction;

  std::string key;
  std::string controllerButton;
  std::string action;

  explicit UIButtonComponent(const bool t_isActive = true, const bool t_isFixed = true,
                             sol::optional<sol::table> t_buttonTable = sol::nullopt,
                             sol::protected_function t_clickFunction = sol::lua_nil, std::string t_key = "",
                             std::string t_controllerButton = "", std::string t_action = "")
      : isActive(t_isActive),
        isFixed(t_isFixed),
        buttonTable(std::move(t_buttonTable)),
        clickFunction(std::move(t_clickFunction)),
        key(std::move(t_key)),
        controllerButton(std::move(t_controllerButton)),
        action(std::move(t_action)) {}
};