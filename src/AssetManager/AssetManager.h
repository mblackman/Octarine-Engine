#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <map>
#include <string>

class AssetManager {
   private:
    std::map<std::string, SDL_Texture*> textures_;
    std::map<std::string, TTF_Font*> fonts_;

   public:
    AssetManager();
    ~AssetManager();

    void ClearAssets();
    void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& path);
    SDL_Texture* GetTexture(const std::string& assetId) const;
    void AddFont(const std::string& assetId, const std::string& path, const int fontSize);
    TTF_Font* GetFont(const std::string& assetId) const;
};