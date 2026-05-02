#include "AssetManager.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../General/Logger.h"

AssetManager::AssetManager() {
}

AssetManager::~AssetManager() {
    ClearAssets();
}

void AssetManager::ClearAssets() {
    for (auto texture : textures_) {
        SDL_DestroyTexture(texture.second);
    }
    for (auto font : fonts_) {
        TTF_CloseFont(font.second);
    }

    textures_.clear();
    fonts_.clear();
}

void AssetManager::AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    textures_.emplace(assetId, texture);

    Logger::Info("Added texture: " + assetId + " from path: " + path);
}

SDL_Texture* AssetManager::GetTexture(const std::string& assetId) const {
    return textures_.at(assetId);
}

void AssetManager::AddFont(const std::string& assetId, const std::string& path, const int fontSize) {
    TTF_Font* font = TTF_OpenFont(path.c_str(), fontSize);
    fonts_.emplace(assetId, font);

    Logger::Info("Added font: " + assetId + " from path: " + path);
}

TTF_Font* AssetManager::GetFont(const std::string& assetId) const {
    return fonts_.at(assetId);
}