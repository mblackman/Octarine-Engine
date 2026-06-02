#pragma once

#include <SDL3/SDL.h>

#include <memory>
#include <sol/sol.hpp>
#include <string>
#include <vector>

#include "../Components/BoxColliderComponent.h"
#include "../Components/GlobalTransformComponent.h"
#include "../ECS/Query.h"
#include "../Engine/EngineContext.h"
#include "../Engine/EngineRuntime.h"
#include "../Engine/FrameLoop.h"
#include "../Engine/SceneLoader.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyInputEvent.h"
#include "../Lua/LuaBindingContext.h"
#include "../Renderer/Renderer.h"

class Registry;

class Game : public LuaBindingContext {
 public:
  Game();

  Game(const Game&) = delete;
  Game& operator=(const Game&) = delete;

  Game(Game&&) = delete;
  Game& operator=(Game&&) = delete;

  ~Game();

  bool Initialize(const std::string& assetPath);
  void Destroy();
  void Run();

  // Headless asset bake (startup mode "bake"): scan the project at `assetPath` for assets, run its
  // startup script to derive every referenced asset id (validating each against the catalog), and
  // emit a scan-free `asset_manifest.lua` (relative paths) next to it. Creates no window, renderer,
  // audio, or game loop. Returns false (→ nonzero process exit, a CI gate) on a config/catalog/write
  // failure or any unresolved asset reference. Constructs its own headless Game internally.
  [[nodiscard]] static bool Bake(const std::string& assetPath);

  // True while running under the headless bake (`-m bake`). Asset-loading Lua globals
  // (`acquire_scene_assets`, `load_asset`) consult this to validate-and-count referenced ids
  // instead of uploading to a GPU/mixer that does not exist in this mode.
  [[nodiscard]] bool IsBakeMode() const override { return bake_mode_; }

  // Accumulate unresolved-reference failures found while a scene script runs under bake. Bake
  // sums these across the whole startup run and fails the process if any remain.
  void RecordBakeValidationFailures(int failures) override { bake_validation_failures_ += failures; }
  static void Quit() { s_is_running_ = false; }

  // LuaBindingContext shutdown hook (the `quit_game` global) — forwards to the static flag.
  void RequestQuit() override { Quit(); }

  // Optional startup-mode hint exposed to Lua as `oct_startup_mode`. Empty string means
  // "no override" — the startup script can show its normal menu. Set by Main from the
  // --startup-mode flag; consumed by the game's bootstrap script. A non-empty mode also
  // suppresses debug UI rendering so benchmarks don't pay that overhead.
  void SetStartupMode(const std::string& mode) { startup_mode_ = mode; }
  [[nodiscard]] bool IsBenchMode() const { return !startup_mode_.empty() && startup_mode_ != "editor"; }

  // Dev override (--use-manifest) letting a non-shipped binary load asset_manifest.lua instead of
  // scanning — the only way to exercise the manifest-load branch without producing a full package.
  // In a shipped build (OCTARINE_SHIPPED) the manifest is used unconditionally and this is moot.
  void SetUseManifest(bool useManifest) { use_manifest_ = useManifest; }

#ifndef OCTARINE_SHIPPED
  // Dev-only: opt into the DevListenServer (Stage 6 of EditorBuildAndDeployPlan). port=0 leaves
  // the server stopped. listenAll=true binds 0.0.0.0; default 127.0.0.1. Called by Main from
  // --dev-listen / --dev-listen-all and stripped from shipped binaries.
  void SetDevListen(int port, bool listenAll) {
    dev_listen_port_ = port;
    dev_listen_all_ = listenAll;
  }
#endif

  [[nodiscard]] SDL_Renderer* GetRenderer() const override { return runtime_.SdlRenderer(); }
  [[nodiscard]] SDL_Window* GetWindow() const { return runtime_.Window(); }

  [[nodiscard]] Registry* GetRegistry() const override { return registry_.get(); }
  [[nodiscard]] sol::state& GetLua() { return lua; }

  // Engine-level resource bundle (SDL handles, EventBus, AssetManager, GameConfig).
  // Lives as a Registry singleton — systems read it the same way via
  // `Registry::Get<EngineContext>()`. Game just forwards to the registry copy so
  // fields stay coherent across Initialize → Setup → AudioSystem::Init mutations.
  [[nodiscard]] EngineContext& GetContext() override { return registry_->Get<EngineContext>(); }
  [[nodiscard]] const EngineContext& GetContext() const { return registry_->Get<EngineContext>(); }

  // Scene lifecycle lives in SceneLoader; these LuaBindingContext overrides delegate so the
  // `scene.*` Lua module (and the editor toolbar) drive it through Game unchanged.
  void LoadScene(const std::string& scenePath) override { scene_loader_->LoadScene(scenePath); }
  void ReloadScene() override { scene_loader_->ReloadScene(); }
  void StopScene() override { scene_loader_->StopScene(); }
  void TrackSceneAssets(const std::vector<std::string>& assetIds) override {
    scene_loader_->TrackSceneAssets(assetIds);
  }

  // True between a successful LoadScene/ReloadScene and the next StopScene. The editor toolbar
  // uses this to decide whether Play should resume a paused scene or (re)start a stopped one.
  [[nodiscard]] bool IsSceneRunning() const { return scene_loader_->IsSceneRunning(); }

 private:
  void Setup();
  // Headless instance method behind the static Bake(): wires the minimal singleton + Lua surface
  // the startup script touches, force-scans the catalog, runs the startup script (which validates
  // its scene references via the bake-mode asset globals), then writes the manifest. Returns false
  // on a load/scan/write failure or any unresolved reference.
  [[nodiscard]] bool RunBakeValidation(const std::string& assetPath);

  EngineRuntime runtime_;
  static inline bool s_is_running_{false};
  bool bake_mode_ = false;
  bool use_manifest_ = false;
#ifndef OCTARINE_SHIPPED
  int dev_listen_port_ = 0;
  bool dev_listen_all_ = false;
#endif
  int bake_validation_failures_ = 0;

  sol::state lua;
  std::string startup_mode_;
  std::unique_ptr<Registry> registry_;
  std::unique_ptr<EventBus> event_bus_;
  std::unique_ptr<Renderer> renderer_;
  // Scene + frame helpers. Constructed in the Game ctor once registry_/renderer_ exist; they hold
  // non-owning refs back into Game's members, so they are declared last and torn down first.
  std::unique_ptr<SceneLoader> scene_loader_;
  std::unique_ptr<FrameLoop> frame_loop_;
};
