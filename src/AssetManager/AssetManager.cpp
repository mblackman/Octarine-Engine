#include "AssetManager.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <filesystem>
#include <ranges>

#include "../Game/GameConfig.h"
#include "../General/Logger.h"

AssetManager::~AssetManager() { ClearAssets(); }
void AssetManager::LoadGameConfig(const GameConfig &config) {
  base_path_ = config.GetAssetPath();
  if (config.GetDefaultScaleMode().has_value()) {
    SetDefaultScaleMode(config.GetDefaultScaleMode().value());
  }
}

void AssetManager::ClearAssets() {
  for (const auto &snd : textures_ | std::views::values) {
    SDL_DestroyTexture(snd);
  }
  for (const auto &snd : fonts_ | std::views::values) {
    TTF_CloseFont(snd);
  }
  for (const auto &clip : audio_clips_ | std::views::values) {
    MIX_DestroyAudio(clip);
  }

  textures_.clear();
  fonts_.clear();
  audio_clips_.clear();
}

void AssetManager::AddTexture(SDL_Renderer *renderer, const std::string &assetId, const std::string &path) {
  const std::string fullPath = GetFullPath(path);

  SDL_Texture *texture = IMG_LoadTexture(renderer, fullPath.c_str());
  if (!texture) {
    Logger::Error("Failed to create texture: " + std::string(SDL_GetError()));
    return;
  }

  if (default_scale_mode_.has_value()) {
    SDL_SetTextureScaleMode(texture, default_scale_mode_.value());
  }

  // Replace prior texture under the same id rather than leaking it.
  if (const auto it = textures_.find(assetId); it != textures_.end()) {
    Logger::Warn("Replacing existing texture: " + assetId);
    SDL_DestroyTexture(it->second);
    it->second = texture;
  } else {
    textures_.emplace(assetId, texture);
  }
  ++texture_generation_;

  Logger::Info("Added texture: " + assetId + " from path: " + fullPath);
}

SDL_Texture *AssetManager::GetTexture(const std::string &assetId) const {
  const auto it = textures_.find(assetId);
  if (it == textures_.end()) {
    Logger::Warn("Requested missing texture: " + assetId);
    return nullptr;
  }
  return it->second;
}

void AssetManager::AddFont(const std::string &assetId, const std::string &path, const float fontSize) {
  const std::string fullPath = GetFullPath(path);
  TTF_Font *font = TTF_OpenFont(fullPath.c_str(), fontSize);
  if (!font) {
    Logger::Error("Failed to load font " + assetId + " from " + fullPath + ": " + std::string(SDL_GetError()));
    return;
  }

  if (const auto it = fonts_.find(assetId); it != fonts_.end()) {
    Logger::Warn("Replacing existing font: " + assetId);
    TTF_CloseFont(it->second);
    it->second = font;
  } else {
    fonts_.emplace(assetId, font);
  }

  Logger::Info("Added font: " + assetId + " from path: " + fullPath);
}

void AssetManager::AddAudioClip(MIX_Mixer *mixer, const std::string &assetId, const std::string &path) {
  if (!mixer) {
    Logger::Error("AddAudioClip called with null mixer for: " + assetId);
    return;
  }

  const std::string fullPath = GetFullPath(path);
  MIX_Audio *clip = MIX_LoadAudio(mixer, fullPath.c_str(), true);
  if (!clip) {
    Logger::Error("Failed to load audio clip " + assetId + " from " + fullPath + ": " + std::string(SDL_GetError()));
    return;
  }

  if (const auto it = audio_clips_.find(assetId); it != audio_clips_.end()) {
    Logger::Warn("Replacing existing audio clip: " + assetId);
    MIX_DestroyAudio(it->second);
    it->second = clip;
  } else {
    audio_clips_.emplace(assetId, clip);
  }

  Logger::Info("Added audio clip: " + assetId + " from path: " + fullPath);
}

MIX_Audio *AssetManager::GetAudioClip(const std::string &assetId) const {
  const auto it = audio_clips_.find(assetId);
  if (it == audio_clips_.end()) {
    Logger::Warn("Requested missing audio clip: " + assetId);
    return nullptr;
  }
  return it->second;
}

TTF_Font *AssetManager::GetFont(const std::string &assetId) const {
  const auto it = fonts_.find(assetId);
  if (it == fonts_.end()) {
    Logger::Warn("Requested missing font: " + assetId);
    return nullptr;
  }
  return it->second;
}

std::string AssetManager::GetFullPath(const std::string &relativePath) const {
  const std::filesystem::path basePath = base_path_;
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
