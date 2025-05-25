#include <utility>

#include <utility>

#pragma once

struct UIButtonComponent {
 public:
  bool isActive;
  sol::optional<sol::table> buttonTable;
  sol::function clickFunction;

  explicit UIButtonComponent(bool isActive = true,
                    sol::optional<sol::table> buttonTable = sol::nullopt,
                    sol::function clickFunction = sol::lua_nil)
      : isActive(isActive),
        buttonTable(std::move(std::move(buttonTable))),
        clickFunction(std::move(std::move(clickFunction))) {}
};