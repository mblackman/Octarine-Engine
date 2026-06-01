#pragma once

#include "Lua/Modules/LuaModule.h"

struct AudioModule {};

template <>
struct LuaModuleBinding<AudioModule> {
  static void install(sol::state& lua, Game& game);
};
