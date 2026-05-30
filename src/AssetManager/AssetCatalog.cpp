#include "AssetManager/AssetCatalog.h"

#include <SDL3/SDL.h>
#include <sol/sol.hpp>

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <optional>
#include <string>

#include "Game/GameConfig.h"
#include "General/Logger.h"

namespace
{
    namespace fs = std::filesystem;

    // Classify a file by extension. `.meta` sidecars and anything unrecognized return nullopt.
    std::optional<AssetType> ClassifyByExtension(const fs::path& path)
    {
        std::string ext = path.extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c)
        {
            return static_cast<char>(std::tolower(c));
        });

        if (ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".bmp") return AssetType::Texture;
        if (ext == ".ttf" || ext == ".otf") return AssetType::Font;
        if (ext == ".wav" || ext == ".ogg" || ext == ".mp3" || ext == ".flac") return AssetType::Audio;
        return std::nullopt;
    }

    std::optional<ScaleMode> ParseScaleMode(const std::string& s)
    {
        if (s == "nearest") return ScaleMode::Nearest;
        if (s == "linear") return ScaleMode::Linear;
        Logger::Warn("AssetCatalog: unknown scale_mode '" + s + "', ignoring");
        return std::nullopt;
    }

    // Inverse of toAssetTypeString — parse the manifest's `type` field back to an AssetType.
    std::optional<AssetType> ParseAssetType(const std::string& s)
    {
        if (s == "texture") return AssetType::Texture;
        if (s == "font") return AssetType::Font;
        if (s == "audio_clip") return AssetType::Audio;
        return std::nullopt;
    }

    // Resolve a manifest's relative `file` to the same stored form the directory scan produces: an
    // absolute, normalized native path. A re-rooted absolute path means the loaders' GetFullPath
    // (base / path) is a no-op on it, exactly as it is for scanned entries. The one exception is an
    // empty root (Android: SDL_IOFromFile resolves against the APK asset root) — there the relative
    // path is kept verbatim so SDL does the resolution.
    std::string ReRootAssetPath(const fs::path& root, const std::string& file)
    {
        if (root.empty()) return file;
        std::error_code ec;
        return fs::absolute(root / file, ec).lexically_normal().string();
    }

    // Format a glyph size for use in a font id suffix: whole sizes render without a decimal point
    // ("16" not "16.0"), fractional sizes keep enough precision to stay unique ("10.5").
    std::string FormatFontSize(float size)
    {
        if (size == std::floor(size))
        {
            return std::to_string(static_cast<long long>(size));
        }
        std::string s = std::to_string(size);
        // Trim trailing zeros from the default 6-digit float formatting.
        s.erase(s.find_last_not_of('0') + 1, std::string::npos);
        if (!s.empty() && s.back() == '.') s.pop_back();
        return s;
    }

    // Escape a string for embedding inside a Lua double-quoted literal in the emitted manifest.
    std::string EscapeLua(const std::string& s)
    {
        std::string out;
        out.reserve(s.size());
        for (const char c : s)
        {
            if (c == '\\' || c == '"') out.push_back('\\');
            out.push_back(c);
        }
        return out;
    }

    TextureMeta ParseTextureMeta(const sol::table& t)
    {
        TextureMeta meta;
        if (const sol::optional<std::string> id = t["id"]; id) meta.id = *id;
        if (const sol::optional<std::string> mode = t["scale_mode"]; mode) meta.scaleMode = ParseScaleMode(*mode);
        if (const sol::optional<std::string> atlas = t["atlas"]; atlas) meta.atlas = *atlas;
        if (const sol::optional<bool> noAtlas = t["no_atlas"]; noAtlas) meta.noAtlas = *noAtlas;
        return meta;
    }

    FontMeta ParseFontMeta(const sol::table& t)
    {
        FontMeta meta;
        if (const sol::optional<std::string> id = t["id"]; id) meta.id = *id;
        if (const sol::optional<sol::table> sizes = t["sizes"]; sizes)
        {
            for (const auto& [_, v] : *sizes)
            {
                if (v.is<float>()) meta.sizes.push_back(v.as<float>());
            }
        }
        else if (const sol::optional<float> single = t["size"]; single)
        {
            meta.sizes.push_back(*single);
        }
        return meta;
    }

    AudioMeta ParseAudioMeta(const sol::table& t)
    {
        AudioMeta meta;
        if (const sol::optional<std::string> id = t["id"]; id) meta.id = *id;
        if (const sol::optional<bool> stream = t["stream"]; stream) meta.stream = *stream;
        return meta;
    }

    // Load `<file>.meta` next to an asset, if present, into a Lua table. Returns nullopt when there
    // is no sidecar or it fails to produce a table (errors are logged and treated as "use defaults").
    std::optional<sol::table> LoadSidecar(sol::state& lua, const fs::path& assetPath)
    {
        const fs::path metaPath = fs::path(assetPath).concat(".meta");
        std::error_code ec;
        if (!fs::exists(metaPath, ec)) return std::nullopt;

        sol::protected_function_result result = lua.safe_script_file(metaPath.string(), sol::script_pass_on_error);
        if (!result.valid())
        {
            const sol::error err = result;
            Logger::Error("AssetCatalog: failed to parse sidecar '" + metaPath.string() + "': " + err.what());
            return std::nullopt;
        }
        if (result.return_count() == 0 || !result[0].is<sol::table>())
        {
            Logger::Warn("AssetCatalog: sidecar did not return a table, ignoring: " + metaPath.string());
            return std::nullopt;
        }
        return result[0].get<sol::table>();
    }
} // namespace

namespace
{
    // Pull the project default scale mode out of GameConfig (shared by the Build/ScanFilesystem
    // GameConfig overloads).
    std::optional<ScaleMode> DefaultScaleModeFromConfig(const GameConfig& gameConfig)
    {
        if (const auto cfgMode = gameConfig.GetDefaultScaleMode(); cfgMode.has_value())
        {
            return ParseScaleMode(*cfgMode);
        }
        return std::nullopt;
    }
} // namespace

bool AssetCatalog::Build(const std::string& basePath, sol::state& lua, const GameConfig& gameConfig, bool allowManifest)
{
    return Build(basePath, lua, DefaultScaleModeFromConfig(gameConfig), allowManifest);
}

bool AssetCatalog::Build(const std::string& basePath, sol::state& lua, std::optional<ScaleMode> defaultScaleMode,
                         bool allowManifest)
{
    // Shipped builds (allowManifest) carry a baked manifest at the root — load it and skip the
    // filesystem walk so the same path works inside a read-only bundle. Dev builds gate this off
    // (OCTARINE_SHIPPED=OFF, no --use-manifest) and always scan, so a stray manifest left beside
    // source assets can't freeze the catalog on a stale snapshot.
    if (allowManifest)
    {
        // Probe via SDL_IO so the manifest is reachable inside read-only bundles (APK assets / .app
        // Resources / iOS bundle). std::filesystem::exists would only see a real disk path and miss
        // AAssetManager-backed entries.
        const fs::path manifestPath = fs::path(basePath) / "asset_manifest.lua";
        const std::string manifestStr = manifestPath.string();
        if (SDL_IOStream* probe = SDL_IOFromFile(manifestStr.c_str(), "rb"); probe != nullptr)
        {
            SDL_CloseIO(probe);
            // Warn-level on purpose — same rationale as the "loaded N entries" line below: this is
            // the bootstrap path a shipped (warn-level) build needs visible so the manifest-vs-scan
            // gate is observable in CI + on-device support repros.
            Logger::Warn("AssetCatalog: baked manifest found, loading (scan skipped): " + manifestStr);
            return LoadManifest(manifestStr, lua, basePath);
        }
        Logger::Warn("AssetCatalog: manifest load allowed but no asset_manifest.lua at root; scanning.");
    }
    return ScanFilesystem(basePath, lua, defaultScaleMode);
}

bool AssetCatalog::ScanFilesystem(const std::string& basePath, sol::state& lua, const GameConfig& gameConfig)
{
    return ScanFilesystem(basePath, lua, DefaultScaleModeFromConfig(gameConfig));
}

bool AssetCatalog::ScanFilesystem(const std::string& basePath, sol::state& lua,
                                  std::optional<ScaleMode> defaultScaleMode)
{
    entries_.clear();
    from_manifest_ = false;

    std::error_code ec;
    const fs::path root(basePath);
    if (!fs::exists(root, ec))
    {
        Logger::Error("AssetCatalog: base path does not exist: " + basePath);
        return false;
    }
    Logger::Info("AssetCatalog: scanning filesystem at " + basePath);

    bool ok = true;
    // Insert one entry, reporting (and rejecting) id collisions with both contributing paths.
    const auto insert = [&](CatalogEntry entry)
    {
        if (const auto it = entries_.find(entry.id); it != entries_.end())
        {
            Logger::Error("AssetCatalog: id collision '" + entry.id + "' between '" + it->second.fullPath + "' and '" +
                entry.fullPath + "'. Use a meta.id override to disambiguate.");
            ok = false;
            return;
        }
        entries_.emplace(entry.id, std::move(entry));
    };

    for (auto it = fs::recursive_directory_iterator(root, fs::directory_options::skip_permission_denied, ec);
         it != fs::recursive_directory_iterator(); it.increment(ec))
    {
        if (ec)
        {
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

        switch (*type)
        {
        case AssetType::Texture:
            {
                TextureMeta meta = sidecar ? ParseTextureMeta(*sidecar) : TextureMeta{};
                meta.applyDefaults(defaultScaleMode);
                CatalogEntry entry;
                entry.type = AssetType::Texture;
                entry.id = meta.id.value_or(stem);
                entry.fullPath = fullPath;
                entry.scaleMode = meta.scaleMode;
                entry.atlas = meta.atlas;
                entry.noAtlas = meta.noAtlas;
                insert(std::move(entry));
                break;
            }
        case AssetType::Font:
            {
                FontMeta meta = sidecar ? ParseFontMeta(*sidecar) : FontMeta{};
                meta.applyDefaults();
                const std::string base = meta.id.value_or(stem);
                for (const float size : meta.sizes)
                {
                    CatalogEntry entry;
                    entry.type = AssetType::Font;
                    entry.id = base + "-" + FormatFontSize(size);
                    entry.fullPath = fullPath;
                    entry.fontSize = size;
                    insert(std::move(entry));
                }
                break;
            }
        case AssetType::Audio:
            {
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

bool AssetCatalog::LoadManifest(const std::string& manifestPath, sol::state& lua, const std::string& basePath)
{
    entries_.clear();
    from_manifest_ = false;

    // Read manifest bytes through SDL_IO so the same path works inside an APK / .app bundle (where
    // Lua's stock fopen-based loader cannot reach AAssetManager / bundle Resources).
    SDL_IOStream* io = SDL_IOFromFile(manifestPath.c_str(), "rb");
    if (!io)
    {
        Logger::Error("AssetCatalog: failed to open manifest '" + manifestPath + "': " + SDL_GetError());
        return false;
    }
    std::size_t size = 0;
    void* data = SDL_LoadFile_IO(io, &size, true);  // closes io
    if (!data)
    {
        Logger::Error("AssetCatalog: failed to read manifest '" + manifestPath + "': " + SDL_GetError());
        return false;
    }
    const std::string chunk(static_cast<const char*>(data), size);
    SDL_free(data);

    sol::protected_function_result result =
        lua.safe_script(chunk, sol::script_pass_on_error, "@" + manifestPath);
    if (!result.valid())
    {
        const sol::error err = result;
        Logger::Error("AssetCatalog: failed to load manifest '" + manifestPath + "': " + err.what());
        return false;
    }
    if (result.return_count() == 0 || !result[0].is<sol::table>())
    {
        Logger::Error("AssetCatalog: manifest did not return a table: " + manifestPath);
        return false;
    }

    const fs::path root(basePath);
    const sol::table table = result[0];
    bool ok = true;

    for (const auto& [key, value] : table)
    {
        const std::string id = key.as<std::string>();
        if (!value.is<sol::table>())
        {
            Logger::Error("AssetCatalog: manifest entry '" + id + "' is not a table; skipping.");
            ok = false;
            continue;
        }
        const sol::table e = value;

        const sol::optional<std::string> typeStr = e["type"];
        const sol::optional<std::string> file = e["file"];
        if (!typeStr || !file)
        {
            Logger::Error("AssetCatalog: manifest entry '" + id + "' missing required 'type'/'file'; skipping.");
            ok = false;
            continue;
        }
        const std::optional<AssetType> type = ParseAssetType(*typeStr);
        if (!type)
        {
            Logger::Error("AssetCatalog: manifest entry '" + id + "' has unknown type '" + *typeStr + "'; skipping.");
            ok = false;
            continue;
        }

        CatalogEntry entry;
        entry.type = *type;
        entry.id = id;
        entry.fullPath = ReRootAssetPath(root, *file);
        switch (*type)
        {
        case AssetType::Texture:
            if (const sol::optional<std::string> mode = e["scale_mode"]; mode) entry.scaleMode = ParseScaleMode(*mode);
            if (const sol::optional<std::string> atlas = e["atlas"]; atlas) entry.atlas = *atlas;
            if (const sol::optional<bool> noAtlas = e["no_atlas"]; noAtlas) entry.noAtlas = *noAtlas;
            if (const sol::optional<std::string> atlasId = e["atlas_id"]; atlasId) entry.atlasId = *atlasId;
            if (const sol::optional<sol::table> slice = e["atlas_slice"]; slice)
            {
                SDL_FRect r{};
                r.x = (*slice)["x"].get_or(0.0F);
                r.y = (*slice)["y"].get_or(0.0F);
                r.w = (*slice)["w"].get_or(0.0F);
                r.h = (*slice)["h"].get_or(0.0F);
                entry.atlasSlice = r;
            }
            break;
        case AssetType::Font:
            entry.fontSize = e["font_size"].get_or(0.0F);
            break;
        case AssetType::Audio:
            entry.stream = e["stream"].get_or(false);
            break;
        }
        entries_.emplace(std::move(id), std::move(entry));
    }

    from_manifest_ = true;
    // Warn-level on purpose: this is the single bootstrap signal that a shipped build is loading
    // its baked catalog (vs scanning). Shipped builds default to warn-level logging (per
    // BuildConfigUnificationPlan phase 3), so an Info log here would be suppressed and the
    // android-emulator gate's manifest-load assertion would never match. Keep as a single line
    // per launch — it's a startup confirmation, not noise.
    Logger::Warn("AssetCatalog: loaded " + std::to_string(entries_.size()) + " entries from manifest " + manifestPath +
        (ok ? "" : " (with malformed entries skipped)"));
    return ok;
}

const CatalogEntry* AssetCatalog::Find(const std::string& id) const
{
    const auto it = entries_.find(id);
    return it == entries_.end() ? nullptr : &it->second;
}

void AssetCatalog::InsertOrReplace(CatalogEntry entry)
{
    const std::string id = entry.id;
    entries_.insert_or_assign(id, std::move(entry));
}

void AssetCatalog::SetAtlasMembership(const std::string& memberId, std::string atlasId, SDL_FRect atlasSlice)
{
    const auto it = entries_.find(memberId);
    if (it == entries_.end()) return;
    it->second.atlasId = std::move(atlasId);
    it->second.atlasSlice = atlasSlice;
}

void AssetCatalog::DumpToLog() const
{
    for (const auto& [id, entry] : entries_)
    {
        Logger::Info(std::string("  catalog[") + toAssetTypeString(entry.type) + "] " + id + " -> " + entry.fullPath);
    }
}

bool AssetCatalog::WriteManifest(const std::string& outPath, const std::string& basePath) const
{
    std::ofstream out(outPath, std::ios::binary);
    if (!out.is_open())
    {
        Logger::Error("AssetCatalog: failed to open manifest for writing: " + outPath);
        return false;
    }

    const fs::path root = fs::path(basePath).lexically_normal();

    out << "-- Auto-generated by `OctarineEngine <project> -m bake`. Do not edit by hand.\n";
    out << "-- Maps asset id -> { type, file (relative to the asset root), metadata }.\n";
    out << "return {\n";

    // entries_ is a std::map, so iteration is id-sorted and the manifest is deterministic.
    for (const auto& [id, entry] : entries_)
    {
        std::error_code ec;
        const fs::path rel = fs::relative(fs::path(entry.fullPath), root, ec);
        const std::string file = (ec || rel.empty()) ? entry.fullPath : rel.generic_string();

        out << "  [\"" << EscapeLua(id) << "\"] = { type = \"" << toAssetTypeString(entry.type) << "\", file = \""
            << EscapeLua(file) << "\"";

        switch (entry.type)
        {
        case AssetType::Texture:
            if (entry.scaleMode.has_value())
            {
                out << ", scale_mode = \"" << (*entry.scaleMode == ScaleMode::Linear ? "linear" : "nearest") << "\"";
            }
            if (entry.atlas.has_value())
            {
                out << ", atlas = \"" << EscapeLua(*entry.atlas) << "\"";
            }
            if (entry.noAtlas)
            {
                out << ", no_atlas = true";
            }
            if (entry.atlasId.has_value())
            {
                out << ", atlas_id = \"" << EscapeLua(*entry.atlasId) << "\"";
            }
            if (entry.atlasSlice.has_value())
            {
                const auto& s = *entry.atlasSlice;
                out << ", atlas_slice = { x = " << static_cast<int>(s.x) << ", y = " << static_cast<int>(s.y)
                    << ", w = " << static_cast<int>(s.w) << ", h = " << static_cast<int>(s.h) << " }";
            }
            break;
        case AssetType::Font:
            out << ", font_size = " << FormatFontSize(entry.fontSize);
            break;
        case AssetType::Audio:
            out << ", stream = " << (entry.stream ? "true" : "false");
            break;
        }

        out << " },\n";
    }

    out << "}\n";
    return out.good();
}

void AssetCatalog::InstallLuaIdTable(sol::state& lua, const std::string& globalName) const
{
    sol::table ids = lua.create_table();
    for (const auto& [id, _] : entries_)
    {
        ids.raw_set(id, id);
    }

    // Raise on any key not backed by a real asset id — turns a typo into an immediate scripting
    // error rather than a silent bad-string load downstream.
    sol::table meta = lua.create_table();
    meta.set_function("__index", [](sol::this_state ts, const sol::table&, const std::string& key) -> sol::object
    {
        luaL_error(ts, "Unknown asset id: '%s'", key.c_str());
        return sol::lua_nil;
    });
    ids[sol::metatable_key] = meta;

    lua[globalName] = ids;
}
