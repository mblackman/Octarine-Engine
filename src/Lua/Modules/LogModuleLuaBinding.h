#pragma once

#include "Lua/Modules/LuaModule.h"

struct LogModule
{
};

template <>
struct LuaModuleBinding<LogModule>
{
    static void install(sol::state& lua, Game& game);
};
