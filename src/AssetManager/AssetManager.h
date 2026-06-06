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
#include "AssetManager/AssetRefcounter.h"
#include "AssetManager/AssetReference.h"
#include "AssetManager/AudioClipStore.h"
#include "AssetManager/FontStore.h"
#include "AssetManager/TextureStore.h"

class AssetPak;
class GameConfig;
class GlyphAtlas;

// Composes the asset subsystem: the AssetCatalog (what exists + how to load it), three typed handle
// stores (TextureStore / FontStore / AudioClipStore — the resident SDL/TTF/MIX handles), and an
// AssetRefcounter (acquire counts). AssetManager itself owns only the cross-cutting concerns the
// pieces can't: the project base path, the optional shipped-bundle pak, IO resolution, and the
// Acquire/Release orchestration that ties refcounts to catalog-driven loads. Public API is
// unchanged from the pre-split monolith — consumers see the same surface.
class AssetManager {
  std::string base_path_;
  // Index of every discoverable asset (id -> file + metadata). Loads nothing on its own; Acquire
  // consults it to load on demand.
  AssetCatalog catalog_;
  TextureStore texture_store_;
  FontStore font_store_;
  AudioClipStore audio_store_;
  // Per-id acquire count. The 0 -> 1 transition loads the underlying handle; the N -> 0 transition
  // unloads it. Assets loaded via the legacy load_asset path are adopted at refcount 1 on first
  // Acquire.
  AssetRefcounter refcounter_;
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

  // Force a resident asset to reload from disk (dev hot-push). When `assetId` is currently
  // resident its handle is destroyed and re-loaded from the catalog, preserving the acquire
  // count so existing references keep working (the texture store bumps its generation so cached
  // SDL_Texture* re-resolve). No-op (returns false) for unknown or non-resident ids. Dev-only:
  // editor-driven content push, never on shipped builds.
  bool Reload(const std::string& assetId, SDL_Renderer* renderer, MIX_Mixer* mixer);

  // Reload every resident asset whose catalog source file is `absPath` (an absolute path). The
  // hot-push path resolves a pushed project-relative file to its absolute form, writes the bytes,
  // then calls this to refresh whatever ids that file backs. Returns the number of ids reloaded.
  int ReloadByPath(const std::string& absPath, SDL_Renderer* renderer, MIX_Mixer* mixer);

  // Absolute source-file paths of every currently-resident asset (textures, fonts, audio clips),
  // deduped. The local asset hot-reload watcher (AssetHotReload) watches exactly this set and
  // calls ReloadByPath when one changes on disk. Paths come from the catalog entry behind each
  // resident id; ids with no catalog entry (or no on-disk source) are skipped. Dev-only use.
  [[nodiscard]] std::vector<std::string> ResidentSourcePaths() const;

  // Validate scene references against the catalog: an id missing from the catalog, or present but
  // whose backing file no longer exists on disk, is logged once with the referencing context.
  // Returns the number of failures (0 == clean). This is the authoritative miss check, replacing
  // the per-frame warnings the Get* accessors used to emit at draw time.
  [[nodiscard]] int Validate(const std::vector<AssetReference>& refs) const;

  void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& path);
  // Resolves to the SDL_Texture under `assetId`. For atlas-member ids this walks the catalog to
  // the backing atlas (whose SDL_Texture* lives in the texture store) — members themselves never
  // appear in the store, which is what lets one atlas keep one SDL handle even with N member ids.
  [[nodiscard]] SDL_Texture* GetTexture(const std::string& assetId) const;
  // Pixel-rect of `assetId` within its atlas, when `assetId` is an atlas member. Nullopt for
  // loose textures and non-textures. Sprite render adds slice.x/y to the sprite's logical
  // src_rect to compose the final source rect into the atlas.
  [[nodiscard]] std::optional<SDL_FRect> GetAtlasSlice(const std::string& assetId) const;
  void AddFont(const std::string& assetId, const std::string& path, float fontSize);
  [[nodiscard]] TTF_Font* GetFont(const std::string& assetId) const;
  // `stream` plumbs the catalog's `meta.stream` flag into SDL_mixer's predecode toggle: false for
  // SFX (eager decode = predictable play latency), true for long music + ambient beds (lazy decode
  // = decode-as-played, keeps the loading footprint flat at the cost of a touch more per-play cost).
  void AddAudioClip(MIX_Mixer* mixer, const std::string& assetId, const std::string& path, bool stream = false);
  [[nodiscard]] MIX_Audio* GetAudioClip(const std::string& assetId) const;
  [[nodiscard]] std::string GetFullPath(const std::string& relativePath) const;
  void SetDefaultScaleMode(const std::string& scaleMode);
  [[nodiscard]] std::uint64_t TextureGeneration() const { return texture_store_.Generation(); }

  [[nodiscard]] AssetCatalog& GetCatalog() { return catalog_; }
  [[nodiscard]] const AssetCatalog& GetCatalog() const { return catalog_; }

  // Wire in an opened AssetPak (typically by Game::Setup on shipped builds, after probing for an
  // asset_bundle.pak beside the project). Asset load sites prefer the pak when set; absent it,
  // they fall through to SDL_IOFromFile on the loose tree. Pointer is non-owning; the Registry
  // owns the AssetPak instance.
  void SetAssetPak(const AssetPak* pak) { asset_pak_ = pak; }
  [[nodiscard]] const AssetPak* GetAssetPak() const { return asset_pak_; }

  // Resident glyph atlas for the given font catalog id, or nullptr if none has been loaded.
  // RenderTextSystem consults this before falling back to TTF_RenderText_Blended.
  [[nodiscard]] const GlyphAtlas* GetGlyphAtlas(const std::string& fontAssetId) const;

  // Current acquire count for an id (0 if untracked). Test/diagnostic aid.
  [[nodiscard]] int RefCount(const std::string& assetId) const;

  [[nodiscard]] const std::map<std::string, SDL_Texture*>& GetTextures() const { return texture_store_.All(); }
  [[nodiscard]] const std::map<std::string, TTF_Font*>& GetFonts() const { return font_store_.All(); }
  [[nodiscard]] const std::map<std::string, MIX_Audio*>& GetAudioClips() const { return audio_store_.All(); }

 private:
  // Open a read-only SDL_IOStream over `fullPath`. Tries the wired AssetPak first (lookup by
  // project-relative key) and falls back to SDL_IOFromFile for dev builds + bootstrap projects
  // where no pak has been baked yet.
  [[nodiscard]] SDL_IOStream* OpenAssetIO(const std::string& fullPath) const;

  // Perform the actual SDL/MIX load for a catalog entry (no refcount bookkeeping). Returns whether
  // the handle is resident afterwards.
  bool LoadFromCatalog(const CatalogEntry& entry, const std::string& assetId, SDL_Renderer* renderer, MIX_Mixer* mixer);

  // Destroy and erase whichever resident handle backs `assetId` (texture/font/audio). The texture
  // store bumps its generation when a texture is dropped so cached SDL_Texture* pointers re-resolve.
  void UnloadAsset(const std::string& assetId);
};
