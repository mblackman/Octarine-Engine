#pragma once

#include <map>
#include <optional>
#include <string>

#include "AssetManager/AssetMetadata.h"

namespace sol {
class state;
}
class GameConfig;

enum class AssetType { Texture, Font, Audio };

inline const char* toAssetTypeString(AssetType type) {
  switch (type) {
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
struct CatalogEntry {
  AssetType type{};
  std::string id;        // resolved unique id (filename stem, or meta.id; fonts append "-<size>")
  std::string fullPath;  // absolute path to the source file on disk
  // Texture
  std::optional<ScaleMode> scaleMode;
  std::optional<std::string> atlas;
  // Font — one entry per size
  float fontSize{0.0F};
  // Audio
  bool stream{false};
};

// Directory-discovery + sidecar-parsing catalog: maps asset id -> CatalogEntry. Loads nothing;
// it is the engine's index of what assets exist and how each should be loaded.
class AssetCatalog {
 public:
  // Scan the convention folders under `basePath`, parse `<file>.meta` sidecars via the borrowed
  // `lua` state, and populate the id -> entry map. Returns false if any id collision was detected.
  // The GameConfig overload pulls the project default scale mode from config; the explicit
  // overload takes it directly (used by tests, which avoids GameConfig's SDL-backed load path).
  bool Build(const std::string& basePath, sol::state& lua, const GameConfig& gameConfig);
  bool Build(const std::string& basePath, sol::state& lua, std::optional<ScaleMode> defaultScaleMode);

  [[nodiscard]] const CatalogEntry* Find(const std::string& id) const;
  [[nodiscard]] bool Contains(const std::string& id) const { return Find(id) != nullptr; }
  [[nodiscard]] const std::map<std::string, CatalogEntry>& Entries() const { return entries_; }
  [[nodiscard]] std::size_t Size() const { return entries_.size(); }
  void Clear() { entries_.clear(); }

  // Emit one Info line per catalog entry (id -> path). Verification aid during bring-up.
  void DumpToLog() const;

  // Install a global Lua table (default name "assets") mapping every catalog id to itself, with a
  // metatable that raises on an unknown key. Lets scripts write `assets.tank` and get an error on
  // a typo instead of silently passing a bad id string. Runtime half of the typed-id surface; the
  // EmmyLua stub for editor autocomplete lands in Stage 7.
  void InstallLuaIdTable(sol::state& lua, const std::string& globalName = "assets") const;

 private:
  std::map<std::string, CatalogEntry> entries_;
};
