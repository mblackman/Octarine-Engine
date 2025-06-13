#pragma once

#include <sol/sol.hpp>
#include <utility>

struct ScriptComponent {
  sol::table scriptTable;
  sol::protected_function updateFunction;
  sol::protected_function onDebugGUIFunction;

  explicit ScriptComponent(sol::table scriptTable = sol::lua_nil, sol::protected_function updateFunction = sol::lua_nil,
                           sol::protected_function onDebugGUIFunction = sol::lua_nil)
      : scriptTable(std::move(std::move(scriptTable))),
        updateFunction(std::move(std::move(updateFunction))),
        onDebugGUIFunction(std::move(std::move(onDebugGUIFunction))) {}
};
