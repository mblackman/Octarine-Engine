#include "Lua/Modules/RegisterAllModules.h"

#include "Lua/Modules/AssetsModuleLuaBinding.h"
#include "Lua/Modules/AudioModuleLuaBinding.h"
#include "Lua/Modules/EntityModuleLuaBinding.h"
#include "Lua/Modules/GameModuleLuaBinding.h"
#include "Lua/Modules/IoModuleLuaBinding.h"
#include "Lua/Modules/LogModuleLuaBinding.h"
#include "Lua/Modules/LuaModule.h"
#include "Lua/Modules/SceneModuleLuaBinding.h"

void RegisterAllLuaModules(sol::state& lua, Game& game)
{
    LuaModuleBinding<LogModule>::install(lua, game);
    LuaModuleBinding<IoModule>::install(lua, game);
    LuaModuleBinding<AssetsModule>::install(lua, game);
    LuaModuleBinding<AudioModule>::install(lua, game);
    LuaModuleBinding<EntityModule>::install(lua, game);
    LuaModuleBinding<SceneModule>::install(lua, game);
    LuaModuleBinding<GameModule>::install(lua, game);
}
