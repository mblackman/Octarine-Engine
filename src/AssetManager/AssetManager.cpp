#include "AssetManager.h"

#include <SDL3/SDL.h>

#include <filesystem>
#include <set>

#include "AssetManager/AssetPak.h"
#include "Game/GameConfig.h"
#include "General/Logger.h"

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
  texture_store_.Clear();
  font_store_.Clear();
  audio_store_.Clear();
  refcounter_.Clear();
}

// Recurses into Acquire(atlasId) when an atlas member is requested (one hop — atlases do not nest).
// NOLINTNEXTLINE(misc-no-recursion)
bool AssetManager::Acquire(const std::string &assetId, SDL_Renderer *renderer, MIX_Mixer *mixer) {
  // Existing reference — just bump the count, no reload.
  if (refcounter_.Has(assetId)) {
    refcounter_.Increment(assetId);
    return true;
  }

  // Already resident with no refcount (legacy load_asset path) — adopt at count 1.
  if (texture_store_.Contains(assetId) || font_store_.Contains(assetId) || audio_store_.Contains(assetId)) {
    refcounter_.Adopt(assetId);
    return true;
  }

  const CatalogEntry *entry = catalog_.Find(assetId);
  if (!entry) {
    Logger::Error("AssetManager::Acquire: id not found in catalog: " + assetId);
    return false;
  }

  // Atlas member: piggy-back on the backing atlas's SDL_Texture* rather than loading the
  // member's source bytes. The atlas gains one refcount per acquired member; releasing the
  // member drops that refcount back. The member id never appears in the texture store, so
  // GetTexture walks the catalog atlas_id -> texture store on lookup.
  if (entry->type == AssetType::Texture && entry->atlasId.has_value()) {
    if (!Acquire(*entry->atlasId, renderer, mixer)) {
      Logger::Error("AssetManager::Acquire: failed to acquire atlas '" + *entry->atlasId + "' for member '" + assetId +
                    "'");
      return false;
    }
    refcounter_.Adopt(assetId);
    return true;
  }

  const bool loaded = LoadFromCatalog(*entry, assetId, renderer, mixer);
  if (loaded) refcounter_.Adopt(assetId);
  return loaded;
}

bool AssetManager::LoadFromCatalog(const CatalogEntry &entry, const std::string &assetId, SDL_Renderer *renderer,
                                   MIX_Mixer *mixer) {
  switch (entry.type) {
    case AssetType::Texture: {
      AddTexture(renderer, assetId, entry.fullPath);
      // Per-asset scale mode from the catalog overrides the project default applied in AddTexture.
      if (entry.scaleMode.has_value()) {
        if (SDL_Texture *texture = texture_store_.Get(assetId)) {
          SDL_SetTextureScaleMode(texture,
                                  *entry.scaleMode == ScaleMode::Linear ? SDL_SCALEMODE_LINEAR : SDL_SCALEMODE_NEAREST);
        }
      }
      return texture_store_.Contains(assetId);
    }
    case AssetType::Font:
      AddFont(assetId, entry.fullPath, entry.fontSize);
      return font_store_.Contains(assetId);
    case AssetType::Audio:
      if (!mixer) {
        // Audio stack disabled (no mixer was Set — AudioSystem::Init failed at startup and already
        // flipped audioEnabled off). Skip silently so we don't spam per-clip errors on platforms
        // without an audio device (Android emulator, headless CI). Debug-level for dev diagnostics.
        Logger::Info("AssetManager::Acquire: audio disabled, skipping clip: " + assetId);
        return false;
      }
      AddAudioClip(mixer, assetId, entry.fullPath, entry.stream);
      return audio_store_.Contains(assetId);
  }
  return false;
}

// Mutually recursive with UnloadAsset: releasing an atlas member drops its stake in the atlas's
// refcount, which can reach zero and unload it.
// NOLINTNEXTLINE(misc-no-recursion)
void AssetManager::Release(const std::string &assetId) {
  const int remaining = refcounter_.Decrement(assetId);
  if (remaining != 0) return;  // -1 == never acquired / untracked; > 0 == still referenced elsewhere
  UnloadAsset(assetId);
}

void AssetManager::ReleaseAll(const std::vector<std::string> &assetIds) {
  for (const auto &id : assetIds) Release(id);
}

int AssetManager::RefCount(const std::string &assetId) const { return refcounter_.Count(assetId); }

// Mutually recursive with Release: an atlas member's unload path calls Release(atlasId).
// NOLINTNEXTLINE(misc-no-recursion)
void AssetManager::UnloadAsset(const std::string &assetId) {
  // Atlas-member alias: shares the atlas's SDL_Texture* via catalog redirect, not via a texture
  // store entry. Drop the member's stake in the atlas's refcount and we're done — the SDL handle
  // stays alive until the last member (or the atlas itself) releases.
  if (const CatalogEntry *e = catalog_.Find(assetId); e != nullptr && e->atlasId.has_value()) {
    Release(*e->atlasId);
    Logger::Info("Unloaded atlas member: " + assetId);
    return;
  }
  if (texture_store_.Remove(assetId)) {
    Logger::Info("Unloaded texture: " + assetId);
    return;
  }
  if (font_store_.Remove(assetId)) {
    Logger::Info("Unloaded font: " + assetId);
    return;
  }
  if (audio_store_.Remove(assetId)) {
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
  texture_store_.Add(renderer, assetId, io, fullPath);
}

SDL_Texture *AssetManager::GetTexture(const std::string &assetId) const {
  if (SDL_Texture *texture = texture_store_.Get(assetId)) {
    return texture;
  }
  // Atlas members are never inserted into the texture store directly — resolve the catalog redirect
  // to the backing atlas's SDL_Texture*. One-hop only (atlases do not nest).
  if (const CatalogEntry *e = catalog_.Find(assetId); e != nullptr && e->atlasId.has_value()) {
    return texture_store_.Get(*e->atlasId);
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
  font_store_.Add(assetId, io, fontSize, base_path_);
}

const GlyphAtlas *AssetManager::GetGlyphAtlas(const std::string &fontAssetId) const {
  return font_store_.GetGlyphAtlas(fontAssetId);
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
  audio_store_.Add(mixer, assetId, io, stream, fullPath);
}

MIX_Audio *AssetManager::GetAudioClip(const std::string &assetId) const { return audio_store_.Get(assetId); }

TTF_Font *AssetManager::GetFont(const std::string &assetId) const { return font_store_.Get(assetId); }

std::string AssetManager::GetFullPath(const std::string &relativePath) const {
  const std::filesystem::path basePath = base_path_;
  const std::filesystem::path assetPath = basePath / relativePath;
  return assetPath.string();
}

void AssetManager::SetDefaultScaleMode(const std::string &scaleMode) {
  if (scaleMode == "nearest") {
    texture_store_.SetDefaultScaleMode(SDL_SCALEMODE_NEAREST);
  } else if (scaleMode == "linear") {
    texture_store_.SetDefaultScaleMode(SDL_SCALEMODE_LINEAR);
  } else {
    Logger::Error("Invalid scale mode: " + scaleMode);
  }
}
