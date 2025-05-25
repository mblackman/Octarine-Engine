#include "MapEditor.h"

#include "../Game/ECSLoader.h"

void MapEditor::Load(sol::state& lua, const std::unique_ptr<Registry>& registry, const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer) {
    std::string filePath = "./assets/scripts/map-editor.lua";
    sol::load_result script = lua.load_file(filePath);

    if (!script.valid()) {
        sol::error error = script;
        Logger::Error("Level script is not valid: " + std::string(error.what()));
        return;
    }

    lua.script_file(filePath);
    Logger::Info("Just opened map-editor.lua");

    ECSLoader loader{};
    sol::table document = lua["document"];

    loader.LoadEntity(document, registry);

    // Load assets for the editor
    sol::table assets = document["assets"];
    int i = 0;

    while (true) {
        sol::optional<sol::table> hasAsset = assets[i];
        if (!hasAsset) {
            break;
        }

        sol::table asset = assets[i];
        loader.LoadAsset(asset, assetManager, renderer);
        i++;
    }

    // Load entities for the editor
    sol::table entities = document["entities"];
    i = 0;

    while (true) {
        sol::optional<sol::table> hasEntity = entities[i];
        if (hasEntity == sol::nullopt) {
            break;
        }

        sol::table entity = entities[i];
        loader.LoadEntity(entity, registry);
        i++;
    }
}