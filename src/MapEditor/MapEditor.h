#pragma once

#include <SDL2/SDL.h>

#include <sol/sol.hpp>

#include "../AssetManager/AssetManager.h"
#include "../ECS/ECS.h"

class MapEditor {
   public:
    MapEditor() = default;
    ~MapEditor() = default;

    void Load(
        sol::state& lua,
        const std::unique_ptr<Registry>& registry,
        const std::unique_ptr<AssetManager>& assetManager,
        SDL_Renderer* renderer);
};