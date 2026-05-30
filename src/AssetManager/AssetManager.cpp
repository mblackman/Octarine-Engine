#include "AssetManager.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <filesystem>
#include <ranges>
#include <set>

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
  refcounts_.clear();
}

bool AssetManager::Acquire(const std::string &assetId, SDL_Renderer *renderer, MIX_Mixer *mixer) {
  // Existing reference — just bump the count, no reload.
  if (const auto it = refcounts_.find(assetId); it != refcounts_.end() && it->second > 0) {
    ++it->second;
    return true;
  }

  // Already resident with no refcount (legacy load_asset path) — adopt at count 1.
  if (textures_.contains(assetId) || fonts_.contains(assetId) || audio_clips_.contains(assetId)) {
    refcounts_[assetId] = 1;
    return true;
  }

  const CatalogEntry *entry = catalog_.Find(assetId);
  if (!entry) {
    Logger::Error("AssetManager::Acquire: id not found in catalog: " + assetId);
    return false;
  }

  const bool loaded = LoadFromCatalog(*entry, assetId, renderer, mixer);
  if (loaded) refcounts_[assetId] = 1;
  return loaded;
}

bool AssetManager::LoadFromCatalog(const CatalogEntry &entry, const std::string &assetId, SDL_Renderer *renderer,
                                   MIX_Mixer *mixer) {
  switch (entry.type) {
    case AssetType::Texture: {
      AddTexture(renderer, assetId, entry.fullPath);
      // Per-asset scale mode from the catalog overrides the project default applied in AddTexture.
      if (entry.scaleMode.has_value()) {
        if (SDL_Texture *texture = textures_.contains(assetId) ? textures_.at(assetId) : nullptr) {
          SDL_SetTextureScaleMode(
              texture, *entry.scaleMode == ScaleMode::Linear ? SDL_SCALEMODE_LINEAR : SDL_SCALEMODE_NEAREST);
        }
      }
      return textures_.contains(assetId);
    }
    case AssetType::Font:
      AddFont(assetId, entry.fullPath, entry.fontSize);
      return fonts_.contains(assetId);
    case AssetType::Audio:
      if (!mixer) {
        // Audio stack disabled (no mixer was Set — AudioSystem::Init failed at startup and already
        // flipped audioEnabled off). Skip silently so we don't spam per-clip errors on platforms
        // without an audio device (Android emulator, headless CI). Debug-level for dev diagnostics.
        Logger::Info("AssetManager::Acquire: audio disabled, skipping clip: " + assetId);
        return false;
      }
      AddAudioClip(mixer, assetId, entry.fullPath);
      return audio_clips_.contains(assetId);
  }
  return false;
}

void AssetManager::Release(const std::string &assetId) {
  const auto it = refcounts_.find(assetId);
  if (it == refcounts_.end()) return;  // never acquired / untracked
  if (--it->second > 0) return;        // still referenced elsewhere
  refcounts_.erase(it);
  UnloadAsset(assetId);
}

void AssetManager::ReleaseAll(const std::vector<std::string> &assetIds) {
  for (const auto &id : assetIds) Release(id);
}

int AssetManager::RefCount(const std::string &assetId) const {
  const auto it = refcounts_.find(assetId);
  return it == refcounts_.end() ? 0 : it->second;
}

void AssetManager::UnloadAsset(const std::string &assetId) {
  if (const auto it = textures_.find(assetId); it != textures_.end()) {
    SDL_DestroyTexture(it->second);
    textures_.erase(it);
    ++texture_generation_;  // invalidate cached SDL_Texture* in sprite renderers
    Logger::Info("Unloaded texture: " + assetId);
    return;
  }
  if (const auto it = fonts_.find(assetId); it != fonts_.end()) {
    TTF_CloseFont(it->second);
    fonts_.erase(it);
    Logger::Info("Unloaded font: " + assetId);
    return;
  }
  if (const auto it = audio_clips_.find(assetId); it != audio_clips_.end()) {
    MIX_DestroyAudio(it->second);
    audio_clips_.erase(it);
    Logger::Info("Unloaded audio clip: " + assetId);
  }
}

int AssetManager::AcquireAll(const std::vector<AssetReference> &refs, SDL_Renderer *renderer, MIX_Mixer *mixer) {
  std::set<std::string> seen;
  int acquired = 0;
  for (const auto &ref : refs) {
    if (!seen.insert(ref.id).second) continue;  // dedupe repeated references
    if (Acquire(ref.id, renderer, mixer)) ++acquired;
  }
  return acquired;
}

int AssetManager::Validate(const std::vector<AssetReference> &refs) const {
  // On a baked-manifest build the entry paths are relative to a read-only bundle (APK/.app) with no
  // real filesystem to stat — the bake step already validated existence, so trust the catalog and
  // only check membership. The on-disk check still runs for live (directory-scan) dev builds.
  const bool checkOnDisk = !catalog_.IsFromManifest();
  int failures = 0;
  for (const auto &ref : refs) {
    const CatalogEntry *entry = catalog_.Find(ref.id);
    if (!entry) {
      Logger::Error("Asset validation: id '" + ref.id + "' (referenced by " + ref.context +
                    ") is not in the asset catalog.");
      ++failures;
      continue;
    }
    if (!checkOnDisk) continue;
    std::error_code ec;
    if (!std::filesystem::exists(entry->fullPath, ec)) {
      Logger::Error("Asset validation: id '" + ref.id + "' (referenced by " + ref.context +
                    ") maps to a missing file: " + entry->fullPath);
      ++failures;
    }
  }
  return failures;
}

void AssetManager::AddTexture(SDL_Renderer *renderer, const std::string &assetId, const std::string &path) {
  const std::string fullPath = GetFullPath(path);

  // Route through SDL IO so the same path resolves on desktop, an APK asset root, or a .app bundle.
  SDL_IOStream *io = SDL_IOFromFile(fullPath.c_str(), "rb");
  if (!io) {
    Logger::Error("Failed to open texture file " + fullPath + ": " + std::string(SDL_GetError()));
    return;
  }
  SDL_Texture *texture = IMG_LoadTexture_IO(renderer, io, true);  // closes the stream
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
    // No per-frame warning: scene-load validation (AssetManager::Validate) is the authoritative
    // miss check. Renderers tolerate a null texture.
    return nullptr;
  }
  return it->second;
}

void AssetManager::AddFont(const std::string &assetId, const std::string &path, const float fontSize) {
  const std::string fullPath = GetFullPath(path);
  SDL_IOStream *io = SDL_IOFromFile(fullPath.c_str(), "rb");
  if (!io) {
    Logger::Error("Failed to open font file " + assetId + " from " + fullPath + ": " + std::string(SDL_GetError()));
    return;
  }
  TTF_Font *font = TTF_OpenFontIO(io, true, fontSize);  // closes the stream
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
  SDL_IOStream *io = SDL_IOFromFile(fullPath.c_str(), "rb");
  if (!io) {
    Logger::Error("Failed to open audio file " + assetId + " from " + fullPath + ": " + std::string(SDL_GetError()));
    return;
  }
  MIX_Audio *clip = MIX_LoadAudio_IO(mixer, io, true, true);  // predecode + closes the stream
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
    return nullptr;  // see GetTexture: validation owns the miss check.
  }
  return it->second;
}

TTF_Font *AssetManager::GetFont(const std::string &assetId) const {
  const auto it = fonts_.find(assetId);
  if (it == fonts_.end()) {
    return nullptr;  // see GetTexture: validation owns the miss check.
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
