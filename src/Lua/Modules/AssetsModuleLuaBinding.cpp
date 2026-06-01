#include "Lua/Modules/AssetsModuleLuaBinding.h"

#include <string>

#include "AssetManager/AssetManager.h"
#include "ECS/Registry.h"
#include "Lua/LuaBindingContext.h"

void LuaModuleBinding<AssetsModule>::install(sol::state& lua, LuaBindingContext& ctx) {
  lua.set_function("get_asset_path", [&ctx](const std::string& relativePath) {
    return ctx.GetRegistry()->Get<AssetManager>().GetFullPath(relativePath);
  });
}
