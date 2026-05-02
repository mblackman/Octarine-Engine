#include "LevelLoader.h"

#include <fstream>
#include <sstream>
#include <string>

#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../Game/Game.h"
#include "../General/Logger.h"
#include "./ECSLoader.h"

LevelLoader::LevelLoader() {
    Logger::Info("Level Loader created.");
}

LevelLoader::~LevelLoader() {
    Logger::Info("Level Loader destroyed.");
}

void LevelLoader::LoadLevel(sol::state& lua, const std::unique_ptr<Registry>& registry, const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, int levelNumber) {
    auto filePath = "./assets/scripts/level" + std::to_string(levelNumber) + ".lua";
    sol::load_result script = lua.load_file(filePath);

    if (!script.valid()) {
        sol::error error = script;
        Logger::Error("Level script is not valid: " + std::string(error.what()));
        return;
    }

    lua.script_file(filePath);
    Logger::Info("Just opened level1.lua");

    ECSLoader ecsLoader{};
    sol::table level = lua["Level"];

    // Read level assets
    sol::table assets = level["assets"];
    int i = 0;

    while (true) {
        sol::optional<sol::table> hasAsset = assets[i];
        if (!hasAsset) {
            break;
        }

        sol::table asset = assets[i];
        ecsLoader.LoadAsset(asset, assetManager, renderer);
        i++;
    }

    // Load the tilemap
    sol::table tileMap = level["tilemap"];
    std::string mapFile = tileMap["map_file"];
    std::ifstream file(mapFile);
    std::string line;
    int tileMapColumns = tileMap["num_cols"];
    std::string tileMapTextureId = tileMap["texture_asset_id"];
    int rowNumber = 0;
    int columnNumber = 0;
    int tileWidth = tileMap["tile_size"];
    int tileHeight = tileMap["tile_size"];
    double tileMapScale = tileMap["scale"];

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string value;
        std::vector<std::string> row;
        columnNumber = 0;

        while (std::getline(ss, value, ',')) {
            row.push_back(value);
        }

        for (const auto& item : row) {
            int value = std::stoi(item);
            int rowIndex = value / tileMapColumns;
            int columnIndex = value % tileMapColumns;

            auto tile = registry->CreateEntity();
            tile.Group("tiles");
            tile.AddComponent<TransformComponent>(glm::vec2(tileWidth * columnNumber * tileMapScale, tileHeight * rowNumber * tileMapScale), glm::vec2(tileMapScale, tileMapScale), 0.0);
            tile.AddComponent<SpriteComponent>(tileMapTextureId, tileWidth, tileHeight, 0, false, tileWidth * columnIndex, tileHeight * rowIndex);
            columnNumber++;
        }

        rowNumber++;
    }
    file.close();

    Game::mapWidth = columnNumber * tileWidth * tileMapScale;
    Game::mapHeight = rowNumber * tileHeight * tileMapScale;

    lua["map_width"] = Game::mapWidth;
    lua["map_height"] = Game::mapHeight;

    // Create entities
    sol::table entities = level["entities"];
    i = 0;
    while (true) {
        sol::optional<sol::table> hasEntity = entities[i];
        if (hasEntity == sol::nullopt) {
            break;
        }

        sol::table entity = entities[i];
        ecsLoader.LoadEntity(entity, registry);
        i++;
    }
}