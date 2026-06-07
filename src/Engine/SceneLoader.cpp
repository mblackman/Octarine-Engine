#include "Engine/SceneLoader.h"

#include <SDL3_mixer/SDL_mixer.h>

#include <algorithm>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "AssetManager/AssetManager.h"
#include "AssetManager/SceneAssetScanner.h"
#include "Audio/AudioTrackCache.h"
#include "ECS/Registry.h"
#include "Engine/EngineContext.h"
#include "Engine/SdlFileReader.h"
#include "Game/GameConfig.h"
#include "General/Logger.h"
#include "Lua/LuaEntityLoader.h"
#include "Renderer/SpriteRenderCache.h"
#include "Systems/InputSystem.h"

#ifdef OCTARINE_WITH_EDITOR
#include "Editor/EditorPersistence.h"
#endif

void SceneLoader::FlushPendingSceneLoad() {
  // First flush also arms deferral: the startup script's initial load ran before any frame
  // (deferred_swaps_ still false) so it took effect immediately; from here, in-frame load_scene
  // calls queue instead. Bake never reaches a frame loop, so its loads stay immediate.
  deferred_swaps_ = true;
  if (!has_pending_scene_) return;
  has_pending_scene_ = false;
  const std::string path = std::move(pending_scene_path_);
  pending_scene_path_.clear();
  // Drop deferral for the duration of the swap so LoadScene runs its body instead of re-queuing,
  // then re-arm for subsequent in-frame callers.
  deferred_swaps_ = false;
  LoadScene(path);
  deferred_swaps_ = true;
}

void SceneLoader::LoadScene(const std::string& scenePath) {
  if (scenePath.empty()) {
    Logger::Warn("LoadScene called with empty path.");
    return;
  }

  // Deferred path: a UIButton on_click (or any in-frame caller) only queues the swap; the frame loop
  // flushes it before systems run (FlushPendingSceneLoad). Last request within a frame wins. See
  // header for the why.
  if (deferred_swaps_) {
    pending_scene_path_ = scenePath;
    has_pending_scene_ = true;
    return;
  }

  auto& assetManager = registry_->Get<AssetManager>();
  SDL_Renderer* renderer = registry_->Get<EngineContext>().sdlRenderer;

  // Use the full path relative to the asset directory if it's a relative path.
  std::string fullPath = assetManager.GetFullPath(scenePath);

  Logger::Info("Loading scene: " + fullPath);

  // Acquire-before-release: stash the previous scene's tracked assets but defer releasing them
  // until the new scene's assets are resident, so any id shared by both scenes never
  // unloads/reloads across the swap. Entities are a different story — a scene script may spawn
  // entities as a side effect while it runs (the no-table path), so the old entities are cleared
  // up front, before the script executes, rather than after (which would wipe the new ones).
  std::vector<std::string> previousSceneAssets = std::move(current_scene_assets_);
  current_scene_assets_.clear();
  clearSceneEntities();

  auto releasePrevious = [&]() { assetManager.ReleaseAll(previousSceneAssets); };

#ifdef OCTARINE_WITH_EDITOR
  if (auto* editorPersistence = registry_->TryGet<EditorPersistence>()) {
    editorPersistence->currentScenePath = scenePath;
    editorPersistence->showSceneWindow = true;
  }
#endif

  auto sceneBytes = ReadFileViaSDL(fullPath);
  sol::protected_function_result result;
  if (sceneBytes) {
    result = lua_.safe_script(*sceneBytes, sol::script_pass_on_error, "@" + fullPath);
  }
  if (!sceneBytes || !result.valid()) {
    if (!sceneBytes) {
      Logger::Error("Failed to read scene '" + fullPath + "'");
    } else {
      const sol::error err = result;
      Logger::Error("Failed to load scene '" + fullPath + "': " + std::string(err.what()));
    }
    releasePrevious();
  } else if (result.return_count() > 0) {
    if (result[0].is<sol::table>()) {
      Logger::Info("Scene script returned a table. Attempting to load assets and entities...");
      sol::table sceneTable = result[0];

      // 1. Load Assets
      sol::optional<sol::table> assets = sceneTable["assets"];
      if (assets && assets->valid()) {
        auto& am = registry_->Get<AssetManager>();
        auto* mixer = registry_->Get<EngineContext>().mixer;
        int assetCount = 0;
        for (auto& [key, value] : *assets) {
          if (value.is<sol::table>()) {
            sol::table asset = value.as<sol::table>();
            std::string type = asset["type"];
            std::string id = asset["id"];
            std::string file = asset["file"];
            if (type == "texture") {
              am.AddTexture(renderer, id, file);
            } else if (type == "font") {
              float fontSize = asset["font_size"];
              am.AddFont(id, file, fontSize);
            } else if (type == "audio_clip") {
              if (mixer) am.AddAudioClip(mixer, id, file);
            }
            assetCount++;
          }
        }
        Logger::Info("Loaded " + std::to_string(assetCount) + " assets from scene table.");
      }

      // 1b. Derive the scene's required asset set from its data (sprite/font/audio ids +
      // tilemap + preload), validate every reference against the catalog, then acquire up
      // front before entities load.
      {
        auto& am = registry_->Get<AssetManager>();
        MIX_Mixer* mixer = registry_->Get<EngineContext>().mixer;
        const std::vector<AssetReference> refs = SceneAssetScanner::CollectRefs(sceneTable);

        if (const int failures = am.Validate(refs); failures > 0) {
          Logger::Error("Scene '" + fullPath + "' has " + std::to_string(failures) + " unresolved asset reference(s).");
          if (registry_->Get<GameConfig>().GetEngineOptions().assetValidationFatal) {
            Logger::Error("assetValidationFatal is set — aborting scene load.");
            releasePrevious();
            return;
          }
        }

        const int acquired = am.AcquireAll(refs, renderer, mixer);
        Logger::Info("Scene scan acquired " + std::to_string(acquired) + " required asset(s).");

        // New scene's assets are now resident. Track them, then release the previous
        // scene's set — ids shared by both stay loaded thanks to refcounting.
        std::vector<std::string> newSceneAssets;
        std::set<std::string> seen;
        for (const auto& ref : refs) {
          if (seen.insert(ref.id).second) newSceneAssets.push_back(ref.id);
        }
        TrackSceneAssets(newSceneAssets);
        releasePrevious();
      }

      // 2. Load Entities
      sol::optional<sol::table> entities = sceneTable["entities"];
      if (entities && entities->valid()) {
        int entityCount = 0;
        for (auto& [key, value] : *entities) {
          if (value.is<sol::table>()) {
            LuaEntityLoader::LoadEntityFromLua(registry_, value.as<sol::table>());
            entityCount++;
          }
        }
        Logger::Info("Loaded " + std::to_string(entityCount) + " entities from scene table.");
      }

      // 3. Try to call a 'run' or 'load' or 'setup' function if present
      sol::optional<sol::function> runFunc = sceneTable["run"];
      if (!runFunc) runFunc = sceneTable["load"].get<sol::optional<sol::function>>();
      if (!runFunc) runFunc = sceneTable["setup"].get<sol::optional<sol::function>>();

      if (runFunc && runFunc->valid()) {
        Logger::Info("Found 'run/load/setup' function in scene table. Calling it...");
        auto funcResult = (*runFunc)(sceneTable);
        if (!funcResult.valid()) {
          sol::error err = funcResult;
          Logger::Error("Failed to run scene function: " + std::string(err.what()));
        }
      }
    } else if (result[0].is<sol::function>()) {
      // No scene table to scan up front. The returned function spawns the scene (and may
      // call acquire_scene_assets, which tracks its ids via TrackSceneAssets). Run it, then
      // release the previous scene's assets — shared ids it already re-acquired survive.
      Logger::Info("Scene script returned a function. Calling it...");
      sol::function sceneFunc = result[0].as<sol::function>();
      auto funcResult = sceneFunc();
      if (!funcResult.valid()) {
        sol::error err = funcResult;
        Logger::Error("Failed to run scene function: " + std::string(err.what()));
      }
      releasePrevious();
    } else {
      releasePrevious();
    }
  } else {
    // No return value: the script built the scene as a side effect while it ran (entities +
    // any acquire_scene_assets call). Those are already in place; just release the old set.
    releasePrevious();
  }

  scene_running_ = true;
}

void SceneLoader::ReloadScene() {
#ifdef OCTARINE_WITH_EDITOR
  if (auto* editorPersistence = registry_->TryGet<EditorPersistence>();
      editorPersistence != nullptr && !editorPersistence->currentScenePath.empty()) {
    LoadScene(editorPersistence->currentScenePath);
    return;
  }
#endif
  Logger::Warn("ReloadScene called but no scene is currently loaded.");
}

void SceneLoader::clearSceneEntities() {
  // Stop every playing track before the emitter entities vanish. AudioTrackCache.Clear() below only
  // drops the entity->track map — it does not halt playback, so a looping source (loop=true) would
  // keep sounding into the next scene and, because AcquireTrack only reuses !MIX_TrackPlaying slots,
  // its pool track would never be reclaimed. MIX_StopAllTracks frees them on both counts.
  if (MIX_Mixer* mixer = registry_->Get<EngineContext>().mixer) {
    MIX_StopAllTracks(mixer, 0);
  }

  registry_->ClearUserEntities();
  if (auto* inputSystem = registry_->TryGet<InputSystem>()) {
    inputSystem->ResetLuaState();
  }
  // Drop cached SDL_Texture* lookups for entities that just got blammed; the next sprite-emit
  // pass repopulates as those entities are recreated by the new scene's load.
  if (auto* spriteCache = registry_->TryGet<SpriteRenderCache>()) {
    spriteCache->Clear();
  }
  // Same for cached MIX_Track* handles — the just-blammed emitters' sinks are gone; AudioSystem
  // re-acquires + re-caches tracks for the new scene's emitters as they play.
  if (auto* trackCache = registry_->TryGet<AudioTrackCache>()) {
    trackCache->Clear();
  }
}

void SceneLoader::TrackSceneAssets(const std::vector<std::string>& assetIds) {
  for (const auto& id : assetIds) {
    if (std::ranges::find(current_scene_assets_, id) == current_scene_assets_.end()) {
      current_scene_assets_.push_back(id);
    }
  }
}

void SceneLoader::StopScene() {
  Logger::Info("Stopping current scene (clearing entities).");
  clearSceneEntities();

  // Release the assets this scene acquired. (LoadScene sequences acquire-before-release itself
  // and does not route through here; this is the explicit-stop path.)
  if (!current_scene_assets_.empty()) {
    if (auto* assetManager = registry_->TryGet<AssetManager>()) {
      assetManager->ReleaseAll(current_scene_assets_);
    }
    current_scene_assets_.clear();
  }

  scene_running_ = false;
}
