#pragma once

#include <sol/sol.hpp>
#include <string>
#include <utility>

struct ScriptComponent {
  sol::table scriptTable;
  sol::protected_function updateFunction;
  sol::protected_function onDebugGUIFunction;
  sol::protected_function onCollisionFunction;
  sol::protected_function onCollisionExitFunction;
  // Absolute path of the script file this component was loaded from, when the author opted into
  // the file-backed form (`script = { source = "scripts/foo.lua" }`). Empty for inline tables —
  // those are not eligible for hot reload.
  std::string sourcePath;

  explicit ScriptComponent(sol::table t_scriptTable = sol::lua_nil,
                           sol::protected_function t_updateFunction = sol::lua_nil,
                           sol::protected_function t_onDebugGUIFunction = sol::lua_nil,
                           sol::protected_function t_onCollisionFunction = sol::lua_nil,
                           sol::protected_function t_onCollisionExitFunction = sol::lua_nil,
                           std::string t_sourcePath = {})
      : scriptTable(std::move(t_scriptTable)),
        updateFunction(std::move(t_updateFunction)),
        onDebugGUIFunction(std::move(t_onDebugGUIFunction)),
        onCollisionFunction(std::move(t_onCollisionFunction)),
        onCollisionExitFunction(std::move(t_onCollisionExitFunction)),
        sourcePath(std::move(t_sourcePath)) {}
};
