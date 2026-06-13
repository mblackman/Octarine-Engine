#pragma once

#include "Lua/Modules/LuaModule.h"

struct ProjectModule {};

template <>
struct LuaModuleBinding<ProjectModule> {
  static void install(sol::state& lua, LuaBindingContext& ctx);
};
