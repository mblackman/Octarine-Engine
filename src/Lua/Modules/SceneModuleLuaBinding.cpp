#include "Lua/Modules/SceneModuleLuaBinding.h"

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <filesystem>
#include <string>
#include <utility>

#include <set>
#include <stdexcept>
#include <vector>

#include "AssetManager/AssetManager.h"
#include "AssetManager/SceneAssetScanner.h"
#include "ECS/Registry.h"
#include "Engine/EngineContext.h"
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
        auto& assetManager = registry->Get<AssetManager>();

        // Under bake there is no renderer/mixer to upload to, so skip the GPU/mixer load. But
        // load_asset is the explicit-file legacy path (it bypasses the catalog), so the bake's
        // catalog-membership check never sees these refs — validate the file on disk here instead so
        // a typo'd path in a direct load_asset call still fails the bake gate. The FS is real at bake.
        if (game.IsBakeMode())
        {
            const std::string file = assetTable["file"].get_or<std::string>("");
            const std::string id = assetTable["id"].get_or<std::string>("");
            std::error_code ec;
            if (file.empty() || !std::filesystem::exists(assetManager.GetFullPath(file), ec))
            {
                Logger::Error("load_asset (bake): id '" + id + "' maps to a missing file: '" +
                    (file.empty() ? "<none>" : assetManager.GetFullPath(file)) + "'");
                game.RecordBakeValidationFailures(1);
            }
            return;
        }

        LoadAsset(std::move(assetTable), assetManager, game.GetRenderer(), game.GetContext().mixer);
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
        MIX_Mixer* mixer = game.GetContext().mixer;

        const std::vector<AssetReference> refs = SceneAssetScanner::CollectRefs(scene);
        const int failures = assetManager.Validate(refs);

        // Bake: tally unresolved references across every scene the startup script loads, then keep
        // running (don't throw on the first miss) so one bake run reports them all. No GPU upload.
        if (game.IsBakeMode())
        {
            game.RecordBakeValidationFailures(failures);
            Logger::Info("acquire_scene_assets (bake): validated " + std::to_string(refs.size()) +
                " reference(s), " + std::to_string(failures) + " unresolved.");
            return 0;
        }

        if (failures > 0 && registry->Get<GameConfig>().GetEngineOptions().assetValidationFatal)
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
