#pragma once

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <string>
#include <unordered_set>

#include "General/FileWatcher.h"

class AssetManager;

#ifndef OCTARINE_SHIPPED

// Local-disk asset hot reload: the editor/player counterpart to ScriptHotReload, but for content
// instead of code. Owns a FileWatcher over the source files of every currently-resident asset
// (textures / fonts / audio clips) and, when one changes on disk, calls AssetManager::ReloadByPath
// to swap the underlying SDL_Texture* / TTF_Font* / MIX_Audio*. Consumers re-resolve for free:
// sprite renderers poll AssetManager::TextureGeneration(), and audio is looked up by id on each
// play, so no reload event is needed.
//
// One instance lives in the Registry singleton set; ticked from the main loop (FrameLoop::Update)
// behind the same EngineOptions.hotReloadEnabled gate the script reloader uses. Compiled out under
// OCTARINE_SHIPPED.
//
// Scope: this watches the loose source tree, so it is the dev/editor inner-loop path. The remote
// hot-push path (DevListenDispatch) drives the same AssetManager::ReloadByPath directly.
class AssetHotReload {
 public:
  struct ReloadInfo {
    std::string path;  // last reloaded source file (empty until first reload)
    int totalReloads = 0;
    int totalIdsReloaded = 0;
  };

  AssetHotReload() = default;

  // Called from the main loop. `dt` is real (non-time-scaled) time so paused editor sessions still
  // iterate. Re-syncs the watch set against the resident assets, then reloads any file whose mtime
  // advanced since the last poll.
  void Tick(AssetManager& assets, SDL_Renderer* renderer, MIX_Mixer* mixer, float dt, float pollSeconds);

  // Force a reload of every resident asset, ignoring mtime. Mirror of ScriptHotReload for the
  // dev-listen force-reload op.
  void ForceReloadAll(AssetManager& assets, SDL_Renderer* renderer, MIX_Mixer* mixer);

  [[nodiscard]] const ReloadInfo& LastReload() const { return last_; }

 private:
  void RefreshTracking(AssetManager& assets);

  FileWatcher watcher_;
  std::unordered_set<std::string> tracked_;  // canonical paths the watcher currently watches
  float accumulator_ = 0.0F;
  ReloadInfo last_;
};

#else

// Shipped stub.
class AssetHotReload {
 public:
  struct ReloadInfo {};
  void Tick(AssetManager&, SDL_Renderer*, MIX_Mixer*, float, float) {}
  void ForceReloadAll(AssetManager&, SDL_Renderer*, MIX_Mixer*) {}
  [[nodiscard]] ReloadInfo LastReload() const { return {}; }
};

#endif
