#pragma once

#include "Lua/Modules/LuaModule.h"

struct UIModule {};

template <>
struct LuaModuleBinding<UIModule> {
  static void install(sol::state& lua, LuaBindingContext& ctx);
};
