#pragma once

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <cstdint>
#include <map>
#include <optional>
#include <string>

class GameConfig;

class AssetManager {
  std::string base_path_;
  std::map<std::string, SDL_Texture*> textures_;
  std::map<std::string, TTF_Font*> fonts_;
  std::map<std::string, MIX_Audio*> audio_clips_;
  std::optional<SDL_ScaleMode> default_scale_mode_;
  // Bumped whenever a texture is added or replaced. Sprite renderers compare against
  // their cached generation to know when to re-resolve a stale SDL_Texture* pointer.
  std::uint64_t texture_generation_{0};

 public:
  AssetManager() = default;

  AssetManager(const AssetManager&) = delete;
  AssetManager& operator=(const AssetManager&) = delete;

  AssetManager(AssetManager&&) noexcept = default;
  AssetManager& operator=(AssetManager&&) noexcept = default;

  ~AssetManager();

  void LoadGameConfig(const GameConfig& config);

  void ClearAssets();
  void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& path);
  [[nodiscard]] SDL_Texture* GetTexture(const std::string& assetId) const;
  void AddFont(const std::string& assetId, const std::string& path, float fontSize);
  [[nodiscard]] TTF_Font* GetFont(const std::string& assetId) const;
  void AddAudioClip(MIX_Mixer* mixer, const std::string& assetId, const std::string& path);
  [[nodiscard]] MIX_Audio* GetAudioClip(const std::string& assetId) const;
  [[nodiscard]] std::string GetFullPath(const std::string& relativePath) const;
  void SetDefaultScaleMode(const std::string& scaleMode);
  [[nodiscard]] std::uint64_t TextureGeneration() const { return texture_generation_; }
};