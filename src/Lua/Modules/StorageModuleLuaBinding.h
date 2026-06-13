#pragma once

#include "Lua/Modules/LuaModule.h"

struct StorageModule {};

template <>
struct LuaModuleBinding<StorageModule> {
  static void install(sol::state& lua, LuaBindingContext& ctx);
};
