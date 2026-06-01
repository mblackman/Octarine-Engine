#pragma once

#include "Lua/Modules/LuaModule.h"

struct GameModule {};

template <>
struct LuaModuleBinding<GameModule> {
  static void install(sol::state& lua, Game& game);
};
