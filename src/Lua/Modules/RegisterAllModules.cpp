#include "Lua/Modules/RegisterAllModules.h"

#include "Lua/Modules/AssetsModuleLuaBinding.h"
#include "Lua/Modules/AudioModuleLuaBinding.h"
#include "Lua/Modules/EntityModuleLuaBinding.h"
#include "Lua/Modules/GameModuleLuaBinding.h"
#include "Lua/Modules/IoModuleLuaBinding.h"
#include "Lua/Modules/LogModuleLuaBinding.h"
#include "Lua/Modules/LuaModule.h"
#include "Lua/Modules/SceneModuleLuaBinding.h"
#include "Lua/Modules/StorageModuleLuaBinding.h"
#include "Lua/Modules/UIModuleLuaBinding.h"
#ifdef OCTARINE_WITH_EDITOR
#include "Lua/Modules/ProjectModuleLuaBinding.h"
#endif

const std::vector<LuaModuleDescriptor> kLuaModules = {
    {"Log", &LuaModuleBinding<LogModule>::install},
    {"Io", &LuaModuleBinding<IoModule>::install},
    {"Assets", &LuaModuleBinding<AssetsModule>::install},
    {"Audio", &LuaModuleBinding<AudioModule>::install},
    {"Entity", &LuaModuleBinding<EntityModule>::install},
    {"Scene", &LuaModuleBinding<SceneModule>::install},
    {"Game", &LuaModuleBinding<GameModule>::install},
    {"UI", &LuaModuleBinding<UIModule>::install},
    {"Storage", &LuaModuleBinding<StorageModule>::install},
#ifdef OCTARINE_WITH_EDITOR
    // Editor-only project-folder writes; install() additionally no-ops outside an editor session.
    {"Project", &LuaModuleBinding<ProjectModule>::install},
#endif
};

void RegisterAllLuaModules(sol::state& lua, LuaBindingContext& ctx) {
  for (const auto& m : kLuaModules) m.install(lua, ctx);
}
