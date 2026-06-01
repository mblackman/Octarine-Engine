#include "Lua/Modules/AssetsModuleLuaBinding.h"

#include <string>

#include "AssetManager/AssetManager.h"
#include "ECS/Registry.h"
#include "Game/Game.h"

void LuaModuleBinding<AssetsModule>::install(sol::state& lua, Game& game) {
  lua.set_function("get_asset_path", [&game](const std::string& relativePath) {
    return game.GetRegistry()->Get<AssetManager>().GetFullPath(relativePath);
  });
}
