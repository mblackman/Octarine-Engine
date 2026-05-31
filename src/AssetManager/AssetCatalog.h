#pragma once

#include <SDL3/SDL_rect.h>

#include <map>
#include <optional>
#include <string>

#include "AssetManager/AssetMetadata.h"

namespace sol
{
    class state;
}

class GameConfig;

enum class AssetType { Texture, Font, Audio };

inline const char* toAssetTypeString(AssetType type)
{
    switch (type)
    {
    case AssetType::Texture:
        return "texture";
    case AssetType::Font:
        return "font";
    case AssetType::Audio:
        return "audio_clip";
    }
    return "unknown";
}

// One resolved, loadable asset. Produced by AssetCatalog::Build from a discovered file plus its
// (optional) sidecar metadata. Holds no SDL/MIX handle — actually loading the asset is
// AssetManager's job (it consults these entries via Acquire).
struct CatalogEntry
{
    AssetType type{};
    std::string id; // resolved unique id (filename stem, or meta.id; fonts append "-<size>")
    std::string fullPath; // absolute path to the source file on disk
    // Texture
    std::optional<ScaleMode> scaleMode;
    std::optional<std::string> atlas; // sidecar `meta.atlas = <group>` (raw group name)
    bool noAtlas{false};              // sidecar `meta.no_atlas = true` (escape hatch, see AssetMetadata.h)
    // Resolved atlas membership — populated by TextureAtlasBaker after the scan. When set, this entry
    // is rendered as a slice of `atlasId`'s texture instead of its own. Both fields move
    // through asset_manifest.lua so shipped builds skip the bake.
    std::optional<std::string> atlasId; // catalog id of the packed atlas texture (e.g. "__atlas_main")
    std::optional<SDL_FRect> atlasSlice; // pixel-rect of this asset inside the atlas
    // Font — one entry per size
    float fontSize{0.0F};
    // Audio
    bool stream{false};
};

// Directory-discovery + sidecar-parsing catalog: maps asset id -> CatalogEntry. Loads nothing;
// it is the engine's index of what assets exist and how each should be loaded.
class AssetCatalog
{
public:
    // Populate the id -> entry map for the project at `basePath`. When `allowManifest` is true and a
    // baked `asset_manifest.lua` is present at the root, load it and skip the filesystem walk — assets
    // resolve with no scan, which is what makes read-only APK/.app bundles work. Otherwise (dev build,
    // or no manifest) recursively scan the convention folders and parse `<file>.meta` sidecars via the
    // borrowed `lua` state. `allowManifest` is the dev-vs-shipped gate (OCTARINE_SHIPPED / the
    // `--use-manifest` dev override): when false the manifest probe is skipped entirely, so a stray
    // manifest beside dev assets can never hijack the scan. Returns false if any id collision (scan)
    // or malformed entry (manifest) was found. The GameConfig overload pulls the project default scale
    // mode from config; the explicit overload takes it directly (used by tests, which avoids
    // GameConfig's SDL-backed load path).
    bool Build(const std::string& basePath, sol::state& lua, const GameConfig& gameConfig, bool allowManifest = true);
    bool Build(const std::string& basePath, sol::state& lua, std::optional<ScaleMode> defaultScaleMode,
               bool allowManifest = true);

    // Force a live filesystem scan, ignoring any baked manifest at the root. `Build` delegates here
    // when no manifest is present; the bake step calls it directly so it always re-derives the
    // catalog from the real files (rather than round-tripping a stale manifest). Returns false on an
    // id collision. Sets IsFromManifest() to false.
    bool ScanFilesystem(const std::string& basePath, sol::state& lua, const GameConfig& gameConfig);
    bool ScanFilesystem(const std::string& basePath, sol::state& lua, std::optional<ScaleMode> defaultScaleMode);

    // Load a baked manifest directly (the branch `Build` takes when `asset_manifest.lua` exists).
    // Each entry's relative `file` is re-rooted against `basePath` so the stored `fullPath` resolves
    // the same way the directory scan's does. Exposed for tests (manifest-vs-scan parity); the
    // runtime reaches it through `Build`. Returns false on a missing/unparseable manifest or a
    // malformed entry. Sets IsFromManifest() to true on success.
    bool LoadManifest(const std::string& manifestPath, sol::state& lua, const std::string& basePath);

    // True when the catalog was populated from a baked manifest rather than a live filesystem scan.
    // Callers (e.g. AssetManager::Validate) use this to skip on-disk `stat` checks that cannot hold
    // inside a read-only bundle — the bake step already validated existence.
    [[nodiscard]] bool IsFromManifest() const { return from_manifest_; }

    [[nodiscard]] const CatalogEntry* Find(const std::string& id) const;
    [[nodiscard]] bool Contains(const std::string& id) const { return Find(id) != nullptr; }
    [[nodiscard]] const std::map<std::string, CatalogEntry>& Entries() const { return entries_; }
    [[nodiscard]] std::size_t Size() const { return entries_.size(); }
    void Clear() { entries_.clear(); }

    // Bake-time mutation surface. TextureAtlasBaker uses these to record packed-atlas membership and to
    // splice in the new atlas entries themselves. Not for runtime use.
    void InsertOrReplace(CatalogEntry entry);
    void SetAtlasMembership(const std::string& memberId, std::string atlasId, SDL_FRect atlasSlice);

    // Emit one Info line per catalog entry (id -> path). Verification aid during bring-up.
    void DumpToLog() const;

    // Serialize the catalog to a `dofile`-able Lua manifest at `outPath`:
    // `return { [id] = { type, file, ...metadata } }`, with `file` made relative to `basePath` and
    // written with forward slashes so it stays portable and Lua-safe. The bake step emits this so a
    // shipped build can resolve assets without walking the filesystem. Returns false if the output
    // cannot be written.
    [[nodiscard]] bool WriteManifest(const std::string& outPath, const std::string& basePath) const;

    // Install a global Lua table (default name "assets") mapping every catalog id to itself, with a
    // metatable that raises on an unknown key. Lets scripts write `assets.tank` and get an error on
    // a typo instead of silently passing a bad id string.
    void InstallLuaIdTable(sol::state& lua, const std::string& globalName = "assets") const;

private:
    std::map<std::string, CatalogEntry> entries_;
    bool from_manifest_{false};
};
