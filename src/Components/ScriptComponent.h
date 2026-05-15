#pragma once

#include <sol/sol.hpp>
#include <utility>

struct ScriptComponent {
  sol::table scriptTable;
  sol::protected_function updateFunction;
  sol::protected_function onDebugGUIFunction;

  explicit ScriptComponent(sol::table t_scriptTable = sol::lua_nil,
                           sol::protected_function t_updateFunction = sol::lua_nil,
                           sol::protected_function t_onDebugGUIFunction = sol::lua_nil)
      : scriptTable(std::move(t_scriptTable)),
        updateFunction(std::move(t_updateFunction)),
        onDebugGUIFunction(std::move(t_onDebugGUIFunction)) {}
};
