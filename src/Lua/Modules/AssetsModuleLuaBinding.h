#pragma once

#include "Lua/Modules/LuaModule.h"

struct AssetsModule {};

template <>
struct LuaModuleBinding<AssetsModule> {
  static void install(sol::state& lua, LuaBindingContext& ctx);
};
