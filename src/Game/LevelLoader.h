#pragma once

#include <SDL2/SDL.h>

#include <memory>
#include <sol/sol.hpp>

#include "../AssetManager/AssetManager.h"
#include "../ECS/ECS.h"

class LevelLoader {
   public:
    LevelLoader();
    ~LevelLoader();
    void LoadLevel(
        sol::state& lua,
        const std::unique_ptr<Registry>& registry,
        const std::unique_ptr<AssetManager>& assetManager,
        SDL_Renderer* renderer,
        int levelNumber);
};