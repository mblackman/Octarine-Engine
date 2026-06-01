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

  [[nodiscard]] SDL_Renderer* GetRenderer() const override { return sdl_renderer_; }
  [[nodiscard]] SDL_Window* GetWindow() const { return window_; }

  [[nodiscard]] Registry* GetRegistry() const override { return registry_.get(); }
  [[nodiscard]] sol::state& GetLua() { return lua; }

  // Engine-level resource bundle (SDL handles, EventBus, AssetManager, GameConfig).
  // Lives as a Registry singleton — systems read it the same way via
  // `Registry::Get<EngineContext>()`. Game just forwards to the registry copy so
  // fields stay coherent across Initialize → Setup → AudioSystem::Init mutations.
  [[nodiscard]] EngineContext& GetContext() override { return registry_->Get<EngineContext>(); }
  [[nodiscard]] const EngineContext& GetContext() const { return registry_->Get<EngineContext>(); }

  void LoadScene(const std::string& scenePath) override;
  void ReloadScene() override;
  void StopScene() override;

  // Record asset ids acquired for the current scene so StopScene/the next LoadScene releases
  // them. Deduped against ids already tracked. Called by the C++ scene loader and by the
  // `acquire_scene_assets` Lua global (which serves scenes that load via a side-effect script
  // rather than returning a table). Safe to call repeatedly with overlapping sets.
  void TrackSceneAssets(const std::vector<std::string>& assetIds) override;

  // True between a successful LoadScene/ReloadScene and the next StopScene. The editor toolbar
  // uses this to decide whether Play should resume a paused scene or (re)start a stopped one.
  [[nodiscard]] bool IsSceneRunning() const { return scene_running_; }

 private:
  void ProcessInput() const;
  void Update(float deltaTime);
  void Render(float deltaTime);
  [[nodiscard]] float WaitTime();
  void Setup();
  // Headless instance method behind the static Bake(): wires the minimal singleton + Lua surface
  // the startup script touches, force-scans the catalog, runs the startup script (which validates
  // its scene references via the bake-mode asset globals), then writes the manifest. Returns false
  // on a load/scan/write failure or any unresolved reference.
  [[nodiscard]] bool RunBakeValidation(const std::string& assetPath);
  // Clear the current scene's user entities and reset per-scene Lua input state, without
  // touching acquired assets. Asset release is sequenced separately so a scene swap can acquire
  // the next scene's set before releasing the previous one (acquire-before-release).
  void clearSceneEntities();
  void OnKeyInputEvent(const KeyInputEvent& event);
  static KeyInputEvent GetKeyInputEvent(SDL_KeyboardEvent* event);

  SDL_Window* window_;
  SDL_Renderer* sdl_renderer_;
  static inline bool s_is_running_{false};
  bool scene_running_ = false;
  bool bake_mode_ = false;
  bool use_manifest_ = false;
  int bake_validation_failures_ = 0;
  Uint64 milliseconds_previous_frame_ = 0;
  // Asset ids acquired for the currently loaded scene. StopScene releases these; LoadScene
  // acquires the next scene's set first (acquire-before-release) so shared assets never churn.
  std::vector<std::string> current_scene_assets_;

  sol::state lua;
  std::string startup_mode_;
  std::unique_ptr<Registry> registry_;
  std::unique_ptr<EventBus> event_bus_;
  std::unique_ptr<Renderer> renderer_;
  std::unique_ptr<ComponentQuery<GlobalTransformComponent, BoxColliderComponent>> collider_query_;
};
