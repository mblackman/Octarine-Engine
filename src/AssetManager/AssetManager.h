#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <map>
#include <optional>
#include <string>

class GameConfig;

class AssetManager {
private:
  std::map<std::string, SDL_Texture*> textures_;
  std::map<std::string, TTF_Font*> fonts_;
  std::optional<SDL_ScaleMode> default_scale_mode_;
  std::string base_path_;

public:
  AssetManager() = default;
  ~AssetManager();

  void SetGameConfig(const GameConfig& gameConfig);

  void ClearAssets();
  void AddTexture(SDL_Renderer* renderer, const std::string& assetId,
                  const std::string& path);
  [[nodiscard]] SDL_Texture* GetTexture(const std::string& assetId) const;
  void AddFont(const std::string& assetId, const std::string& path,
               int fontSize);
  [[nodiscard]] TTF_Font* GetFont(const std::string& assetId) const;
  [[nodiscard]] std::string GetBasePath() const;
  [[nodiscard]] std::string GetFullPath(const std::string& relativePath) const;
  void SetDefaultScaleMode(const std::string& scaleMode);
};