#pragma once

#include <sol/sol.hpp>
#include <utility>

struct ScriptComponent {
  sol::table scriptTable;
  sol::protected_function updateFunction;
#ifdef OCTARINE_WITH_IMGUI
  sol::protected_function onDebugGUIFunction;
#endif

  explicit ScriptComponent(sol::table t_scriptTable = sol::lua_nil,
                           sol::protected_function t_updateFunction = sol::lua_nil
#ifdef OCTARINE_WITH_IMGUI
                           ,
                           sol::protected_function t_onDebugGUIFunction = sol::lua_nil
#endif
                           )
      : scriptTable(std::move(t_scriptTable)),
        updateFunction(std::move(t_updateFunction))
#ifdef OCTARINE_WITH_IMGUI
        ,
        onDebugGUIFunction(std::move(t_onDebugGUIFunction))
#endif
  {}
};
