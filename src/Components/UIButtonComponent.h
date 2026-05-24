#pragma once

#include <sol/sol.hpp>
#include <utility>

struct UIButtonComponent {
  bool isActive;
  bool isFixed;
  sol::optional<sol::table> buttonTable;
  sol::protected_function clickFunction;

  explicit UIButtonComponent(const bool t_isActive = true, const bool t_isFixed = true,
                             sol::optional<sol::table> t_buttonTable = sol::nullopt,
                             sol::protected_function t_clickFunction = sol::lua_nil)
      : isActive(t_isActive),
        isFixed(t_isFixed),
        buttonTable(std::move(t_buttonTable)),
        clickFunction(std::move(t_clickFunction)) {}
};