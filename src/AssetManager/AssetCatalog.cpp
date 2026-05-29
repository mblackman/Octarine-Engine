#include "AssetManager/AssetCatalog.h"

#include <sol/sol.hpp>

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <optional>
#include <string>

#include "Game/GameConfig.h"
#include "General/Logger.h"

namespace {

namespace fs = std::filesystem;

// Classify a file by extension. `.meta` sidecars and anything unrecognized return nullopt.
std::optional<AssetType> ClassifyByExtension(const fs::path& path) {
  std::string ext = path.extension().string();
  std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

  if (ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".bmp") return AssetType::Texture;
  if (ext == ".ttf" || ext == ".otf") return AssetType::Font;
  if (ext == ".wav" || ext == ".ogg" || ext == ".mp3" || ext == ".flac") return AssetType::Audio;
  return std::nullopt;
}

std::optional<ScaleMode> ParseScaleMode(const std::string& s) {
  if (s == "nearest") return ScaleMode::Nearest;
  if (s == "linear") return ScaleMode::Linear;
  Logger::Warn("AssetCatalog: unknown scale_mode '" + s + "', ignoring");
  return std::nullopt;
}

// Format a glyph size for use in a font id suffix: whole sizes render without a decimal point
// ("16" not "16.0"), fractional sizes keep enough precision to stay unique ("10.5").
std::string FormatFontSize(float size) {
  if (size == std::floor(size)) {
    return std::to_string(static_cast<long long>(size));
  }
  std::string s = std::to_string(size);
  // Trim trailing zeros from the default 6-digit float formatting.
  s.erase(s.find_last_not_of('0') + 1, std::string::npos);
  if (!s.empty() && s.back() == '.') s.pop_back();
  return s;
}

TextureMeta ParseTextureMeta(const sol::table& t) {
  TextureMeta meta;
  if (const sol::optional<std::string> id = t["id"]; id) meta.id = *id;
  if (const sol::optional<std::string> mode = t["scale_mode"]; mode) meta.scaleMode = ParseScaleMode(*mode);
  if (const sol::optional<std::string> atlas = t["atlas"]; atlas) meta.atlas = *atlas;
  return meta;
}

FontMeta ParseFontMeta(const sol::table& t) {
  FontMeta meta;
  if (const sol::optional<std::string> id = t["id"]; id) meta.id = *id;
  if (const sol::optional<sol::table> sizes = t["sizes"]; sizes) {
    for (const auto& [_, v] : *sizes) {
      if (v.is<float>()) meta.sizes.push_back(v.as<float>());
    }
  } else if (const sol::optional<float> single = t["size"]; single) {
    meta.sizes.push_back(*single);
  }
  return meta;
}

AudioMeta ParseAudioMeta(const sol::table& t) {
  AudioMeta meta;
  if (const sol::optional<std::string> id = t["id"]; id) meta.id = *id;
  if (const sol::optional<bool> stream = t["stream"]; stream) meta.stream = *stream;
  return meta;
}

// Load `<file>.meta` next to an asset, if present, into a Lua table. Returns nullopt when there
// is no sidecar or it fails to produce a table (errors are logged and treated as "use defaults").
std::optional<sol::table> LoadSidecar(sol::state& lua, const fs::path& assetPath) {
  const fs::path metaPath = fs::path(assetPath).concat(".meta");
  std::error_code ec;
  if (!fs::exists(metaPath, ec)) return std::nullopt;

  sol::protected_function_result result = lua.safe_script_file(metaPath.string(), sol::script_pass_on_error);
  if (!result.valid()) {
    const sol::error err = result;
    Logger::Error("AssetCatalog: failed to parse sidecar '" + metaPath.string() + "': " + err.what());
    return std::nullopt;
  }
  if (result.return_count() == 0 || !result[0].is<sol::table>()) {
    Logger::Warn("AssetCatalog: sidecar did not return a table, ignoring: " + metaPath.string());
    return std::nullopt;
  }
  return result[0].get<sol::table>();
}

}  // namespace

bool AssetCatalog::Build(const std::string& basePath, sol::state& lua, const GameConfig& gameConfig) {
  std::optional<ScaleMode> defaultScaleMode;
  if (const auto cfgMode = gameConfig.GetDefaultScaleMode(); cfgMode.has_value()) {
    defaultScaleMode = ParseScaleMode(*cfgMode);
  }
  return Build(basePath, lua, defaultScaleMode);
}

bool AssetCatalog::Build(const std::string& basePath, sol::state& lua, std::optional<ScaleMode> defaultScaleMode) {
  entries_.clear();

  std::error_code ec;
  const fs::path root(basePath);
  if (!fs::exists(root, ec)) {
    Logger::Error("AssetCatalog: base path does not exist: " + basePath);
    return false;
  }

  bool ok = true;
  // Insert one entry, reporting (and rejecting) id collisions with both contributing paths.
  const auto insert = [&](CatalogEntry entry) {
    if (const auto it = entries_.find(entry.id); it != entries_.end()) {
      Logger::Error("AssetCatalog: id collision '" + entry.id + "' between '" + it->second.fullPath + "' and '" +
                    entry.fullPath + "'. Use a meta.id override to disambiguate.");
      ok = false;
      return;
    }
    entries_.emplace(entry.id, std::move(entry));
  };

  for (auto it = fs::recursive_directory_iterator(root, fs::directory_options::skip_permission_denied, ec);
       it != fs::recursive_directory_iterator(); it.increment(ec)) {
    if (ec) {
      Logger::Warn("AssetCatalog: directory iteration error: " + ec.message());
      ec.clear();
      continue;
    }
    if (!it->is_regular_file(ec)) continue;

    const fs::path& path = it->path();
    // Sidecars are metadata, not assets — never catalog them.
    if (path.extension() == ".meta") continue;

    const std::optional<AssetType> type = ClassifyByExtension(path);
    if (!type) continue;

    const std::string fullPath = fs::absolute(path, ec).lexically_normal().string();
    const std::string stem = path.stem().string();
    const std::optional<sol::table> sidecar = LoadSidecar(lua, path);

    switch (*type) {
      case AssetType::Texture: {
        TextureMeta meta = sidecar ? ParseTextureMeta(*sidecar) : TextureMeta{};
        meta.applyDefaults(defaultScaleMode);
        CatalogEntry entry;
        entry.type = AssetType::Texture;
        entry.id = meta.id.value_or(stem);
        entry.fullPath = fullPath;
        entry.scaleMode = meta.scaleMode;
        entry.atlas = meta.atlas;
        insert(std::move(entry));
        break;
      }
      case AssetType::Font: {
        FontMeta meta = sidecar ? ParseFontMeta(*sidecar) : FontMeta{};
        meta.applyDefaults();
        const std::string base = meta.id.value_or(stem);
        for (const float size : meta.sizes) {
          CatalogEntry entry;
          entry.type = AssetType::Font;
          entry.id = base + "-" + FormatFontSize(size);
          entry.fullPath = fullPath;
          entry.fontSize = size;
          insert(std::move(entry));
        }
        break;
      }
      case AssetType::Audio: {
        AudioMeta meta = sidecar ? ParseAudioMeta(*sidecar) : AudioMeta{};
        meta.applyDefaults();
        CatalogEntry entry;
        entry.type = AssetType::Audio;
        entry.id = meta.id.value_or(stem);
        entry.fullPath = fullPath;
        entry.stream = meta.stream;
        insert(std::move(entry));
        break;
      }
    }
  }

  Logger::Info("AssetCatalog: built " + std::to_string(entries_.size()) + " entries from " + basePath +
               (ok ? "" : " (with id collisions)"));
  return ok;
}

const CatalogEntry* AssetCatalog::Find(const std::string& id) const {
  const auto it = entries_.find(id);
  return it == entries_.end() ? nullptr : &it->second;
}

void AssetCatalog::DumpToLog() const {
  for (const auto& [id, entry] : entries_) {
    Logger::Info(std::string("  catalog[") + toAssetTypeString(entry.type) + "] " + id + " -> " + entry.fullPath);
  }
}

void AssetCatalog::InstallLuaIdTable(sol::state& lua, const std::string& globalName) const {
  sol::table ids = lua.create_table();
  for (const auto& [id, _] : entries_) {
    ids.raw_set(id, id);
  }

  // Raise on any key not backed by a real asset id — turns a typo into an immediate scripting
  // error rather than a silent bad-string load downstream.
  sol::table meta = lua.create_table();
  meta.set_function("__index", [](sol::this_state ts, const sol::table&, const std::string& key) -> sol::object {
    luaL_error(ts, "Unknown asset id: '%s'", key.c_str());
    return sol::lua_nil;
  });
  ids[sol::metatable_key] = meta;

  lua[globalName] = ids;
}
