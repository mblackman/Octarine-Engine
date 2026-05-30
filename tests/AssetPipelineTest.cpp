// Unit checks for the asset pipeline data model + catalog. gtest-free; exit code is the number
// of failed checks (mirrors tests/LuaApiSmokeTest.cpp style). Built only when
// OCTARINE_ENABLE_TESTS=ON and registered with ctest as AssetPipelineTest.
//
// Tests the metadata default-fill rules and catalog building.
// Checks are pointed at a fixture directory.

#include <sol/sol.hpp>

#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "AssetManager/AssetCatalog.h"
#include "AssetManager/AssetManager.h"
#include "AssetManager/AssetMetadata.h"
#include "AssetManager/AssetPak.h"
#include "AssetManager/AssetReference.h"
#include "AssetManager/AtlasBaker.h"
#include "AssetManager/AudioNormalizer.h"
#include "AssetManager/GlyphAtlas.h"
#include "stb/stb_image_write.h"  // header-only; IMPL lives in AtlasBaker.cpp
#include "AssetManager/SceneAssetScanner.h"
#include <SDL3/SDL.h>
#include <cmath>
#include <SDL3_ttf/SDL_ttf.h>

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

        // 3 textures (hero, boss, branding) + 3 fonts (main-16, title-12, title-24) + 2 audio
        // (jump default-decode, music meta.stream=true).
        Check(catalog.Size() == 8, "catalog discovered 8 entries");

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

        const CatalogEntry* music = catalog.Find("music");
        Check(music != nullptr && music->type == AssetType::Audio, "music.wav is audio");
        Check(music != nullptr && music->stream, "music.wav meta.stream=true propagates to catalog");
    }

    std::cout << "[manifest] baked manifest matches the directory scan\n";
    {
        sol::state lua;
        lua.open_libraries(sol::lib::base, sol::lib::table);

        // Scan the fixture, bake it to a manifest, then reload from that manifest re-rooted against the
        // same base. The two catalogs must be entry-for-entry identical (scan/manifest parity).
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

    std::cout << "[pak] round-trip pack/open via fixture catalog\n";
    {
        // SDL needed for SDL_LoadFile / SDL_IOFromConstMem during Open/OpenIO.
        SDL_Init(0);

        sol::state lua;
        lua.open_libraries(sol::lib::base, sol::lib::table);

        AssetCatalog catalog;
        catalog.Build(ASSET_TEST_FIXTURE_DIR, lua, std::optional<ScaleMode>(ScaleMode::Nearest));
        Check(catalog.Size() > 0, "pak round-trip: fixture catalog has entries");

        const std::filesystem::path tmpDir =
            std::filesystem::temp_directory_path() / "octarine_asset_pak_test";
        std::error_code ec;
        std::filesystem::create_directories(tmpDir, ec);
        const std::string pakPath = (tmpDir / "asset_bundle.pak").string();

        const bool packed = AssetPak::Pack(catalog, pakPath, ASSET_TEST_FIXTURE_DIR);
        Check(packed, "AssetPak::Pack succeeds");

        AssetPak pak;
        Check(pak.Open(pakPath), "AssetPak::Open succeeds on freshly packed file");
        // The pak dedupes by source file: a font that appears in the catalog at multiple sizes
        // shares one underlying file + one pak entry. Compare against unique fullPath count, not
        // raw catalog.Size().
        std::set<std::string> uniqueFullPaths;
        for (const auto& [id, entry] : catalog.Entries()) {
            uniqueFullPaths.insert(entry.fullPath);
        }
        Check(pak.Size() == uniqueFullPaths.size(), "pak entry count matches unique source-file count");

        // Byte-compare every entry: read original file off disk, fetch the pak slice via OpenIO,
        // confirm length + contents match. Proves the offset/size TOC + memory-mapped slice
        // serve the same bytes the loose tree would have.
        int compared = 0;
        int mismatch = 0;
        for (const auto& [id, entry] : catalog.Entries())
        {
            const std::filesystem::path rel = std::filesystem::relative(
                std::filesystem::path(entry.fullPath),
                std::filesystem::path(ASSET_TEST_FIXTURE_DIR));
            const std::string relStr = rel.generic_string();

            std::ifstream src(entry.fullPath, std::ios::binary);
            const std::vector<char> srcBytes((std::istreambuf_iterator<char>(src)),
                                              std::istreambuf_iterator<char>());

            SDL_IOStream* io = pak.OpenIO(relStr);
            if (io == nullptr)
            {
                ++mismatch;
                continue;
            }
            std::vector<char> pakBytes(srcBytes.size());
            const size_t got = SDL_ReadIO(io, pakBytes.data(), pakBytes.size());
            SDL_CloseIO(io);
            if (got != srcBytes.size() || std::memcmp(pakBytes.data(), srcBytes.data(), got) != 0)
            {
                ++mismatch;
            }
            ++compared;
        }
        Check(compared == static_cast<int>(catalog.Size()), "every catalog entry resolves through the pak");
        Check(mismatch == 0, "every pak entry's bytes match the source file exactly");

        std::filesystem::remove_all(tmpDir, ec);
        SDL_Quit();
    }

    std::cout << "[glyph atlas] synthetic Lua + PNG round-trip\n";
    {
        // AtlasBaker against a real TTF is exercised by the bake smoke against
        // Octarine-Engine-Example in CI — the fixture font under tests/fixtures/assets/fonts/
        // is a stub ASCII placeholder, not a real face. Here we test only the GlyphAtlas loader
        // shape: a synthetic 4×4 RGBA PNG + a hand-rolled metrics Lua file should round-trip
        // through `GlyphAtlas::Load` cleanly.
        SDL_Init(0);

        const std::filesystem::path tmpDir =
            std::filesystem::temp_directory_path() / "octarine_atlas_synthetic_test";
        std::error_code ec;
        std::filesystem::create_directories(tmpDir, ec);
        const std::string pngPath = (tmpDir / "synth.atlas.png").string();
        const std::string luaPath = (tmpDir / "synth.atlas.lua").string();

        // 4×4 fully-opaque white via stbi_write — easier than handcrafting a PNG byte stream.
        {
            std::vector<unsigned char> pixels(4 * 4 * 4, 255);
            Check(stbi_write_png(pngPath.c_str(), 4, 4, 4, pixels.data(), 16) != 0,
                  "synthetic 4x4 atlas PNG written via stbi_write_png");
        }
        {
            std::ofstream lua(luaPath);
            lua << "return {\n"
                << "  size = 16,\n  atlas_width = 4,\n  atlas_height = 4,\n  line_skip = 16,\n"
                << "  glyphs = {\n"
                << "    [65] = { x=0, y=0, w=4, h=4, advance=5, minx=0, miny=0 },\n"
                << "  },\n}\n";
        }

        GlyphAtlas atlas;
        Check(atlas.Load(pngPath, luaPath), "GlyphAtlas::Load round-trips synthetic atlas");
        Check(atlas.IsLoaded(), "GlyphAtlas::IsLoaded true after Load");
        Check(atlas.Size() == 1, "synthetic atlas reports exactly one glyph");
        Check(atlas.Contains(static_cast<std::uint32_t>('A')), "synthetic atlas contains 'A'");
        Check(!atlas.Contains(static_cast<std::uint32_t>('B')), "synthetic atlas correctly excludes 'B'");
        Check(atlas.AtlasWidth() == 4 && atlas.AtlasHeight() == 4, "synthetic atlas reports 4x4 dims");
        Check(atlas.LineSkip() == 16, "synthetic atlas reports line_skip=16");

        const GlyphAtlas::Glyph* gA = atlas.Find(static_cast<std::uint32_t>('A'));
        Check(gA != nullptr && gA->advance == 5.0F, "'A' glyph advance round-trips");

        // AtlasBaker's DefaultAsciiPrintable helper is independent of TTF availability.
        const auto cps = AtlasBaker::DefaultAsciiPrintable();
        Check(cps.size() == 95, "DefaultAsciiPrintable returns 95 codepoints (0x20..0x7E)");
        Check(cps.front() == 0x20 && cps.back() == 0x7E, "DefaultAsciiPrintable spans space..tilde");

        std::filesystem::remove_all(tmpDir, ec);
        SDL_Quit();
    }

    std::cout << "[audio normalize] BS.1770 measure + gain round-trip\n";
    {
        // Synthesize 2 seconds of a quiet 1 kHz sine wave (amplitude 0.1 in [-1,1], 48 kHz mono),
        // measure its loudness, normalize to -16 LUFS, measure again. The bake step writes WAVs
        // via AudioNormalizer's same path, so this exercises ReadWav/WriteWav + the full BS.1770
        // chain end-to-end.
        const std::filesystem::path tmpDir =
            std::filesystem::temp_directory_path() / "octarine_audio_norm_test";
        std::error_code ec;
        std::filesystem::create_directories(tmpDir, ec);
        const std::string srcPath = (tmpDir / "quiet_tone.wav").string();
        const std::string dstPath = (tmpDir / "normalized.wav").string();

        {
            constexpr std::uint32_t sampleRate = 48000;
            constexpr std::uint32_t frames = sampleRate * 2;  // 2 seconds
            constexpr double amplitude = 0.1;
            constexpr double freq = 1000.0;
            std::vector<std::int16_t> samples(frames);
            for (std::uint32_t i = 0; i < frames; ++i) {
                const double t = static_cast<double>(i) / sampleRate;
                const double v = amplitude * std::sin(2.0 * 3.14159265358979323846 * freq * t);
                samples[i] = static_cast<std::int16_t>(std::lrint(v * 32767.0));
            }
            // Hand-rolled minimal WAV writer mirroring AudioNormalizer's expected shape.
            std::ofstream f(srcPath, std::ios::binary);
            const std::uint32_t dataBytes = static_cast<std::uint32_t>(samples.size() * 2);
            const std::uint32_t fileSize = 36 + dataBytes;
            const std::uint16_t numChannels = 1;
            const std::uint16_t bitsPerSample = 16;
            const std::uint16_t blockAlign = numChannels * (bitsPerSample / 8);
            const std::uint32_t byteRate = sampleRate * blockAlign;
            const std::uint16_t audioFormat = 1;
            const std::uint32_t fmtSize = 16;
            f.write("RIFF", 4);
            f.write(reinterpret_cast<const char*>(&fileSize), 4);
            f.write("WAVE", 4);
            f.write("fmt ", 4);
            f.write(reinterpret_cast<const char*>(&fmtSize), 4);
            f.write(reinterpret_cast<const char*>(&audioFormat), 2);
            f.write(reinterpret_cast<const char*>(&numChannels), 2);
            f.write(reinterpret_cast<const char*>(&sampleRate), 4);
            f.write(reinterpret_cast<const char*>(&byteRate), 4);
            f.write(reinterpret_cast<const char*>(&blockAlign), 2);
            f.write(reinterpret_cast<const char*>(&bitsPerSample), 2);
            f.write("data", 4);
            f.write(reinterpret_cast<const char*>(&dataBytes), 4);
            f.write(reinterpret_cast<const char*>(samples.data()), dataBytes);
        }

        const double preLufs = AudioNormalizer::MeasureWavLufs(srcPath);
        Check(std::isfinite(preLufs), "BS.1770 measures synthetic quiet sine to a finite LUFS");
        Check(preLufs < -20.0, "synthetic quiet sine measures well below -20 LUFS");

        const double targetLufs = -16.0;
        Check(AudioNormalizer::NormalizeWav(srcPath, dstPath, targetLufs),
              "AudioNormalizer::NormalizeWav writes the normalized WAV");

        const double postLufs = AudioNormalizer::MeasureWavLufs(dstPath);
        Check(std::isfinite(postLufs), "normalized output measures to a finite LUFS");
        Check(std::abs(postLufs - targetLufs) < 1.0,
              "normalized output lands within 1 LU of the -16 LUFS target");

        std::filesystem::remove_all(tmpDir, ec);
    }

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
