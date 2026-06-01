#pragma once

#include "Lua/Modules/LuaModule.h"

struct EntityModule {};

template <>
struct LuaModuleBinding<EntityModule> {
  static void install(sol::state& lua, Game& game);
};
