// Unit checks for the asset pipeline data model + catalog. gtest-free; exit code is the number
// of failed checks (mirrors tests/LuaApiSmokeTest.cpp style). Built only when
// OCTARINE_ENABLE_TESTS=ON and registered with ctest as AssetPipelineTest.
//
// Tests the metadata default-fill rules and catalog building.
// Checks are pointed at a fixture directory.

#include <sol/sol.hpp>

#include <filesystem>
#include <iostream>
#include <optional>
#include <string>

#include "AssetManager/AssetCatalog.h"
#include "AssetManager/AssetManager.h"
#include "AssetManager/AssetMetadata.h"
#include "AssetManager/AssetReference.h"
#include "AssetManager/SceneAssetScanner.h"

namespace
{
    int g_failures = 0;

    void Check(const bool condition, const std::string& what)
    {
        if (condition)
        {
            std::cout << "  ok   " << what << "\n";
        }
        else
        {
            std::cerr << "  FAIL " << what << "\n";
            ++g_failures;
        }
    }
} // namespace

int main()
{
    std::cout << "[metadata] default-fill\n";

    // Texture: no sidecar value + no project default -> nearest, no atlas.
    {
        TextureMeta m;
        m.applyDefaults(std::nullopt);
        Check(m.scaleMode == ScaleMode::Nearest, "texture scaleMode defaults to nearest");
        Check(!m.atlas.has_value(), "texture atlas defaults to none");
    }
    // Texture: project default applies when the sidecar omits scaleMode.
    {
        TextureMeta m;
        m.applyDefaults(ScaleMode::Linear);
        Check(m.scaleMode == ScaleMode::Linear, "texture scaleMode inherits project default");
    }
    // Texture: an explicit sidecar value beats the project default.
    {
        TextureMeta m;
        m.scaleMode = ScaleMode::Nearest;
        m.applyDefaults(ScaleMode::Linear);
        Check(m.scaleMode == ScaleMode::Nearest, "explicit texture scaleMode beats default");
    }
    // Font: empty sizes -> { kDefaultFontSize }.
    {
        FontMeta m;
        m.applyDefaults();
        Check(m.sizes.size() == 1 && m.sizes[0] == kDefaultFontSize, "font sizes default to { 16 }");
    }
    // Font: provided sizes preserved.
    {
        FontMeta m;
        m.sizes = {5.0F, 10.0F};
        m.applyDefaults();
        Check(m.sizes.size() == 2, "font sizes preserved when provided");
    }
    // Audio: stream defaults to false (full decode).
    {
        AudioMeta m;
        m.applyDefaults();
        Check(!m.stream, "audio stream defaults to false");
    }

    std::cout << "[scale-mode] string conversion\n";
    Check(std::string(toScaleModeString(ScaleMode::Nearest)) == "nearest", "ScaleMode::Nearest -> \"nearest\"");
    Check(std::string(toScaleModeString(ScaleMode::Linear)) == "linear", "ScaleMode::Linear -> \"linear\"");

#ifdef ASSET_TEST_FIXTURE_DIR
    std::cout << "[catalog] build against fixture dir\n";
    {
        sol::state lua;
        lua.open_libraries(sol::lib::base, sol::lib::table);

        AssetCatalog catalog;
        // Project default = nearest; sidecars override per-file.
        const bool ok = catalog.Build(ASSET_TEST_FIXTURE_DIR, lua, std::optional<ScaleMode>(ScaleMode::Nearest));
        Check(ok, "catalog build reports no id collisions");

        // 3 textures (hero, boss, branding) + 3 fonts (main-16, title-12, title-24) + 1 audio (jump).
        Check(catalog.Size() == 7, "catalog discovered 7 entries");

        const CatalogEntry* hero = catalog.Find("hero");
        Check(hero != nullptr && hero->type == AssetType::Texture, "hero is a texture");
        Check(hero != nullptr && hero->scaleMode == ScaleMode::Linear, "hero scale_mode from sidecar (linear)");

        const CatalogEntry* boss = catalog.Find("boss");
        Check(boss != nullptr && boss->scaleMode == ScaleMode::Nearest, "boss inherits project default (nearest)");

        Check(catalog.Contains("branding"), "logo.png meta.id override -> 'branding'");
        Check(!catalog.Contains("logo"), "overridden id does not also appear under the stem");

        Check(catalog.Contains("main-16"), "main.ttf default size -> 'main-16'");
        Check(catalog.Contains("title-12") && catalog.Contains("title-24"), "title.ttf expands per size with id base");

        const CatalogEntry* jump = catalog.Find("jump");
        Check(jump != nullptr && jump->type == AssetType::Audio, "jump.wav is audio");
        Check(jump != nullptr && !jump->stream, "jump defaults to non-streaming");
    }

    std::cout << "[manifest] baked manifest matches the directory scan\n";
    {
        sol::state lua;
        lua.open_libraries(sol::lib::base, sol::lib::table);

        // Scan the fixture, bake it to a manifest, then reload from that manifest re-rooted against the
        // same base. The two catalogs must be entry-for-entry identical (Stage 6 parity guarantee).
        AssetCatalog scanned;
        scanned.Build(ASSET_TEST_FIXTURE_DIR, lua, std::optional<ScaleMode>(ScaleMode::Nearest));
        Check(!scanned.IsFromManifest(), "directory scan does not report manifest origin");

        const std::string manifestPath =
            (std::filesystem::temp_directory_path() / "octarine_test_asset_manifest.lua").string();
        Check(scanned.WriteManifest(manifestPath, ASSET_TEST_FIXTURE_DIR), "WriteManifest succeeds");

        AssetCatalog loaded;
        sol::state lua2;
        lua2.open_libraries(sol::lib::base, sol::lib::table);
        Check(loaded.LoadManifest(manifestPath, lua2, ASSET_TEST_FIXTURE_DIR), "LoadManifest succeeds");
        Check(loaded.IsFromManifest(), "manifest load reports manifest origin");
        Check(loaded.Size() == scanned.Size(), "manifest entry count matches the scan");

        bool parity = true;
        for (const auto& [id, want] : scanned.Entries())
        {
            const CatalogEntry* got = loaded.Find(id);
            if (got == nullptr || got->type != want.type || got->fullPath != want.fullPath ||
                got->scaleMode != want.scaleMode || got->atlas != want.atlas || got->fontSize != want.fontSize ||
                got->stream != want.stream)
            {
                parity = false;
                std::cerr << "    mismatch on id '" << id << "'\n";
            }
        }
        Check(parity, "every manifest entry matches its scanned counterpart");

        std::error_code ec;
        std::filesystem::remove(manifestPath, ec);
    }
#else
    std::cerr << "  WARN ASSET_TEST_FIXTURE_DIR not defined; skipping catalog build checks\n";
#endif

    std::cout << "[scanner] scene dependency collection\n";
    {
        sol::state lua;
        lua.open_libraries(sol::lib::base);
        const auto scene = lua.script(R"lua(
      return {
        tilemap = { texture_asset_id = "tilemap-tex" },
        preload = { "bullet-tex", "spawn-font-10" },
        entities = {
          {
            components = {
              sprite = { texture_asset_id = "player-tex" },
              audio_source = { clip_id = "engine-hum" },
            },
            entities = {
              { components = { text_label = { font_id = "hud-font" } } },
              { components = { sprite = { texture_asset_id = "player-tex" } } },  -- dup
            },
          },
          { components = { sprite = { texture_asset_id = "enemy-tex" } } },
        },
      }
    )lua");

        const auto ids = SceneAssetScanner::Collect(scene);
        Check(ids.count("tilemap-tex") == 1, "scanner picks up tilemap texture");
        Check(ids.count("bullet-tex") == 1 && ids.count("spawn-font-10") == 1, "scanner unions the preload list");
        Check(ids.count("player-tex") == 1, "scanner reads nested sprite texture_asset_id");
        Check(ids.count("enemy-tex") == 1, "scanner reads sibling-entity sprite");
        Check(ids.count("engine-hum") == 1, "scanner reads audio_source clip_id");
        Check(ids.count("hud-font") == 1, "scanner reads text_label font_id");
        Check(ids.size() == 7, "scanner dedupes repeated ids (7 unique)");
    }

#ifdef ASSET_TEST_FIXTURE_DIR
    std::cout << "[validation] reference checks against catalog\n";
    {
        sol::state lua;
        lua.open_libraries(sol::lib::base, sol::lib::table);

        AssetManager am;
        am.GetCatalog().Build(ASSET_TEST_FIXTURE_DIR, lua, std::optional<ScaleMode>(ScaleMode::Nearest));

        const std::vector<AssetReference> refs = {
            {"hero", "entity 'A' sprite.texture_asset_id"}, // valid
            {"main-16", "entity 'B' text_label.font_id"}, // valid
            {"nope", "entity 'C' sprite.texture_asset_id"}, // missing from catalog
        };
        Check(am.Validate(refs) == 1, "validation flags exactly the one unknown id");

        const std::vector<AssetReference> clean = {{"hero", "x"}, {"branding", "y"}};
        Check(am.Validate(clean) == 0, "validation passes a clean reference set");

        // Refcount accounting on the no-load paths (real SDL loads need a renderer + real files, so
        // those are exercised by the runtime scene-swap check, not here).
        Check(am.RefCount("hero") == 0, "untracked id starts at refcount 0");
        Check(!am.Acquire("totally-missing-id", nullptr, nullptr), "Acquire of an unknown id fails");
        Check(am.RefCount("totally-missing-id") == 0, "failed Acquire leaves no refcount entry");
        am.Release("never-acquired"); // must be a safe no-op
        Check(am.RefCount("never-acquired") == 0, "Release of an untracked id is a no-op");
    }

    std::cout << "[assets-table] id proxy raises on typo\n";
    {
        sol::state lua;
        lua.open_libraries(sol::lib::base, sol::lib::table);

        AssetCatalog catalog;
        catalog.Build(ASSET_TEST_FIXTURE_DIR, lua, std::optional<ScaleMode>(ScaleMode::Nearest));
        catalog.InstallLuaIdTable(lua);

        const sol::protected_function_result good = lua.safe_script("return assets.hero", sol::script_pass_on_error);
        Check(good.valid() && good.get<std::string>() == "hero", "assets.hero resolves to its id");

        const sol::protected_function_result bad = lua.safe_script("return assets.definitely_missing",
                                                                   sol::script_pass_on_error);
        Check(!bad.valid(), "assets.<unknown> raises instead of returning nil");
    }
#endif

    if (g_failures == 0)
    {
        std::cout << "\nAsset pipeline test: PASS\n";
    }
    else
    {
        std::cerr << "\nAsset pipeline test: " << g_failures << " FAILED\n";
    }
    return g_failures;
}
