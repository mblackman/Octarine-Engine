#include "Game.h"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <algorithm>
#include <filesystem>
#include <memory>
#include <set>
#include <sol/sol.hpp>
#include <string>
#include <unordered_set>

#include "AssetManager/AssetCatalog.h"
#include "AssetManager/AssetHotReload.h"
#include "AssetManager/AssetPak.h"
#include "AssetManager/AtlasBaker.h"
#include "AssetManager/AudioNormalizer.h"
#include "AssetManager/SceneAssetScanner.h"
#include "AssetManager/TextureAtlasBaker.h"
#include "Components/BoxColliderComponent.h"
#include "Components/CameraComponents.h"
#include "Components/EntityMaskComponent.h"
#include "Components/GlobalTransformComponent.h"
#include "Components/HealthComponent.h"
#include "Components/PositionComponent.h"
#include "Components/ProjectileComponent.h"
#include "Components/ProjectileEmitterComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/RotationComponent.h"
#include "Components/ScaleComponent.h"
#include "Components/ScriptComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/SquarePrimitiveComponent.h"
#include "Components/TextLabelComponent.h"
#include "Components/ViewportInfo.h"
#include "ECS/Iterable.h"
#include "ECS/Query.h"
#include "ECS/Registry.h"
#include "Engine/EditorBootstrap.h"
#include "Engine/EngineBootstrap.h"
#include "Engine/Platform/PlatformPaths.h"
#include "Engine/SdlFileReader.h"
#include "Events/KeyInputEvent.h"
#include "Events/MouseInputEvent.h"
#include "Events/MouseWheelEvent.h"
#include "GameConfig.h"
#include "General/Logger.h"
#include "General/PerfUtils.h"
#include "General/Rect.h"
#include "Renderer/RenderQueue.h"
#include "Renderer/Renderer.h"
#include "Renderer/SpriteRenderCache.h"
// InputSystemLuaBinding specializes LuaSystemBinding<InputSystem> — needed below where
// LuaSystemRegistry::registerSystem(inputSystem) instantiates the lookup. Don't drop.
#include "Components/AudioActiveTag.h"
#include "Components/AudioListenerComponent.h"
#include "Lua/Bindings/InputSystemLuaBinding.h"
#include "Lua/Bindings/LuaSystemRegistry.h"
#include "Lua/HotReload/ScriptHotReload.h"
#include "Lua/LuaApiManifest.h"
#include "Lua/LuaEntityLoader.h"
#include "Systems/AnimationSystem.h"
#include "Systems/AudioCullingSystem.h"
#include "Systems/AudioSystem.h"
#include "Systems/CameraFollowSystem.h"
#include "Systems/CollisionSystem.h"
#include "Systems/DamageSystem.h"
#include "Systems/DopplerSystem.h"
#include "Systems/DrawColliderSystem.h"
#include "Systems/InputSystem.h"
#include "Systems/ObstacleBounceSystem.h"
#include "Systems/OffScreenDespawnSystem.h"
#include "Systems/ProjectileEmitSystem.h"
#include "Systems/ProjectileLifecycleSystem.h"
#include "Systems/RenderPrimitiveSystem.h"
#include "Systems/RenderSpriteSystem.h"
#include "Systems/RenderTextSystem.h"
#include "Systems/ScriptSystem.h"
#include "Systems/SpatialAudioSystem.h"
#include "Systems/TransformSystem.h"
#include "Systems/UIButtonSystem.h"
#include "Systems/UpdateListenerTransformSystem.h"
#include "Systems/VelocityIntegrationSystem.h"

#ifdef OCTARINE_WITH_IMGUI
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#endif

#ifndef OCTARINE_SHIPPED
#include "Dev/DevListenDispatch.h"
#include "Dev/DevListenServer.h"
#endif

inline void LoadGame(sol::state& lua, const AssetManager& assetManager, const GameConfig& gameConfig) {
  const auto filePath = assetManager.GetFullPath(gameConfig.GetStartupScript());

  Logger::Info("Loading entry script: " + filePath);
  auto bytes = ReadFileViaSDL(filePath);
  if (!bytes) {
    Logger::Error("Failed to read entry script: " + filePath);
    return;
  }

  sol::protected_function_result result;
  try {
    result = lua.safe_script(*bytes, sol::script_pass_on_error, "@" + filePath);
  } catch (const std::exception& ex) {
    Logger::Error(ex.what());
    return;
  }

  if (!result.valid()) {
    const sol::error err = result;
    Logger::Error("Failed to load entry script '" + filePath + "': " + err.what());
    return;
  }

  Logger::Info("Just opened entry script: " + filePath);
}

Game::Game() {
  registry_ = std::make_unique<Registry>();
  event_bus_ = std::make_unique<EventBus>();
  renderer_ = std::make_unique<Renderer>();
  scene_loader_ = std::make_unique<SceneLoader>(registry_.get(), lua);
#ifndef OCTARINE_SHIPPED
  // Expose the scene loader as a Registry singleton so the dev-iteration listener's reload_scene
  // op can reach it from its main-thread dispatch context. Game still owns the instance; this is a
  // non-owning pointer handle.
  registry_->Set<SceneLoader*>(scene_loader_.get());
#endif
  frame_loop_ = std::make_unique<FrameLoop>(this, registry_.get(), event_bus_.get(), renderer_.get(), &runtime_, lua);
  // Generational GC collects the short-lived objects that dominate game-loop Lua allocations
  // (per-frame script tables, vectors, closures) cheaply in minor cycles, reserving full sweeps
  // for objects that actually survive. This reduces the p99 frame spikes caused by GC pauses.
  lua_gc(lua.lua_state(), LUA_GCGEN, 0, 0);
  Logger::Info("Game Constructor called.");
}

Game::~Game() { Logger::Info("Game Destructor called."); }

bool Game::Initialize(const std::string& assetPath) {
  if (!runtime_.InitSubsystems()) {
    return false;
  }

  registry_->Set<GameConfig>(GameConfig());
  auto& gameConfig = registry_->Get<GameConfig>();

  StartDevListenServer();

  std::string effectivePath = assetPath;
  // Editor builds Set their editor singletons (persistence/launcher/export) and may default
  // effectivePath to the last-opened project here; player builds no-op. See EditorBootstrap.
  engine_bootstrap::editor::InstallSingletons(*registry_, effectivePath);

  // With no explicit path (and, in editor builds, no remembered project) default the asset base to
  // the executable/bundle dir on desktop; Android keeps it empty so assets resolve against the APK
  // asset root. The Android/desktop fork lives in PlatformPaths, not here.
  engine_bootstrap::platform::ApplyDefaultBasePath(effectivePath);

  bool projectLoaded = false;
  if (engine_bootstrap::platform::ShouldAttemptProjectLoad(effectivePath)) {
    if (gameConfig.LoadConfigFromFile(effectivePath)) {
      gameConfig.LoadUserPreferences();
      engine_bootstrap::editor::OnProjectLoaded(*registry_, effectivePath);
      projectLoaded = true;
    } else {
      Logger::Warn("Failed to load project from path: " + effectivePath);
    }
  }

  // Editor-global audio prefs (mute + master volume) layered over the per-project preferences.
  engine_bootstrap::editor::ApplyAudioPrefs(*registry_);

  // Editor builds default into editor mode (unless a non-editor mode was requested); player builds
  // never do and reject `--startup-mode editor` (ok=false → abort).
  const auto startupDecision = engine_bootstrap::editor::DecideStartupMode(startup_mode_);
  if (!startupDecision.ok) {
    return false;
  }

  if (!projectLoaded || startupDecision.defaultToEditor) {
    Logger::Info("Starting in Editor Mode.");
    gameConfig.SetIsEditorMode(true);
    engine_bootstrap::editor::PauseForEditorSession(*registry_);
  }

  if (!CreateWindowAndScene(projectLoaded)) {
    return false;
  }

  InitImGuiBackend();

  // Populate the engine-level resource bundle now that SDL is up. AssetManager + mixer are
  // filled in by Setup once those exist; consumers read the same instance via
  // Registry::Get<EngineContext>().
  EngineContext ctx;
  ctx.sdlWindow = runtime_.Window();
  ctx.sdlRenderer = runtime_.SdlRenderer();
  ctx.eventBus = event_bus_.get();
  ctx.config = &gameConfig;
  registry_->Set<EngineContext>(ctx);

  s_is_running_ = true;
  return true;
}

void Game::InitImGuiBackend() {
#ifdef OCTARINE_WITH_IMGUI
  auto& gameConfig = registry_->Get<GameConfig>();
  // io.IniFilename holds the raw pointer for the context's lifetime, so the backing string must
  // outlive InitImGui — keep it static.
  static std::string imguiIniPath;
  if (gameConfig.HasLoadedConfig()) {
    imguiIniPath = gameConfig.GetAssetPath() + "/imgui.ini";
  } else if (char* prefPath = SDL_GetPrefPath("Octarine", "Engine")) {
    imguiIniPath = std::string(prefPath) + "imgui_editor.ini";
    SDL_free(prefPath);
  } else {
    imguiIniPath = "imgui_editor.ini";
  }
  runtime_.InitImGui(imguiIniPath.c_str());

  // Editor: bundled Roboto at the persisted size + theme + the default layout on first run.
  // Player-with-ImGui: the built-in default font. The editor/player fork lives in EditorBootstrap.
  engine_bootstrap::editor::SetupEditorImGui(*registry_);
#endif
}

void Game::StartDevListenServer() {
#ifndef OCTARINE_SHIPPED
  // DevListenServer: TCP listener on 127.0.0.1:<port> when --dev-listen given. Compile-stripped
  // from shipped builds. Available in editor + player presets alike so the standalone player can
  // host the dev-iterate loop too.
  auto& devListen = registry_->Set<octarine::dev::DevListenServer>(octarine::dev::DevListenServer());
  // Install the engine handler so parked eval/reload/push ops apply on the main thread in Pump().
  devListen.SetCommandHandler(octarine::dev::MakeEngineCommandHandler(*registry_, lua));
  if (dev_listen_port_ > 0) {
    octarine::dev::ServerOptions opts;
    opts.port = static_cast<std::uint16_t>(dev_listen_port_);
    opts.listen_all = dev_listen_all_;
    if (!devListen.Start(opts)) {
      Logger::Error("DevListenServer failed to start; --dev-listen flag had no effect.");
    }
  }
#endif
}

bool Game::CreateWindowAndScene(bool projectLoaded) {
  auto& gameConfig = registry_->Get<GameConfig>();
  gameConfig.windowWidth = projectLoaded ? gameConfig.GetDefaultWidth() : Constants::kDefaultWindowWidth;
  gameConfig.windowHeight = projectLoaded ? gameConfig.GetDefaultHeight() : Constants::kDefaultWindowHeight;
  const std::string title = projectLoaded ? gameConfig.GetGameTitle() : "Octarine Engine - Editor";

  if (!runtime_.CreateWindow(title, gameConfig.windowWidth, gameConfig.windowHeight)) {
    return false;
  }
  return renderer_->CreateScene(runtime_.SdlRenderer(), gameConfig.windowWidth, gameConfig.windowHeight);
}

void Game::Destroy() {
  if (registry_) {
#ifndef OCTARINE_SHIPPED
    // Stop the dev listener before the registry teardown drops it so the listener thread
    // joins cleanly while the engine is still alive.
    if (auto* devListen = registry_->TryGet<octarine::dev::DevListenServer>()) {
      devListen->Stop();
    }
#endif

    auto& gameConfig = registry_->Get<GameConfig>();
    gameConfig.SaveUserPreferences();
    // Persist editor prefs (audio + per-project layout); no-op in player builds.
    engine_bootstrap::editor::SaveOnShutdown(*registry_);
  }

  // Tear registry/event bus down BEFORE SDL_Quit so owned systems (AudioSystem -> MIX_Quit,
  // AssetManager -> SDL_DestroyTexture) can still call into a live SDL.
  registry_.reset();
  event_bus_.reset();

  // Free the off-screen scene target while the SDL renderer is still live (EngineRuntime owns
  // the ImGui backend + window/renderer destroy + SDL_Quit, in that order).
  if (runtime_.SdlRenderer()) {
    renderer_->DestroyScene();
  }
  runtime_.Shutdown();
}

bool Game::Bake(const std::string& assetPath) {
  if (assetPath.empty()) {
    Logger::Error("Bake: no project path provided.");
    return false;
  }

  // Minimal init: GameConfig::LoadConfigFromFile and the engine's file reads (SDL_IOFromFile /
  // SDL_LoadFile) expect the library to be initialized. No video/audio subsystem is needed —
  // the bake validates references and paths only, with no GPU/mixer upload.
  if (!SDL_Init(0)) {
    Logger::Error("Bake: SDL_Init failed: " + std::string(SDL_GetError()));
    return false;
  }

  bool ok = false;
  {
    Game game;  // SDL-free constructor — allocates Registry/EventBus/Renderer, opens no window.
    game.bake_mode_ = true;
    game.startup_mode_ = "bake";
    ok = game.RunBakeValidation(assetPath);
  }

  SDL_Quit();
  return ok;
}

bool Game::RunBakeValidation(const std::string& assetPath) {
  registry_->Set<GameConfig>(GameConfig());
  auto& gameConfig = registry_->Get<GameConfig>();
  if (!gameConfig.LoadConfigFromFile(assetPath)) {
    Logger::Error("Bake: failed to load config from " + assetPath);
    return false;
  }

  // Bake mode: no window/renderer/mixer. Set the engine context first with the bits that do
  // exist; InstallCoreSingletons plumbs ctx.assets once it Sets the AssetManager.
  EngineContext bakeCtx;
  bakeCtx.eventBus = event_bus_.get();
  bakeCtx.config = &gameConfig;
  registry_->Set<EngineContext>(bakeCtx);

  // Shared bootstrap spine. withFramePathCaches=false — bake runs no frames and no audio systems,
  // so the SpriteRenderCache / AudioTrackCache slots are unused.
  engine_bootstrap::InstallCoreSingletons(*registry_, registry_->Get<EngineContext>(), gameConfig.windowWidth,
                                          gameConfig.windowHeight, /*withFramePathCaches=*/false);
  engine_bootstrap::InstallPoolAndProjectile(*registry_);
  auto& inputSystem = engine_bootstrap::InstallInputSystem(*registry_, event_bus_);
  engine_bootstrap::RegisterAllComponentBindings();
  LuaSystemRegistry::clear();
  LuaSystemRegistry::registerSystem(inputSystem);
  engine_bootstrap::InstallLuaLibraries(lua);
  // Bake doesn't register ScriptSystem as a per-frame system (no frames). A stack instance
  // is enough to install the primitive + component usertypes Lua scripts reference.
  ScriptSystem scriptSystem;
  scriptSystem.CreateLuaBindings(lua);
  engine_bootstrap::InstallLuaModules(lua, *this);
  LuaSystemRegistry::bindAll(lua);
  engine_bootstrap::SetCommonLuaGlobals(lua, gameConfig, startup_mode_);

  auto& assetManager = registry_->Get<AssetManager>();
  assetManager.LoadGameConfig(gameConfig);

  // Always re-derive the catalog from the real files — never round-trip an existing (possibly
  // stale) manifest sitting in the project dir.
  if (!assetManager.GetCatalog().ScanFilesystem(assetManager.GetBasePath(), lua, gameConfig)) {
    Logger::Error("Bake: catalog scan failed (id collisions). Aborting.");
    return false;
  }
  assetManager.GetCatalog().InstallLuaIdTable(lua);

  // Run the startup script. Its scene loads call acquire_scene_assets / load_asset, which in bake
  // mode validate every referenced id against the catalog and tally failures on this Game instead
  // of uploading to a GPU. This is the CI gate: a typo'd or missing asset reference fails the bake.
  LoadGame(lua, assetManager, gameConfig);

  // Pack atlases declared by `meta.atlas = <group>` sidecars into <project>/_atlases/<group>.png
  // and mutate member entries to record their slice within each. Runs after scene load so a
  // future `meta.atlas` injection from scene scripts could also be honored. Members tagged
  // `meta.no_atlas = true` skip the pack and ship as loose textures.
  if (!TextureAtlasBaker{}.Run(assetManager.GetCatalog(), assetPath)) {
    Logger::Error("Bake: atlas packing failed. Aborting.");
    return false;
  }

  const std::string manifestPath = (std::filesystem::path(assetPath) / "asset_manifest.lua").string();
  const bool wrote = assetManager.GetCatalog().WriteManifest(manifestPath, assetPath);
  if (wrote) {
    Logger::Info("Bake: wrote " + std::to_string(assetManager.GetCatalog().Size()) + " entries to " + manifestPath);
  }

  // Glyph atlas pass (Stage 14 B3): for every Font catalog entry, rasterize ASCII printable
  // into a packed PNG + Lua metrics sidecar under `<basePath>/atlases/<asset_id>.atlas.{png,lua}`.
  // Runtime probes for those alongside the .ttf and renders from the atlas when present, falling
  // back to TTF_RenderText_Blended when missing. TTF needs explicit init in bake (Bake() opens
  // SDL_Init(0) only — no video/audio subsystems).
  std::vector<std::string> atlasFiles;
  if (wrote && TTF_Init()) {
    const std::filesystem::path atlasDir = std::filesystem::path(assetPath) / "atlases";
    std::error_code ec;
    std::filesystem::create_directories(atlasDir, ec);
    const auto codepoints = AtlasBaker::DefaultAsciiPrintable();
    for (const auto& [id, entry] : assetManager.GetCatalog().Entries()) {
      if (entry.type != AssetType::Font) continue;
      TTF_Font* font = TTF_OpenFont(entry.fullPath.c_str(), entry.fontSize);
      if (font == nullptr) {
        Logger::Warn("Bake: TTF_OpenFont failed for " + entry.fullPath + " @ " + std::to_string(entry.fontSize) +
                     " — skipping atlas.");
        continue;
      }
      const std::string pngPath = (atlasDir / (id + ".atlas.png")).string();
      const std::string luaPath = (atlasDir / (id + ".atlas.lua")).string();
      if (AtlasBaker::Bake(font, entry.fontSize, codepoints, pngPath, luaPath)) {
        atlasFiles.push_back(pngPath);
        atlasFiles.push_back(luaPath);
      }
      TTF_CloseFont(font);
    }
    TTF_Quit();
  }

  // Audio normalize pass (Stage 14 B2): for every Audio catalog entry with `meta.normalize=true`,
  // run BS.1770 integrated loudness measurement + apply the gain to land at -16 LUFS, writing
  // the normalized WAV to `<basePath>/normalized/<rel>.wav`. The pak override map then routes
  // the catalog's original relPath to read bytes from the normalized variant — so shipped
  // builds carry the loudness-aligned audio while dev tree (and dev runs) stay untouched.
  std::map<std::string, std::string> audioOverrides;
  if (wrote) {
    const std::filesystem::path normRoot = std::filesystem::path(assetPath) / "normalized";
    for (const auto& [id, entry] : assetManager.GetCatalog().Entries()) {
      if (entry.type != AssetType::Audio || !entry.normalize) continue;
      const std::filesystem::path src(entry.fullPath);
      const std::filesystem::path rel = std::filesystem::relative(src, std::filesystem::path(assetPath));
      const std::filesystem::path dst = normRoot / rel;
      std::error_code ec;
      std::filesystem::create_directories(dst.parent_path(), ec);
      if (AudioNormalizer::NormalizeWav(entry.fullPath, dst.string())) {
        audioOverrides[rel.generic_string()] = dst.string();
      }
    }
  }

  // Pack every cataloged asset (+ any derived atlas files, w/ optional normalized-audio
  // overrides) into a single asset_bundle.pak alongside the manifest. Shipped builds
  // (OCTARINE_SHIPPED) open this in place of the loose asset tree; dev builds ignore it.
  // Manifest write is the source-of-truth gate — if that failed, skip the pak so a stale
  // archive doesn't ship.
  if (wrote) {
    const std::string pakPath = (std::filesystem::path(assetPath) / "asset_bundle.pak").string();
    if (!AssetPak::Pack(assetManager.GetCatalog(), pakPath, assetPath, atlasFiles, audioOverrides)) {
      Logger::Error("Bake: AssetPak::Pack failed for " + pakPath);
      return false;
    }
  }

  if (bake_validation_failures_ > 0) {
    Logger::Error("Bake: " + std::to_string(bake_validation_failures_) +
                  " unresolved asset reference(s) across scenes. Build is broken.");
    return false;
  }
  return wrote;
}

void Game::Run() {
  Setup();

  // Seed previous-frame tick so the first WaitTime call produces a sane delta.
  frame_loop_->Begin();

  while (s_is_running_) {
    frame_loop_->ProcessInput();
    const float deltaTime = frame_loop_->WaitTime();
    frame_loop_->Update(deltaTime);
    frame_loop_->Render(deltaTime);
  }
}

void Game::Setup() {
  auto& gameConfig = registry_->Get<GameConfig>();

  // Shared bootstrap spine (see engine_bootstrap/EngineBootstrap.h). withFramePathCaches=true —
  // the live game loop reads SpriteRenderCache from the sprite-render pass and AudioTrackCache
  // from the spatial/Doppler/culling audio systems.
  engine_bootstrap::InstallCoreSingletons(*registry_, registry_->Get<EngineContext>(), gameConfig.windowWidth,
                                          gameConfig.windowHeight, /*withFramePathCaches=*/true);

  // ScriptSystem is registered as a per-frame system (vs. Bake's stack instance) so its
  // operator() runs each tick. CreateLuaBindings still happens below in the bootstrap spine.
  auto& scriptSystem = registry_->RegisterSystem<ScriptComponent>(ScriptSystem());

  auto& inputSystem = engine_bootstrap::InstallInputSystem(*registry_, event_bus_);
  engine_bootstrap::RegisterAllComponentBindings();
  LuaSystemRegistry::clear();
  LuaSystemRegistry::registerSystem(inputSystem);
  engine_bootstrap::InstallLuaLibraries(lua);
  // Snapshot stdlib globals before binding so the API manifest can diff out everything the
  // engine adds. Cheap; the actual dump only fires when OCTARINE_DUMP_LUA_API is set.
  const auto preBindingGlobals = LuaApiManifest::SnapshotGlobals(lua);
  engine_bootstrap::InstallPoolAndProjectile(*registry_);
  scriptSystem.CreateLuaBindings(lua);
  engine_bootstrap::InstallLuaModules(lua, *this);
  LuaSystemRegistry::bindAll(lua);
  engine_bootstrap::SetCommonLuaGlobals(lua, gameConfig, startup_mode_);

  // Emit the EmmyLua API stub when requested (no-op otherwise). Runs once per Setup, after the
  // full surface — globals, modules, system tables — is installed.
  LuaApiManifest::MaybeDumpFromEnv(lua, preBindingGlobals);

  if (startup_mode_ == "editor") {
    gameConfig.SetIsEditorMode(true);
  }

  if (IsBenchMode() && startup_mode_ != "editor") {
    // Bench runs shouldn't pay for debug overlays. Force-zero every toggle that would add per-frame
    // work. These EngineOptions fields exist in every build (and read as no-ops without ImGui); the
    // editor-only window toggles move into EditorBootstrap (no-op in player builds).
    auto& options = gameConfig.GetEngineOptions();
    options.showDebugGUI = false;
    options.showFpsCounter = false;
    options.showEntityInfo = false;
    options.showImGuiDemoWindow = false;
    options.drawColliders = false;
    engine_bootstrap::editor::DisableBenchOverlays(*registry_);
  }

  auto& assetManager = registry_->Get<AssetManager>();
  if (gameConfig.HasLoadedConfig()) {
    assetManager.LoadGameConfig(gameConfig);

    // Discover every available asset + its sidecar metadata up front. Loads nothing — the
    // catalog is the index Acquire loads from. Built before LoadGame so the startup
    // script and scene loads can resolve ids against it.
    //
    // Dev-vs-shipped gate: a shipped build (OCTARINE_SHIPPED) loads the baked manifest and skips
    // the scan — required inside a read-only bundle. A dev build always scans (live files are the
    // truth) unless --use-manifest is passed to exercise the manifest branch from a dev binary.
    bool allowManifest = use_manifest_;
#ifdef OCTARINE_SHIPPED
    allowManifest = true;
#endif
    assetManager.GetCatalog().Build(assetManager.GetBasePath(), lua, gameConfig, allowManifest);
    assetManager.GetCatalog().DumpToLog();

    // Shipped builds (and a dev build with --use-manifest) probe for an asset_bundle.pak
    // beside the manifest. When present, AssetManager reads every asset out of it instead of
    // the loose tree — same code paths, different SDL_IOStream source. Absence is fine:
    // dev/bootstrap projects haven't baked one yet.
    if (allowManifest) {
      const std::string pakPath = (std::filesystem::path(assetManager.GetBasePath()) / "asset_bundle.pak").string();
      auto& pak = registry_->Set<AssetPak>(AssetPak());
      if (pak.Open(pakPath)) {
        assetManager.SetAssetPak(&pak);
      }
    }

    // Expose the global `assets` table (id -> id, raises on typo) so scripts can reference
    // ids safely before LoadGame runs the startup script.
    assetManager.GetCatalog().InstallLuaIdTable(lua);
  }

  // Audio must be live before LoadGame: the startup script may call load_asset for audio_clip
  // entries, which need the mixer. RegisterSystem owns the single AudioSystem instance — Init
  // runs against that instance so the EventBus subscription, cmd buffer, and per-frame Update
  // all share state.
  auto& audioSystem = registry_->RegisterSystem<AudioSourceComponent>(AudioSystem());
  if (!audioSystem.Init(registry_.get(), event_bus_)) {
    Logger::Error("AudioSystem failed to initialize; audio disabled this session.");
  }
  // Mixer is owned by AudioSystem; publish it on the context so asset loads + the master-
  // volume sync (in Game::Update) can reach it without going through the Registry's typed slot.
  registry_->Get<EngineContext>().mixer = audioSystem.Mixer();

  if (gameConfig.HasLoadedConfig()) {
    LoadGame(lua, assetManager, gameConfig);
  }

  // The startup script's first load_scene ran immediately above so frame 1 has a populated scene.
  // From here on, in-game load_scene/reload_scene (e.g. a UIButton on_click) defer to the top of
  // the next frame so the swap never runs inside an event dispatch or system ForEach.
  scene_loader_->EnableDeferredSwaps();

  registry_->RegisterParallelSystem<SpriteComponent, AnimationComponent>(AnimationSystem());
  registry_->RegisterParallelSystem<ProjectileComponent>(ProjectileLifecycleSystem());

  // Engine-side auto-fire: tick emitters whose frequency > 0 and spawn a projectile on expiry.
  // This re-introduces the fixed-interval emit that auto_fire.lua had moved into per-entity Lua;
  // running it in C++ removes one sol2 call per shooter per frame (ScriptSystem was ~56% of Update
  // at stress scale). fire_projectile(...) from Lua still works for scripted/aimed firing — those
  // emitters keep frequency 0 and are skipped here, so the two paths never double-fire one emitter.
  //
  // Serial (RegisterSystem, not parallel): the tick spawns into the registry. It is registered
  // here, after InstallPoolAndProjectile Set + Init'd the canonical ProjectileEmitSystem, so the
  // copy stored by the system list carries the initialized pool id. Placed before
  // VelocityIntegrationSystem so freshly-spawned projectiles integrate/transform/collide this same
  // frame — matching the spawn-timing the old ScriptSystem-driven Lua path had.
  registry_->RegisterSystem<ProjectileEmitterComponent, PositionComponent>(registry_->Get<ProjectileEmitSystem>());

  // Integrate velocity into local position. Must run before TransformSystem so the hierarchy
  // resolves with the latest positions, and before CollisionSystem reads the global transforms.
  registry_->RegisterParallelSystem<PositionComponent, RigidBodyComponent>(VelocityIntegrationSystem());

  // Despawn entities (except the player) once they leave the playable area.
  registry_->RegisterParallelSystem<PositionComponent, SpriteComponent>(OffScreenDespawnSystem());

  // Resolve hierarchy after Movement mutates local positions, before Collision reads globals.
  registry_->RegisterBulkSystem<GlobalTransformComponent>(TransformSystem());

  // Collision reads transform.globalPosition / globalScale — must run after TransformSystem.
  registry_->RegisterBulkSystem(CollisionSystem());

  // Spatial audio: snapshot the listener entity (UpdateListenerTransformSystem) then mutate
  // gain + stereo pan on live spatial tracks (SpatialAudioSystem). Both must run AFTER
  // TransformSystem so emitter + listener globals are this-frame values; AudioSystem (above)
  // is the one that adds the AudioSinkComponent and caches its MIX_Track in AudioTrackCache,
  // which these resolve per entity.
  registry_->Set<AudioListenerCache>(AudioListenerCache{});
  registry_->RegisterBulkSystem<GlobalTransformComponent>(UpdateListenerTransformSystem());
  // Phase 5 culling: pre-register the AudioActiveTag so its component entity exists
  // before any HasTag query fires (Tag<T>() is lazy otherwise). CullingSystem then gates
  // SpatialAudioSystem / DopplerSystem implicitly — culled emitters lose their sink, and
  // both downstream systems short-circuit on the missing sink.
  registry_->Tag<AudioActiveTag>();
  registry_->RegisterSystem<GlobalTransformComponent, AudioSourceComponent>(AudioCullingSystem());
  registry_->RegisterSystem<GlobalTransformComponent, AudioSourceComponent, AudioSinkComponent>(SpatialAudioSystem());
  // Doppler is independent of spatial: governs frequency ratio via MIX_SetTrackFrequencyRatio.
  // Requires RigidBodyComponent on the emitter (no body → no velocity → no shift).
  registry_->RegisterSystem<GlobalTransformComponent, RigidBodyComponent, AudioSourceComponent, AudioSinkComponent>(
      DopplerSystem());

  // Camera follows after gameplay-driven transform updates
  registry_->RegisterSystem<PositionComponent, CameraFollowComponent>(CameraFollowSystem());

  // Render queue producers
  registry_->RegisterParallelSystem<GlobalTransformComponent, SpriteComponent>(RenderSpriteSystem());
  registry_->RegisterSystem<TextLabelComponent>(RenderTextSystem());
  registry_->RegisterParallelSystem<SquarePrimitiveComponent, GlobalTransformComponent>(RenderPrimitiveSystem());

  // Event subscriptions (one-time). FrameLoop holds its own RAII subscription handle.
  frame_loop_->SubscribeToEvents();
  // Event-driven systems with no per-frame Update — owned by the Registry instead of
  // living as parallel members on Game. Keeps the registry as the single source of truth.
  auto& uiButtonSystem = registry_->Set<UIButtonSystem>(UIButtonSystem());
  auto& damageSystem = registry_->Set<DamageSystem>(DamageSystem());
  auto& obstacleBounceSystem = registry_->Set<ObstacleBounceSystem>(ObstacleBounceSystem());
  uiButtonSystem.Init(registry_.get(), event_bus_);
  damageSystem.Init(registry_.get(), event_bus_);
  obstacleBounceSystem.Init(registry_.get(), event_bus_);

  // Hot reload owns its own FileWatcher + (path -> entities) discovery loop. Compiled out
  // under OCTARINE_SHIPPED; in dev/editor builds the runtime gate lives on EngineOptions.
  registry_->Set<ScriptHotReload>(ScriptHotReload());
  // Asset hot reload: same FileWatcher infra, watching resident texture/font/audio source files
  // and swapping the live SDL/MIX handles via AssetManager::ReloadByPath when they change on disk.
  registry_->Set<AssetHotReload>(AssetHotReload());
}
