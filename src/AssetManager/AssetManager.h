#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <map>
#include <optional>
#include <string>

class GameConfig;

class AssetManager {
  std::string base_path_;
  std::map<std::string, SDL_Texture*> textures_;
  std::map<std::string, TTF_Font*> fonts_;
  std::optional<SDL_ScaleMode> default_scale_mode_;

 public:
  AssetManager() = default;

  AssetManager(const AssetManager&) = default;
  AssetManager& operator=(const AssetManager&) = default;

  AssetManager(AssetManager&&) noexcept = default;
  AssetManager& operator=(AssetManager&&) = default;

  ~AssetManager();

  void LoadGameConfig(const GameConfig& config);

  void ClearAssets();
  void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& path);
  [[nodiscard]] SDL_Texture* GetTexture(const std::string& assetId) const;
  void AddFont(const std::string& assetId, const std::string& path, float fontSize);
  [[nodiscard]] TTF_Font* GetFont(const std::string& assetId) const;
  [[nodiscard]] std::string GetFullPath(const std::string& relativePath) const;
  void SetDefaultScaleMode(const std::string& scaleMode);
};