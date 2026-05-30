#pragma once

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "AssetManager/AssetCatalog.h"
#include "AssetManager/AssetReference.h"

class AssetPak;
class GameConfig;

class AssetManager {
  std::string base_path_;
  std::map<std::string, SDL_Texture*> textures_;
  std::map<std::string, TTF_Font*> fonts_;
  std::map<std::string, MIX_Audio*> audio_clips_;
  std::optional<SDL_ScaleMode> default_scale_mode_;
  // Index of every discoverable asset (id -> file + metadata). Loads nothing on its own;
  // Acquire consults it to load on demand.
  AssetCatalog catalog_;
  // Per-id acquire count. The 0 -> 1 transition loads the underlying handle; the N -> 0
  // transition unloads it. Assets loaded via the legacy load_asset path are adopted at refcount 1
  // on first Acquire. Entries at zero are erased.
  std::map<std::string, int> refcounts_;
  // Bumped whenever a texture is added or replaced. Sprite renderers compare against
  // their cached generation to know when to re-resolve a stale SDL_Texture* pointer.
  std::uint64_t texture_generation_{0};
  // Optional shipped-bundle archive (Stage 14 / B4). When set, AssetManager prefers reading each
  // asset's bytes from the pak over the loose file at `fullPath`. Non-owning — the Registry owns
  // the AssetPak instance.
  const AssetPak* asset_pak_{nullptr};

 public:
  AssetManager() = default;

  AssetManager(const AssetManager&) = delete;
  AssetManager& operator=(const AssetManager&) = delete;

  AssetManager(AssetManager&&) noexcept = default;
  AssetManager& operator=(AssetManager&&) noexcept = default;

  ~AssetManager();

  [[nodiscard]] std::string GetBasePath() const { return base_path_; }
  void LoadGameConfig(const GameConfig& config);

  void ClearAssets();

  // Load a single catalog asset by id if it is not already resident. Returns true when the asset
  // is available afterwards (already loaded, or freshly loaded), false when the id is unknown to
  // the catalog or the underlying load failed. `renderer`/`mixer` feed the texture/audio loaders.
  // Uses refcounting to manage the underlying handle.
  bool Acquire(const std::string& assetId, SDL_Renderer* renderer, MIX_Mixer* mixer);

  // Acquire the deduped set of referenced ids. Returns the count successfully acquired.
  int AcquireAll(const std::vector<AssetReference>& refs, SDL_Renderer* renderer, MIX_Mixer* mixer);

  // Drop one reference to an asset id. When the count reaches zero the underlying SDL/MIX handle
  // is destroyed and the entry erased; unknown/untracked ids are ignored.
  void Release(const std::string& assetId);
  void ReleaseAll(const std::vector<std::string>& assetIds);

  // Validate scene references against the catalog: an id missing from the catalog, or present but
  // whose backing file no longer exists on disk, is logged once with the referencing context.
  // Returns the number of failures (0 == clean). This is the authoritative miss check, replacing
  // the per-frame warnings the Get* accessors used to emit at draw time.
  [[nodiscard]] int Validate(const std::vector<AssetReference>& refs) const;

  void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& path);
  [[nodiscard]] SDL_Texture* GetTexture(const std::string& assetId) const;
  void AddFont(const std::string& assetId, const std::string& path, float fontSize);
  [[nodiscard]] TTF_Font* GetFont(const std::string& assetId) const;
  // `stream` plumbs the catalog's `meta.stream` flag into SDL_mixer's predecode toggle: false for
  // SFX (eager decode = predictable play latency), true for long music + ambient beds (lazy decode
  // = decode-as-played, keeps the loading footprint flat at the cost of a touch more per-play cost).
  void AddAudioClip(MIX_Mixer* mixer, const std::string& assetId, const std::string& path,
                    bool stream = false);
  [[nodiscard]] MIX_Audio* GetAudioClip(const std::string& assetId) const;
  [[nodiscard]] std::string GetFullPath(const std::string& relativePath) const;
  void SetDefaultScaleMode(const std::string& scaleMode);
  [[nodiscard]] std::uint64_t TextureGeneration() const { return texture_generation_; }

  [[nodiscard]] AssetCatalog& GetCatalog() { return catalog_; }
  [[nodiscard]] const AssetCatalog& GetCatalog() const { return catalog_; }

  // Wire in an opened AssetPak (typically by Game::Setup on shipped builds, after probing for an
  // asset_bundle.pak beside the project). Asset load sites prefer the pak when set; absent it,
  // they fall through to SDL_IOFromFile on the loose tree. Pointer is non-owning; the Registry
  // owns the AssetPak instance.
  void SetAssetPak(const AssetPak* pak) { asset_pak_ = pak; }
  [[nodiscard]] const AssetPak* GetAssetPak() const { return asset_pak_; }

  // Current acquire count for an id (0 if untracked). Test/diagnostic aid.
  [[nodiscard]] int RefCount(const std::string& assetId) const;

  [[nodiscard]] const std::map<std::string, SDL_Texture*>& GetTextures() const { return textures_; }
  [[nodiscard]] const std::map<std::string, TTF_Font*>& GetFonts() const { return fonts_; }
  [[nodiscard]] const std::map<std::string, MIX_Audio*>& GetAudioClips() const { return audio_clips_; }

 private:
  // Open a read-only SDL_IOStream over `fullPath`. Tries the wired AssetPak first (lookup by
  // project-relative key) and falls back to SDL_IOFromFile for dev builds + bootstrap projects
  // where no pak has been baked yet.
  [[nodiscard]] SDL_IOStream* OpenAssetIO(const std::string& fullPath) const;

  // Perform the actual SDL/MIX load for a catalog entry (no refcount bookkeeping). Returns whether
  // the handle is resident afterwards.
  bool LoadFromCatalog(const CatalogEntry& entry, const std::string& assetId, SDL_Renderer* renderer,
                       MIX_Mixer* mixer);

  // Destroy and erase whichever resident handle backs `assetId` (texture/font/audio). Bumps
  // texture_generation_ when a texture is dropped so cached SDL_Texture* pointers re-resolve.
  void UnloadAsset(const std::string& assetId);
};