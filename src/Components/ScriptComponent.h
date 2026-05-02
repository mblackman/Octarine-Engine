#pragma once

#include <sol/sol.hpp>

struct ScriptComponent {
    sol::function updateFunction;

    ScriptComponent(sol::function updateFunction = sol::lua_nil) : updateFunction(updateFunction) {
    }
};
