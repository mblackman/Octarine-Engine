#include "AssetManager.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <filesystem>

#include "../Game/GameConfig.h"
#include "../General/Logger.h"

AssetManager::~AssetManager() { ClearAssets(); }

void AssetManager::ClearAssets() {
  for (const auto &[fst, snd] : textures_) {
    SDL_DestroyTexture(snd);
  }
  for (const auto &[fst, snd] : fonts_) {
    TTF_CloseFont(snd);
  }

  textures_.clear();
  fonts_.clear();
}

void AssetManager::AddTexture(SDL_Renderer *renderer, const std::string &assetId, const std::string &path) {
  const std::string fullPath = GetFullPath(path);

  SDL_Texture *texture = IMG_LoadTexture(renderer, fullPath.c_str());
  if (!texture) {
    Logger::Error("Failed to create texture: " + std::string(SDL_GetError()));
    return;
  }

  if (!default_scale_mode_.has_value() && GameConfig::GetInstance().GetDefaultScaleMode().has_value()) {
    SetDefaultScaleMode(GameConfig::GetInstance().GetDefaultScaleMode().value());
  }

  if (default_scale_mode_.has_value()) {
    SDL_SetTextureScaleMode(texture, default_scale_mode_.value());
  }

  textures_.emplace(assetId, texture);

  Logger::Info("Added texture: " + assetId + " from path: " + fullPath);
}

SDL_Texture *AssetManager::GetTexture(const std::string &assetId) const { return textures_.at(assetId); }

void AssetManager::AddFont(const std::string &assetId, const std::string &path, const float fontSize) {
  const std::string fullPath = GetFullPath(path);
  TTF_Font *font = TTF_OpenFont(fullPath.c_str(), fontSize);
  fonts_.emplace(assetId, font);

  Logger::Info("Added font: " + assetId + " from path: " + fullPath);
}

TTF_Font *AssetManager::GetFont(const std::string &assetId) const { return fonts_.at(assetId); }

std::string AssetManager::GetFullPath(const std::string &relativePath) const {
  const std::filesystem::path basePath = GameConfig::GetInstance().GetAssetPath();
  const std::filesystem::path assetPath = basePath / relativePath;
  return assetPath.string();
}

void AssetManager::SetDefaultScaleMode(const std::string &scaleMode) {
  if (scaleMode == "nearest") {
    default_scale_mode_ = SDL_SCALEMODE_NEAREST;
  } else if (scaleMode == "linear") {
    default_scale_mode_ = SDL_SCALEMODE_LINEAR;
  } else {
    Logger::Error("Invalid scale mode: " + scaleMode);
  }
}
