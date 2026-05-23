#pragma once

#include "Lua/Modules/LuaModule.h"

struct IoModule
{
};

template <>
struct LuaModuleBinding<IoModule>
{
    static void install(sol::state& lua, Game& game);
};
