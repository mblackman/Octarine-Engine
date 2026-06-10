// Typed-store lifecycle checks beyond AssetRefcounterTest's pure bookkeeping: TextureStore
// handle ownership + generation bumps, and AssetManager's Acquire/Release orchestration,
// hot-reload swap path, and missing-asset error paths — all against a real (software, headless)
// SDL renderer and a temp-dir catalog built at runtime, since the checked-in fixture images are
// placeholder bytes that don't decode. gtest-free; exit code is the number of failed checks.
// Registered with ctest as AssetStoreTest.

#include <SDL3/SDL.h>

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <sol/sol.hpp>
#include <string>
#include <vector>

#include "AssetManager/AssetCatalog.h"
#include "AssetManager/AssetManager.h"
#include "AssetManager/TextureStore.h"
#include "General/Logger.h"
#include "TestHarness.h"
#include "stb/stb_image_write.h"  // header-only; IMPL lives in AtlasBaker.cpp

using octarine::test::Check;
using octarine::test::CheckEq;

namespace {

// Write a real, decodable PNG (solid color) so IMG_LoadTexture_IO succeeds.
bool WritePng(const std::string& path, const unsigned char r, const unsigned char g, const unsigned char b) {
  std::vector<unsigned char> pixels;
  pixels.reserve(8 * 8 * 4);
  for (int i = 0; i < 8 * 8; ++i) {
    pixels.push_back(r);
    pixels.push_back(g);
    pixels.push_back(b);
    pixels.push_back(255);
  }
  return stbi_write_png(path.c_str(), 8, 8, 4, pixels.data(), 8 * 4) != 0;
}

}  // namespace

int main() {
  Logger::Init();

  // Headless render target: a software renderer over a plain surface needs no window, no GPU,
  // and no video driver — exactly what the store needs to create real SDL_Texture handles in CI.
  SDL_Init(0);
  SDL_Surface* surface = SDL_CreateSurface(64, 64, SDL_PIXELFORMAT_RGBA32);
  SDL_Renderer* renderer = surface != nullptr ? SDL_CreateSoftwareRenderer(surface) : nullptr;
  Check(renderer != nullptr, "software renderer created headlessly");
  if (renderer == nullptr) return octarine::test::ReportSummary("AssetStoreTest");

  const std::filesystem::path tmpDir = std::filesystem::temp_directory_path() / "octarine_asset_store_test";
  std::error_code ec;
  std::filesystem::remove_all(tmpDir, ec);
  std::filesystem::create_directories(tmpDir / "images", ec);

  std::cout << "[texture store] add / replace / remove drive the generation counter\n";
  {
    const std::string pngPath = (tmpDir / "images" / "store.png").string();
    Check(WritePng(pngPath, 255, 0, 0), "test PNG written");

    TextureStore store;
    const std::uint64_t gen0 = store.Generation();

    SDL_Texture* added = store.Add(renderer, "store", SDL_IOFromFile(pngPath.c_str(), "rb"), pngPath);
    Check(added != nullptr, "Add returns the resident texture");
    Check(store.Get("store") == added && store.Contains("store"), "Get/Contains resolve the added id");
    CheckEq(store.Generation(), gen0 + 1, "Add bumps the generation");

    SDL_Texture* replaced = store.Add(renderer, "store", SDL_IOFromFile(pngPath.c_str(), "rb"), pngPath);
    Check(replaced != nullptr && store.Get("store") == replaced, "re-Add under the same id replaces the handle");
    CheckEq(store.Generation(), gen0 + 2, "replace bumps the generation");

    // Undecodable bytes: loader fails, store state and generation stay untouched.
    const std::string bogusPath = (tmpDir / "images" / "bogus.txt").string();
    std::ofstream(bogusPath) << "not an image";
    SDL_Texture* bogus = store.Add(renderer, "bogus", SDL_IOFromFile(bogusPath.c_str(), "rb"), bogusPath);
    Check(bogus == nullptr, "Add returns nullptr for undecodable bytes");
    Check(!store.Contains("bogus"), "failed load leaves no store entry");
    CheckEq(store.Generation(), gen0 + 2, "failed load does not bump the generation");

    Check(store.Remove("store"), "Remove reports an erased entry");
    Check(store.Get("store") == nullptr, "removed id no longer resolves");
    CheckEq(store.Generation(), gen0 + 3, "Remove bumps the generation");
    Check(!store.Remove("store"), "second Remove is a no-op");
    CheckEq(store.Generation(), gen0 + 3, "no-op Remove does not bump the generation");
  }

  std::cout << "[asset manager] acquire/release lifecycle against a runtime catalog\n";
  {
    Check(WritePng((tmpDir / "images" / "sprite.png").string(), 0, 255, 0), "catalog PNG written");

    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::table);

    AssetManager manager;
    Check(manager.GetCatalog().Build(tmpDir.string(), lua, std::optional<ScaleMode>(ScaleMode::Nearest)),
          "catalog builds over the temp asset tree");
    Check(manager.GetCatalog().Find("sprite") != nullptr, "catalog indexed the texture id");

    // Missing-asset error path: unknown ids fail without touching the stores.
    Check(!manager.Acquire("definitely_missing", renderer, nullptr), "Acquire of an unknown id returns false");
    CheckEq(manager.RefCount("definitely_missing"), 0, "failed Acquire leaves no refcount");

    Check(manager.Acquire("sprite", renderer, nullptr), "first Acquire loads the texture");
    Check(manager.GetTexture("sprite") != nullptr, "texture resident after Acquire");
    CheckEq(manager.RefCount("sprite"), 1, "first Acquire counts 1");

    const std::uint64_t genAfterLoad = manager.TextureGeneration();
    Check(manager.Acquire("sprite", renderer, nullptr), "second Acquire succeeds");
    CheckEq(manager.RefCount("sprite"), 2, "second Acquire counts 2");
    CheckEq(manager.TextureGeneration(), genAfterLoad, "re-Acquire does not reload (generation unchanged)");

    manager.Release("sprite");
    CheckEq(manager.RefCount("sprite"), 1, "Release walks the count down");
    Check(manager.GetTexture("sprite") != nullptr, "still resident while referenced");

    manager.Release("sprite");
    CheckEq(manager.RefCount("sprite"), 0, "last Release zeroes the count");
    Check(manager.GetTexture("sprite") == nullptr, "last Release unloads the handle");

    manager.Release("sprite");  // untracked — must be ignored, not underflow
    CheckEq(manager.RefCount("sprite"), 0, "Release of an untracked id is a no-op");

    Check(manager.Acquire("sprite", renderer, nullptr), "id is re-acquirable after full release");
    CheckEq(manager.RefCount("sprite"), 1, "re-acquire restarts the count at 1");
    manager.Release("sprite");
  }

  std::cout << "[asset manager] hot-reload swap preserves references, bumps the generation\n";
  {
    const std::string spritePath = (tmpDir / "images" / "swap.png").string();
    Check(WritePng(spritePath, 0, 0, 255), "initial swap PNG written");

    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::table);

    AssetManager manager;
    Check(manager.GetCatalog().Build(tmpDir.string(), lua, std::optional<ScaleMode>(ScaleMode::Nearest)),
          "catalog builds for the reload run");

    Check(!manager.Reload("swap", renderer, nullptr), "Reload of a non-resident id is a no-op");
    Check(!manager.Reload("definitely_missing", renderer, nullptr), "Reload of an unknown id is a no-op");

    Check(manager.Acquire("swap", renderer, nullptr), "texture acquired before reload");
    Check(manager.Acquire("swap", renderer, nullptr), "second reference taken before reload");
    const std::uint64_t genBefore = manager.TextureGeneration();

    // New bytes on disk, then the dev hot-push path: handle swaps, refcount must survive.
    Check(WritePng(spritePath, 255, 255, 0), "swap PNG overwritten with new bytes");
    Check(manager.Reload("swap", renderer, nullptr), "Reload swaps the resident handle");
    Check(manager.GetTexture("swap") != nullptr, "texture resident after reload");
    CheckEq(manager.RefCount("swap"), 2, "reload preserves the acquire count");
    Check(manager.TextureGeneration() > genBefore, "reload bumps the texture generation");

    // Path-driven variant used by the file watcher: resolves the absolute path back to its ids.
    CheckEq(manager.ReloadByPath(spritePath, renderer, nullptr), 1, "ReloadByPath reloads the backing id");
    CheckEq(manager.ReloadByPath((tmpDir / "images" / "unknown.png").string(), renderer, nullptr), 0,
            "ReloadByPath of an untracked path reloads nothing");

    const std::vector<std::string> resident = manager.ResidentSourcePaths();
    Check(std::find(resident.begin(), resident.end(), manager.GetCatalog().Find("swap")->fullPath) != resident.end(),
          "ResidentSourcePaths reports the resident texture's source file");

    manager.ReleaseAll({"swap", "swap"});
    Check(manager.GetTexture("swap") == nullptr, "ReleaseAll drops both references");
  }

  std::cout << "[asset manager] audio acquire fails cleanly with no mixer\n";
  {
    // The headless/disabled-audio path: a catalog audio entry acquired with a null mixer must
    // report failure (and leave no refcount) instead of crashing or half-loading.
    std::filesystem::create_directories(tmpDir / "sounds", ec);
    std::ofstream((tmpDir / "sounds" / "clip.wav").string(), std::ios::binary) << "RIFF stub";

    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::table);

    AssetManager manager;
    Check(manager.GetCatalog().Build(tmpDir.string(), lua, std::optional<ScaleMode>(ScaleMode::Nearest)),
          "catalog builds with the audio entry");
    Check(manager.GetCatalog().Find("clip") != nullptr, "catalog indexed the audio id");
    Check(!manager.Acquire("clip", renderer, nullptr), "audio Acquire with null mixer returns false");
    CheckEq(manager.RefCount("clip"), 0, "failed audio Acquire leaves no refcount");
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroySurface(surface);
  std::filesystem::remove_all(tmpDir, ec);
  SDL_Quit();

  return octarine::test::ReportSummary("AssetStoreTest");
}
