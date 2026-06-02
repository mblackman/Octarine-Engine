// Headless Game + bake pipeline checks.
// - Constructor allocates Registry/EventBus/Lua without touching SDL (matches the documented
//   "Game game;" shape used by the smoke test).
// - Game::Bake(fixtureDir) runs end-to-end against a minimal project fixture: loads config,
//   scans the catalog, runs the startup script, writes asset_manifest.lua. Verifies the
//   manifest lands on disk.
// - SceneAssetScanner::CollectRefs preserves duplicates + provenance (Collect's dedup wrapper
//   is exercised by AssetPipelineTest).

#include <filesystem>
#include <sol/sol.hpp>
#include <string>

#include "AssetManager/AssetReference.h"
#include "AssetManager/SceneAssetScanner.h"
#include "ECS/Registry.h"
#include "EventBus/EventBus.h"
#include "Game/Game.h"
#include "General/Logger.h"
#include "TestHarness.h"

using octarine::test::Check;
using octarine::test::CheckEq;

int main() {
  // Bake calls Logger::Info widely (works without Init via spdlog's default), but the bound
  // Lua log_i / log_e in the startup script dereference lua_logger_ which Init sets up.
  Logger::Init();

  std::cout << "[headless Game construction]\n";
  {
    // The constructor must not open a window, init SDL, or create a renderer — only allocate
    // the registry/eventbus/lua state. Smoke + behavior tests both rely on this guarantee.
    Game game;
    Check(game.GetRegistry() != nullptr, "Game::GetRegistry() returns a live Registry");
    Check(game.GetWindow() == nullptr, "Game::GetWindow() is nullptr before Initialize()");
    Check(game.GetRenderer() == nullptr, "Game::GetRenderer() is nullptr before Initialize()");

    // Registry is usable: create an entity, verify counters.
    const Entity e = game.GetRegistry()->CreateEntity();
    Check(game.GetRegistry()->IsAlive(e), "Registry held by headless Game accepts CreateEntity");
    CheckEq(game.GetRegistry()->GetUserEntityCount(), 1ULL, "user entity count visible on headless Game");
  }

  std::cout << "[scanner: CollectRefs preserves duplicates + provenance]\n";
  {
    sol::state lua;
    lua.open_libraries(sol::lib::base);
    const auto scene = lua.script(R"lua(
          return {
            preload = { "shared-tex" },
            entities = {
              {
                components = {
                  sprite = { texture_asset_id = "shared-tex" },
                },
              },
              {
                components = {
                  sprite = { texture_asset_id = "shared-tex" },  -- duplicate id, distinct ref
                },
              },
              {
                components = {
                  audio_source = { clip_id = "blip" },
                },
              },
            },
          }
        )lua");

    const std::vector<AssetReference> refs = SceneAssetScanner::CollectRefs(scene);
    // 1 preload + 2 sprite refs + 1 audio = 4 references with the same dup id present twice.
    CheckEq(refs.size(), size_t{4}, "CollectRefs returns one entry per reference site");

    int sharedTexCount = 0;
    bool foundBlip = false;
    for (const auto& ref : refs) {
      if (ref.id == "shared-tex") ++sharedTexCount;
      if (ref.id == "blip") foundBlip = true;
      Check(!ref.context.empty(), "every ref carries provenance context");
    }
    CheckEq(sharedTexCount, 3, "shared-tex appears 3x in CollectRefs (preload + 2 sprites)");
    Check(foundBlip, "CollectRefs picks up audio_source.clip_id");

    // The dedup wrapper (Collect) is the production path for "what's in this scene".
    const auto ids = SceneAssetScanner::Collect(scene);
    CheckEq(ids.size(), size_t{2}, "Collect dedups CollectRefs down to 2 distinct ids");
  }

  std::cout << "[scanner: cross-scene delta — shared vs. exclusive ids]\n";
  {
    sol::state lua;
    lua.open_libraries(sol::lib::base);
    const auto sceneA = lua.script(R"lua(
          return {
            preload = { "shared", "only-in-a" },
            entities = {},
          }
        )lua");
    const auto sceneB = lua.script(R"lua(
          return {
            preload = { "shared", "only-in-b-1", "only-in-b-2" },
            entities = {},
          }
        )lua");

    const auto a = SceneAssetScanner::Collect(sceneA);
    const auto b = SceneAssetScanner::Collect(sceneB);
    Check(a.count("shared") == 1 && b.count("shared") == 1, "shared id present in both scenes");
    Check(a.count("only-in-a") == 1 && b.count("only-in-a") == 0, "only-in-a exclusive to scene A");
    Check(a.count("only-in-b-1") == 0 && b.count("only-in-b-1") == 1, "only-in-b-1 exclusive to scene B");

    // Acquire-before-release semantics from Game::LoadScene depend on this set difference being
    // correct: ids only in A get released, ids only in B get acquired, shared ids untouched.
    std::vector<std::string> toRelease;
    for (const auto& id : a) {
      if (b.find(id) == b.end()) toRelease.push_back(id);
    }
    std::vector<std::string> toAcquire;
    for (const auto& id : b) {
      if (a.find(id) == a.end()) toAcquire.push_back(id);
    }
    CheckEq(toRelease.size(), size_t{1}, "exactly one id to release across A→B swap");
    CheckEq(toAcquire.size(), size_t{2}, "exactly two new ids to acquire on A→B swap");
  }

#ifdef BAKE_FIXTURE_DIR
  std::cout << "[bake: end-to-end against fixture project]\n";
  {
    const std::filesystem::path fixtureDir = BAKE_FIXTURE_DIR;
    const std::filesystem::path manifestPath = fixtureDir / "asset_manifest.lua";

    // Bake writes several artifacts next to the project (manifest + asset pak + atlas output dir).
    // The test owns all of them: wipe before and after so the source tree stays clean on CI and
    // re-runs start from the same pre-state.
    const auto CleanBakeOutputs = [&] {
      std::error_code ec;
      std::filesystem::remove(manifestPath, ec);
      std::filesystem::remove(fixtureDir / "asset_bundle.pak", ec);
      std::filesystem::remove_all(fixtureDir / "atlases", ec);
    };

    CleanBakeOutputs();

    const bool ok = Game::Bake(fixtureDir.string());
    Check(ok, "Game::Bake returns true on a clean fixture project");
    Check(std::filesystem::exists(manifestPath), "Game::Bake wrote asset_manifest.lua next to the project");

    CleanBakeOutputs();
  }
#else
  std::cerr << "  WARN BAKE_FIXTURE_DIR not defined; skipping Game::Bake check\n";
#endif

  return octarine::test::ReportSummary("Game bake test");
}
