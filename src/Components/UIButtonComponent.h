#pragma once

struct UIButtonComponent {
   public:
    bool isActive;
    sol::optional<sol::table> buttonTable;
    sol::function clickFunction;

    UIButtonComponent(bool isActive = true, sol::optional<sol::table> buttonTable = sol::nullopt, sol::function clickFunction = sol::lua_nil) : isActive(isActive), buttonTable(buttonTable), clickFunction(clickFunction) {
    }
};