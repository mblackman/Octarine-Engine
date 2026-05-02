#include "AssetManager.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <filesystem>

#include "../General/Logger.h"

AssetManager::AssetManager(std::string basePath) : base_path_(
    std::move(basePath)) {
}

AssetManager::~AssetManager() { ClearAssets(); }

void AssetManager::ClearAssets() {
  for (const auto& texture : textures_) {
    SDL_DestroyTexture(texture.second);
  }
  for (const auto& font : fonts_) {
    TTF_CloseFont(font.second);
  }

  textures_.clear();
  fonts_.clear();
}

void AssetManager::AddTexture(SDL_Renderer* renderer,
                              const std::string& assetId,
                              const std::string& path) {
  const std::string fullPath = GetFullPath(path);

  SDL_Texture* texture = IMG_LoadTexture(renderer, fullPath.c_str());
  if (!texture) {
    Logger::Error("Failed to create texture: " + std::string(SDL_GetError()));
    return;
  }

  textures_.emplace(assetId, texture);

  Logger::Info("Added texture: " + assetId + " from path: " + fullPath);
}

SDL_Texture* AssetManager::GetTexture(const std::string& assetId) const {
  return textures_.at(assetId);
}

void AssetManager::AddFont(const std::string& assetId, const std::string& path,
                           const int fontSize) {
  const std::string fullPath = GetFullPath(path);
  TTF_Font* font = TTF_OpenFont(fullPath.c_str(), fontSize);
  fonts_.emplace(assetId, font);

  Logger::Info("Added font: " + assetId + " from path: " + fullPath);
}

TTF_Font* AssetManager::GetFont(const std::string& assetId) const {
  return fonts_.at(assetId);
}

std::string AssetManager::GetBasePath() const { return base_path_; }

std::string AssetManager::GetFullPath(const std::string& relativePath) const {
  const std::filesystem::path basePath = GetBasePath();
  const std::filesystem::path assetPath = basePath / relativePath;
  return assetPath.string();
}