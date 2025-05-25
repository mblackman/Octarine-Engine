#pragma once

#include <SDL2/SDL.h>

#include "../AssetManager/AssetManager.h"
#include "./RenderQueue.h"

class Renderer {
   public:
    Renderer() = default;
    ~Renderer() = default;

    void Render(const RenderQueue& renderQueue, SDL_Renderer* renderer, SDL_Rect& camera, std::unique_ptr<AssetManager>& assetManager);

   private:
    void RenderSprite(const Entity& entity, SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager, SDL_Rect& camera);
    void RenderSquare(const Entity& entity, SDL_Renderer* renderer, SDL_Rect& camera);
    void RenderText(const Entity& entity, SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager, SDL_Rect& camera);
};