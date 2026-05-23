#include "Lua/Modules/SceneModuleLuaBinding.h"

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <string>
#include <utility>

#include "AssetManager/AssetManager.h"
#include "ECS/Registry.h"
#include "Game/Game.h"
#include "General/Logger.h"
#include "Lua/LuaEntityLoader.h"

namespace
{
    void LoadAsset(sol::table assetTable, AssetManager& assetManager, SDL_Renderer* renderer, MIX_Mixer* mixer)
    {
        if (const std::string assetType = assetTable["type"]; assetType == "texture")
        {
            assetManager.AddTexture(renderer, assetTable["id"], assetTable["file"]);
        }
        else if (assetType == "font")
        {
            const float fontSize = assetTable["font_size"];
            assetManager.AddFont(assetTable["id"], assetTable["file"], fontSize);
        }
        else if (assetType == "audio_clip")
        {
            assetManager.AddAudioClip(mixer, assetTable["id"], assetTable["file"]);
        }
        else
        {
            Logger::Error("Unknown asset type: " + assetType);
        }
    }
} // namespace

void LuaModuleBinding<SceneModule>::install(sol::state& lua, Game& game)
{
    lua.set_function("load_asset", [&game](sol::table assetTable)
    {
        auto* registry = game.GetRegistry();
        LoadAsset(std::move(assetTable), registry->Get<AssetManager>(), game.GetRenderer(),
                  registry->Get<MIX_Mixer*>());
    });
    lua.set_function("load_entity", [&game](const sol::table& assetTable)
    {
        LuaEntityLoader::LoadEntityFromLua(game.GetRegistry(), assetTable);
    });
    lua.set_function("clear_scene", [&game]() { game.StopScene(); });
    lua.set_function("load_scene", [&game](const std::string& path) { game.LoadScene(path); });
    lua.set_function("reload_scene", [&game]() { game.ReloadScene(); });
    lua.set_function("stop_scene", [&game]() { game.StopScene(); });
}
