#pragma once

#include "Lua/Modules/LuaModule.h"

struct SceneModule
{
};

template <>
struct LuaModuleBinding<SceneModule>
{
    static void install(sol::state& lua, Game& game);
};
