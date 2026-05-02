#pragma once

#include <sol/sol.hpp>
#include <utility>
#include <utility>

struct ScriptComponent {
  sol::function updateFunction;

  explicit ScriptComponent(sol::function updateFunction = sol::lua_nil)
      : updateFunction(std::move(std::move(updateFunction))) {}
};
