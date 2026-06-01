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
#include "AssetManager/AssetPak.h"
#include "AssetManager/GlyphAtlas.h"

AssetManager::~AssetManager() { ClearAssets(); }

SDL_IOStream *AssetManager::OpenAssetIO(const std::string &fullPath) const {
  if (asset_pak_ != nullptr && asset_pak_->IsOpen() && !base_path_.empty()) {
    std::error_code ec;
    const std::filesystem::path rel =
        std::filesystem::relative(std::filesystem::path(fullPath), std::filesystem::path(base_path_), ec);
    if (!ec && !rel.empty()) {
      if (SDL_IOStream *io = asset_pak_->OpenIO(rel.generic_string()); io != nullptr) {
        return io;
      }
    }
  }
  return SDL_IOFromFile(fullPath.c_str(), "rb");
}
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

  // Atlas member: piggy-back on the backing atlas's SDL_Texture* rather than loading the
  // member's source bytes. The atlas gains one refcount per acquired member; releasing the
  // member drops that refcount back. The member id never appears in textures_, so GetTexture
  // walks the catalog atlas_id -> textures_[atlas_id] on lookup.
  if (entry->type == AssetType::Texture && entry->atlasId.has_value()) {
    if (!Acquire(*entry->atlasId, renderer, mixer)) {
      Logger::Error("AssetManager::Acquire: failed to acquire atlas '" + *entry->atlasId + "' for member '" + assetId +
                    "'");
      return false;
    }
    refcounts_[assetId] = 1;
    return true;
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
          SDL_SetTextureScaleMode(texture,
                                  *entry.scaleMode == ScaleMode::Linear ? SDL_SCALEMODE_LINEAR : SDL_SCALEMODE_NEAREST);
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
      AddAudioClip(mixer, assetId, entry.fullPath, entry.stream);
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
  // Atlas-member alias: shares the atlas's SDL_Texture* via catalog redirect, not via a
  // textures_ entry. Drop the member's stake in the atlas's refcount and we're done — the
  // SDL handle stays alive until the last member (or the atlas itself) releases.
  if (const CatalogEntry *e = catalog_.Find(assetId); e != nullptr && e->atlasId.has_value()) {
    Release(*e->atlasId);
    Logger::Info("Unloaded atlas member: " + assetId);
    return;
  }
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

  // Route through OpenAssetIO so the same path resolves on desktop, an APK asset root, a .app
  // bundle, or — for shipped builds — the OCPK asset bundle.
  SDL_IOStream *io = OpenAssetIO(fullPath);
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
  if (const auto it = textures_.find(assetId); it != textures_.end()) {
    return it->second;
  }
  // Atlas members are never inserted into textures_ directly — resolve the catalog redirect
  // to the backing atlas's SDL_Texture*. One-hop only (atlases do not nest).
  if (const CatalogEntry *e = catalog_.Find(assetId); e != nullptr && e->atlasId.has_value()) {
    if (const auto ait = textures_.find(*e->atlasId); ait != textures_.end()) {
      return ait->second;
    }
  }
  // No per-frame warning: scene-load validation (AssetManager::Validate) is the authoritative
  // miss check. Renderers tolerate a null texture.
  return nullptr;
}

std::optional<SDL_FRect> AssetManager::GetAtlasSlice(const std::string &assetId) const {
  if (const CatalogEntry *e = catalog_.Find(assetId); e != nullptr) {
    return e->atlasSlice;
  }
  return std::nullopt;
}

void AssetManager::AddFont(const std::string &assetId, const std::string &path, const float fontSize) {
  const std::string fullPath = GetFullPath(path);
  SDL_IOStream *io = OpenAssetIO(fullPath);
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

  // Probe for a glyph-atlas sidecar pair (Stage 14 B3). The bake step writes them under
  // <basePath>/atlases/<asset_id>.atlas.{png,lua}; presence is the opt-in. SDL_IOFromFile is used
  // for the existence check so the probe transparently resolves through a shipped pak too.
  if (base_path_.empty()) return;
  const std::filesystem::path atlasPng = std::filesystem::path(base_path_) / "atlases" / (assetId + ".atlas.png");
  const std::filesystem::path atlasLua = std::filesystem::path(base_path_) / "atlases" / (assetId + ".atlas.lua");
  SDL_IOStream *probe = SDL_IOFromFile(atlasPng.string().c_str(), "rb");
  if (probe == nullptr) return;
  SDL_CloseIO(probe);
  auto atlas = std::make_unique<GlyphAtlas>();
  if (atlas->Load(atlasPng.string(), atlasLua.string())) {
    glyph_atlases_[assetId] = std::move(atlas);
  }
}

const GlyphAtlas *AssetManager::GetGlyphAtlas(const std::string &fontAssetId) const {
  const auto it = glyph_atlases_.find(fontAssetId);
  return it == glyph_atlases_.end() ? nullptr : it->second.get();
}

void AssetManager::AddAudioClip(MIX_Mixer *mixer, const std::string &assetId, const std::string &path,
                                const bool stream) {
  if (!mixer) {
    Logger::Error("AddAudioClip called with null mixer for: " + assetId);
    return;
  }

  const std::string fullPath = GetFullPath(path);
  SDL_IOStream *io = OpenAssetIO(fullPath);
  if (!io) {
    Logger::Error("Failed to open audio file " + assetId + " from " + fullPath + ": " + std::string(SDL_GetError()));
    return;
  }
  // predecode=true is the default for short SFX — full PCM in RAM at load time, lowest play-call
  // latency. predecode=false (meta.stream=true on the source) keeps the file compressed and
  // decodes on demand; pays back as flat memory for long-form music + ambient beds.
  MIX_Audio *clip = MIX_LoadAudio_IO(mixer, io, !stream, true);  // closes the stream regardless
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
