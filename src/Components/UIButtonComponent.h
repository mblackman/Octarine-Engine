#pragma once

#include <utility>

struct UIButtonComponent {
  bool isActive;
  sol::optional<sol::table> buttonTable;
  sol::protected_function clickFunction;

  explicit UIButtonComponent(const bool isActive = true, sol::optional<sol::table> buttonTable = sol::nullopt,
                             sol::protected_function clickFunction = sol::lua_nil)
      : isActive(isActive),
        buttonTable(std::move(std::move(buttonTable))),
        clickFunction(std::move(std::move(clickFunction))) {}
};