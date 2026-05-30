#include "Lua/Modules/RegisterAllModules.h"

#include "Lua/Modules/AssetsModuleLuaBinding.h"
#include "Lua/Modules/AudioModuleLuaBinding.h"
#include "Lua/Modules/EntityModuleLuaBinding.h"
#include "Lua/Modules/GameModuleLuaBinding.h"
#include "Lua/Modules/IoModuleLuaBinding.h"
#include "Lua/Modules/LogModuleLuaBinding.h"
#include "Lua/Modules/LuaModule.h"
#include "Lua/Modules/SceneModuleLuaBinding.h"

const std::vector<LuaModuleDescriptor> kLuaModules = {
    {"Log",    &LuaModuleBinding<LogModule>::install},
    {"Io",     &LuaModuleBinding<IoModule>::install},
    {"Assets", &LuaModuleBinding<AssetsModule>::install},
    {"Audio",  &LuaModuleBinding<AudioModule>::install},
    {"Entity", &LuaModuleBinding<EntityModule>::install},
    {"Scene",  &LuaModuleBinding<SceneModule>::install},
    {"Game",   &LuaModuleBinding<GameModule>::install},
};

void RegisterAllLuaModules(sol::state& lua, Game& game)
{
    for (const auto& m : kLuaModules) m.install(lua, game);
}
