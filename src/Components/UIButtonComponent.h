#pragma once

#include <utility>

struct UIButtonComponent {
  bool isActive;
  sol::optional<sol::table> buttonTable;
  sol::protected_function clickFunction;

  explicit UIButtonComponent(const bool t_isActive = true, sol::optional<sol::table> t_buttonTable = sol::nullopt,
                             sol::protected_function t_clickFunction = sol::lua_nil)
      : isActive(t_isActive), buttonTable(std::move(t_buttonTable)), clickFunction(std::move(t_clickFunction)) {}
};