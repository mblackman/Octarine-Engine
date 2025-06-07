#pragma once

#include <sol/sol.hpp>
#include <utility>

struct ScriptComponent {
  sol::protected_function updateFunction;
  sol::protected_function onDebugGUIFunction;

  explicit ScriptComponent(sol::protected_function updateFunction = sol::lua_nil,
                           sol::protected_function onDebugGUIFunction = sol::lua_nil)
      : updateFunction(std::move(std::move(updateFunction))),
        onDebugGUIFunction(std::move(std::move(onDebugGUIFunction))) {}
};
