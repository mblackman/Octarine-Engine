#include "Lua/Modules/SceneModuleLuaBinding.h"

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <string>
#include <utility>

#include <set>
#include <stdexcept>
#include <vector>

#include "AssetManager/AssetManager.h"
#include "AssetManager/SceneAssetScanner.h"
#include "ECS/Registry.h"
#include "Game/Game.h"
#include "Game/GameConfig.h"
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
    // Scan a scene table for its required asset ids (sprite/font/audio + tilemap + preload),
    // validate them against the catalog, then acquire each. The data-driven replacement for a
    // manual load_asset loop; scripts using a custom loader call this instead of listing assets by
    // hand. Returns the number of assets successfully acquired. Raises a Lua error when validation
    // fails and the assetValidationFatal dev gate is set.
    lua.set_function("acquire_scene_assets", [&game](const sol::table& scene) -> int
    {
        auto* registry = game.GetRegistry();
        auto& assetManager = registry->Get<AssetManager>();
        auto* mixerPtr = registry->TryGet<MIX_Mixer*>();
        MIX_Mixer* mixer = mixerPtr ? *mixerPtr : nullptr;

        const std::vector<AssetReference> refs = SceneAssetScanner::CollectRefs(scene);
        if (const int failures = assetManager.Validate(refs); failures > 0 &&
            registry->Get<GameConfig>().GetEngineOptions().assetValidationFatal)
        {
            throw std::runtime_error("acquire_scene_assets: " + std::to_string(failures) +
                                     " unresolved asset reference(s); assetValidationFatal is set.");
        }

        const int acquired = assetManager.AcquireAll(refs, game.GetRenderer(), mixer);

        // Record the acquired ids on the Game so the next scene swap / StopScene releases them.
        // Without this, scenes that load via a side-effect script (rather than returning a table
        // the C++ loader scans) would acquire on every reload and never release — refcounts climb.
        std::vector<std::string> ids;
        std::set<std::string> seen;
        for (const auto& ref : refs)
        {
            if (seen.insert(ref.id).second) ids.push_back(ref.id);
        }
        game.TrackSceneAssets(ids);

        Logger::Info("acquire_scene_assets: acquired " + std::to_string(acquired) + " asset(s).");
        return acquired;
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
