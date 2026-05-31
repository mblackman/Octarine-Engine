#include "Game.h"

#include <SDL3/SDL.h>

#include <algorithm>
#include <filesystem>
#include <memory>
#include <set>
#include <sol/sol.hpp>
#include <string>
#include <unordered_set>

#include "../Events/KeyInputEvent.h"
#include "../General/Logger.h"
#include "AssetManager/AssetCatalog.h"
#include "AssetManager/AssetPak.h"
#include "AssetManager/AtlasBaker.h"
#include "AssetManager/AudioNormalizer.h"
#include "AssetManager/TextureAtlasBaker.h"
#include "AssetManager/SceneAssetScanner.h"
#include <SDL3_ttf/SDL_ttf.h>
#include "../Renderer/RenderQueue.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/SpriteRenderCache.h"
#include "Components/BoxColliderComponent.h"
#include "Components/CameraComponents.h"
#include "Components/EntityMaskComponent.h"
#include "Components/HealthComponent.h"
#include "Components/ProjectileComponent.h"
#include "Components/ProjectileEmitterComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/ScriptComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/SquarePrimitiveComponent.h"
#include "Components/GlobalTransformComponent.h"
#include "Components/PositionComponent.h"
#include "Components/RotationComponent.h"
#include "Components/ScaleComponent.h"
#include "Components/TextLabelComponent.h"
#include "Components/ViewportInfo.h"
#include "ECS/Iterable.h"
#include "ECS/Query.h"
#include "ECS/Registry.h"
#include "Engine/EngineBootstrap.h"
#include "Events/MouseInputEvent.h"
#include "Events/MouseWheelEvent.h"
#include "GameConfig.h"
#include "General/PerfUtils.h"
#include "General/Rect.h"
// InputSystemLuaBinding specializes LuaSystemBinding<InputSystem> — needed below where
// LuaSystemRegistry::registerSystem(inputSystem) instantiates the lookup. Don't drop.
#include "Lua/Bindings/InputSystemLuaBinding.h"
#include "Lua/Bindings/LuaSystemRegistry.h"
#include "Lua/HotReload/ScriptHotReload.h"
#include "Lua/LuaApiManifest.h"
#include "Lua/LuaEntityLoader.h"
#include "Components/AudioActiveTag.h"
#include "Components/AudioListenerComponent.h"
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
#include "Systems/RenderDebugGUISystem.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#endif

#ifdef OCTARINE_WITH_EDITOR
#include "../Editor/EditorLayoutPresets.h"
#include "../Editor/EditorPersistence.h"
#include "../Editor/ExportBuilder.h"
#include "../Editor/PlayerLauncher.h"
#endif

constexpr Uint8 GREY_COLOR = 24;
constexpr Uint8 BLACK_COLOR = 0;

namespace
{
    // Read a file's bytes through SDL_IO so the same path resolves on desktop, inside an APK asset
    // root, or inside a .app bundle. Lua's stock fopen-based loader only sees a real filesystem and
    // misses AAssetManager-backed entries on Android. Used by LoadGame + LoadScene; the dofile
    // override that exposes this to scripts lives in engine_bootstrap::InstallLuaLibraries.
    std::optional<std::string> ReadFileViaSDL(const std::string& path)
    {
        SDL_IOStream* io = SDL_IOFromFile(path.c_str(), "rb");
        if (!io)
        {
            Logger::Error("SDL_IOFromFile failed for '" + path + "': " + std::string(SDL_GetError()));
            return std::nullopt;
        }
        std::size_t size = 0;
        void* data = SDL_LoadFile_IO(io, &size, true); // closes io
        if (!data)
        {
            Logger::Error("SDL_LoadFile_IO failed for '" + path + "': " + std::string(SDL_GetError()));
            return std::nullopt;
        }
        std::string out(static_cast<const char*>(data), size);
        SDL_free(data);
        return out;
    }
} // namespace

inline void LoadGame(sol::state& lua, const AssetManager& assetManager, const GameConfig& gameConfig)
{
    const auto filePath = assetManager.GetFullPath(gameConfig.GetStartupScript());

    Logger::Info("Loading entry script: " + filePath);
    auto bytes = ReadFileViaSDL(filePath);
    if (!bytes)
    {
        Logger::Error("Failed to read entry script: " + filePath);
        return;
    }

    sol::protected_function_result result;
    try
    {
        result = lua.safe_script(*bytes, sol::script_pass_on_error, "@" + filePath);
    }
    catch (const std::exception& ex)
    {
        Logger::Error(ex.what());
        return;
    }

    if (!result.valid())
    {
        const sol::error err = result;
        Logger::Error("Failed to load entry script '" + filePath + "': " + err.what());
        return;
    }

    Logger::Info("Just opened entry script: " + filePath);
}

Game::Game() : window_(nullptr), sdl_renderer_(nullptr)
{
    registry_ = std::make_unique<Registry>();
    event_bus_ = std::make_unique<EventBus>();
    renderer_ = std::make_unique<Renderer>();
    Logger::Info("Game Constructor called.");
}

Game::~Game() { Logger::Info("Game Destructor called."); }

bool Game::Initialize(const std::string& assetPath)
{
    constexpr auto SDL_INI = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD;

    if (!SDL_Init(SDL_INI))
    {
        Logger::Error("SDL_Init Error: " + std::string(SDL_GetError()));
        return false;
    }

    if (!TTF_Init())
    {
        Logger::Error("TTF_Init Error: " + std::string(SDL_GetError()));
        return false;
    }

    registry_->Set<GameConfig>(GameConfig());
    auto& gameConfig = registry_->Get<GameConfig>();

    std::string effectivePath = assetPath;
#ifdef OCTARINE_WITH_EDITOR
    registry_->Set<EditorPersistence>(EditorPersistence());
    auto& editorPersistence = registry_->Get<EditorPersistence>();
    editorPersistence.LoadGlobal();
    registry_->Set<octarine::editor::PlayerLauncher>(octarine::editor::PlayerLauncher());
    registry_->Set<octarine::editor::ExportBuilder>(octarine::editor::ExportBuilder());
    if (effectivePath.empty())
    {
        effectivePath = editorPersistence.lastProjectPath;
        if (!effectivePath.empty())
        {
            Logger::Info("No project path provided, attempting to load last project: " + effectivePath);
        }
    }
#endif

    // No explicit path (and, in editor builds, no remembered project): default the asset base to the
    // executable/bundle location. On desktop this is the exe dir; in a .app it is Contents/Resources,
    // on iOS the bundle root. Keeps the same code path working for packaged builds with no -p argument.
    //
    // Android is the exception: assets live inside the APK, reached by SDL_OpenTitleStorage /
    // SDL_IOFromFile resolving *relative* paths against the APK asset root. SDL_GetBasePath() there
    // returns the app's internal data dir (not the APK), so the base path must stay empty and config
    // loading must still run against that empty base.
#ifndef __ANDROID__
    if (effectivePath.empty())
    {
        if (const char* basePath = SDL_GetBasePath())
        {
            effectivePath = basePath;
            Logger::Info("No project path provided; defaulting asset base path to: " + effectivePath);
        }
    }
#else
    Logger::Info("Android: resolving assets from the APK asset root (empty base path).");
#endif

    bool projectLoaded = false;
#ifdef __ANDROID__
    const bool attemptProjectLoad = true;  // empty base → APK title storage
#else
    const bool attemptProjectLoad = !effectivePath.empty();
#endif
    if (attemptProjectLoad)
    {
        if (gameConfig.LoadConfigFromFile(effectivePath))
        {
            gameConfig.LoadUserPreferences();
#ifdef OCTARINE_WITH_EDITOR
            editorPersistence.LoadProject(effectivePath);
            editorPersistence.lastProjectPath = effectivePath;
            editorPersistence.SaveGlobal();
#endif
            projectLoaded = true;
        }
        else
        {
            Logger::Warn("Failed to load project from path: " + effectivePath);
        }
    }

#ifdef OCTARINE_WITH_EDITOR
    // Editor-global audio prefs are authoritative for editor sessions, so apply them after the
    // per-project LoadUserPreferences above (which may have set masterVolume from preferences.ini).
    auto& audioOptions = gameConfig.GetEngineOptions();
    audioOptions.audioEnabled = !editorPersistence.audioMuted;
    audioOptions.masterVolume = editorPersistence.masterVolume;
#endif

#ifdef OCTARINE_WITH_EDITOR
    const bool defaultToEditor = startup_mode_.empty() || startup_mode_ == "editor";
#else
    if (startup_mode_ == "editor")
    {
        Logger::Error(
            "--startup-mode editor was requested but this is a player build (built without OCTARINE_WITH_EDITOR).");
        return false;
    }
    const bool defaultToEditor = false;
#endif

    if (!projectLoaded || defaultToEditor)
    {
        Logger::Info("Starting in Editor Mode.");
        gameConfig.SetIsEditorMode(true);
#ifdef OCTARINE_WITH_EDITOR
        // Open the editor with gameplay paused so the scene sits ready to inspect; the toolbar Play
        // button starts it. Player builds are unaffected (this branch only runs for editor sessions).
        gameConfig.GetEngineOptions().isPaused = true;
#endif
    }

    gameConfig.windowWidth = projectLoaded ? gameConfig.GetDefaultWidth() : Constants::kDefaultWindowWidth;
    gameConfig.windowHeight = projectLoaded ? gameConfig.GetDefaultHeight() : Constants::kDefaultWindowHeight;
    std::string title = projectLoaded ? gameConfig.GetGameTitle() : "Octarine Engine - Editor";

    SDL_CreateWindowAndRenderer(title.c_str(), gameConfig.windowWidth, gameConfig.windowHeight, SDL_WINDOW_RESIZABLE,
                                &window_, &sdl_renderer_);

    if (!window_)
    {
        Logger::Error("SDL_CreateWindow Error: " + std::string(SDL_GetError()));
        return false;
    }

    if (!sdl_renderer_)
    {
        Logger::Error("SDL_CreateRenderer Error: " + std::string(SDL_GetError()));
        return false;
    }

    game_render_texture_ = SDL_CreateTexture(sdl_renderer_, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                             gameConfig.windowWidth, gameConfig.windowHeight);

    if (!game_render_texture_)
    {
        Logger::Error("SDL_CreateTexture Error: " + std::string(SDL_GetError()));
        return false;
    }

#ifdef OCTARINE_WITH_IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    static std::string imguiIniPath;
    if (gameConfig.HasLoadedConfig())
    {
        imguiIniPath = gameConfig.GetAssetPath() + "/imgui.ini";
        io.IniFilename = imguiIniPath.c_str();
    }
    else
    {
        char* prefPath = SDL_GetPrefPath("Octarine", "Engine");
        if (prefPath)
        {
            imguiIniPath = std::string(prefPath) + "imgui_editor.ini";
            io.IniFilename = imguiIniPath.c_str();
            SDL_free(prefPath);
        }
        else
        {
            io.IniFilename = "imgui_editor.ini";
        }
    }

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplSDL3_InitForSDLRenderer(window_, sdl_renderer_);
    ImGui_ImplSDLRenderer3_Init(sdl_renderer_);

#ifdef OCTARINE_WITH_EDITOR
    // --- Resolve editor font size (DPI-aware default on first launch) ---
    // Roboto renders a bit smaller than ProggyClean at the same px, so we use 17
    // as the unscaled default rather than 16.
    float fontSize = editorPersistence.editorFontSize;
    if (fontSize <= 0.0F)
    {
        const SDL_DisplayID displayId = SDL_GetPrimaryDisplay();
        const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(displayId);
        float dpiScale = 1.0F;
        if (mode != nullptr && mode->pixel_density > 0.0F)
        {
            dpiScale = mode->pixel_density;
        }
        fontSize = 17.0F * dpiScale;
        editorPersistence.editorFontSize = fontSize;
    }
    RenderDebugGUISystem::RebuildEditorFont(fontSize);
    RenderDebugGUISystem::ApplyEditorStyle(editorPersistence.editorStyleIndex, fontSize);

    // First-run: if ImGui has no saved layout for this project / pref dir, apply
    // the bundled default so the user sees a curated workspace instead of a pile
    // of floating windows.
    if (!octarine::editor::layouts::HasImGuiIni())
    {
        octarine::editor::layouts::ApplyDefaultPreset(editorPersistence);
    }
#else
    io.Fonts->Clear();
    io.Fonts->AddFontDefault();
    io.Fonts->Build();
#endif
#endif

    SDL_SetRenderDrawColor(sdl_renderer_, GREY_COLOR, GREY_COLOR, GREY_COLOR, Constants::kUint8Max);

    // Populate the engine-level resource bundle now that SDL is up. AssetManager + mixer are
    // filled in by Setup once those exist; consumers read the same instance via
    // Registry::Get<EngineContext>().
    EngineContext ctx;
    ctx.sdlWindow = window_;
    ctx.sdlRenderer = sdl_renderer_;
    ctx.eventBus = event_bus_.get();
    ctx.config = &gameConfig;
    registry_->Set<EngineContext>(ctx);

    s_is_running_ = true;
    return true;
}

void Game::Destroy()
{
    if (registry_)
    {
        auto& gameConfig = registry_->Get<GameConfig>();
        gameConfig.SaveUserPreferences();
#ifdef OCTARINE_WITH_EDITOR
        if (auto* editorPersistence = registry_->TryGet<EditorPersistence>())
        {
            const auto& audioOptions = gameConfig.GetEngineOptions();
            editorPersistence->audioMuted = !audioOptions.audioEnabled;
            editorPersistence->masterVolume = audioOptions.masterVolume;
            editorPersistence->SaveGlobal();
            if (gameConfig.HasLoadedConfig())
            {
                editorPersistence->SaveProject(gameConfig.GetAssetPath());
            }
        }
#endif
    }

    // Tear registry/event bus down BEFORE SDL_Quit so owned systems (AudioSystem -> MIX_Quit,
    // AssetManager -> SDL_DestroyTexture) can still call into a live SDL.
    registry_.reset();
    event_bus_.reset();

    if (sdl_renderer_)
    {
        if (game_render_texture_)
        {
            SDL_DestroyTexture(game_render_texture_);
        }
#ifdef OCTARINE_WITH_IMGUI
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
#endif
        SDL_DestroyRenderer(sdl_renderer_);
    }

    if (window_)
    {
        SDL_DestroyWindow(window_);
    }

    SDL_Quit();
}

bool Game::Bake(const std::string& assetPath)
{
    if (assetPath.empty())
    {
        Logger::Error("Bake: no project path provided.");
        return false;
    }

    // Minimal init: GameConfig::LoadConfigFromFile and the engine's file reads (SDL_IOFromFile /
    // SDL_LoadFile) expect the library to be initialized. No video/audio subsystem is needed —
    // the bake validates references and paths only, with no GPU/mixer upload.
    if (!SDL_Init(0))
    {
        Logger::Error("Bake: SDL_Init failed: " + std::string(SDL_GetError()));
        return false;
    }

    bool ok = false;
    {
        Game game; // SDL-free constructor — allocates Registry/EventBus/Renderer, opens no window.
        game.bake_mode_ = true;
        game.startup_mode_ = "bake";
        ok = game.RunBakeValidation(assetPath);
    }

    SDL_Quit();
    return ok;
}

bool Game::RunBakeValidation(const std::string& assetPath)
{
    registry_->Set<GameConfig>(GameConfig());
    auto& gameConfig = registry_->Get<GameConfig>();
    if (!gameConfig.LoadConfigFromFile(assetPath))
    {
        Logger::Error("Bake: failed to load config from " + assetPath);
        return false;
    }

    // Bake mode: no window/renderer/mixer. Set the engine context first with the bits that do
    // exist; InstallCoreSingletons plumbs ctx.assets once it Sets the AssetManager.
    EngineContext bakeCtx;
    bakeCtx.eventBus = event_bus_.get();
    bakeCtx.config = &gameConfig;
    registry_->Set<EngineContext>(bakeCtx);

    // Shared bootstrap spine. withSpriteRenderCache=false — bake runs no frames so the render
    // cache slot is unused.
    engine_bootstrap::InstallCoreSingletons(*registry_, registry_->Get<EngineContext>(), gameConfig.windowWidth,
                                            gameConfig.windowHeight, /*withSpriteRenderCache=*/false);
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
    if (!assetManager.GetCatalog().ScanFilesystem(assetManager.GetBasePath(), lua, gameConfig))
    {
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
    if (!TextureAtlasBaker{}.Run(assetManager.GetCatalog(), assetPath))
    {
        Logger::Error("Bake: atlas packing failed. Aborting.");
        return false;
    }

    const std::string manifestPath = (std::filesystem::path(assetPath) / "asset_manifest.lua").string();
    const bool wrote = assetManager.GetCatalog().WriteManifest(manifestPath, assetPath);
    if (wrote)
    {
        Logger::Info("Bake: wrote " + std::to_string(assetManager.GetCatalog().Size()) + " entries to " +
            manifestPath);
    }

    // Glyph atlas pass (Stage 14 B3): for every Font catalog entry, rasterize ASCII printable
    // into a packed PNG + Lua metrics sidecar under `<basePath>/atlases/<asset_id>.atlas.{png,lua}`.
    // Runtime probes for those alongside the .ttf and renders from the atlas when present, falling
    // back to TTF_RenderText_Blended when missing. TTF needs explicit init in bake (Bake() opens
    // SDL_Init(0) only — no video/audio subsystems).
    std::vector<std::string> atlasFiles;
    if (wrote && TTF_Init())
    {
        const std::filesystem::path atlasDir = std::filesystem::path(assetPath) / "atlases";
        std::error_code ec;
        std::filesystem::create_directories(atlasDir, ec);
        const auto codepoints = AtlasBaker::DefaultAsciiPrintable();
        for (const auto& [id, entry] : assetManager.GetCatalog().Entries())
        {
            if (entry.type != AssetType::Font) continue;
            TTF_Font* font = TTF_OpenFont(entry.fullPath.c_str(), entry.fontSize);
            if (font == nullptr)
            {
                Logger::Warn("Bake: TTF_OpenFont failed for " + entry.fullPath + " @ " +
                             std::to_string(entry.fontSize) + " — skipping atlas.");
                continue;
            }
            const std::string pngPath = (atlasDir / (id + ".atlas.png")).string();
            const std::string luaPath = (atlasDir / (id + ".atlas.lua")).string();
            if (AtlasBaker::Bake(font, entry.fontSize, codepoints, pngPath, luaPath))
            {
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
    if (wrote)
    {
        const std::filesystem::path normRoot = std::filesystem::path(assetPath) / "normalized";
        for (const auto& [id, entry] : assetManager.GetCatalog().Entries())
        {
            if (entry.type != AssetType::Audio || !entry.normalize) continue;
            const std::filesystem::path src(entry.fullPath);
            const std::filesystem::path rel = std::filesystem::relative(src, std::filesystem::path(assetPath));
            const std::filesystem::path dst = normRoot / rel;
            std::error_code ec;
            std::filesystem::create_directories(dst.parent_path(), ec);
            if (AudioNormalizer::NormalizeWav(entry.fullPath, dst.string()))
            {
                audioOverrides[rel.generic_string()] = dst.string();
            }
        }
    }

    // Pack every cataloged asset (+ any derived atlas files, w/ optional normalized-audio
    // overrides) into a single asset_bundle.pak alongside the manifest. Shipped builds
    // (OCTARINE_SHIPPED) open this in place of the loose asset tree; dev builds ignore it.
    // Manifest write is the source-of-truth gate — if that failed, skip the pak so a stale
    // archive doesn't ship.
    if (wrote)
    {
        const std::string pakPath = (std::filesystem::path(assetPath) / "asset_bundle.pak").string();
        if (!AssetPak::Pack(assetManager.GetCatalog(), pakPath, assetPath, atlasFiles, audioOverrides))
        {
            Logger::Error("Bake: AssetPak::Pack failed for " + pakPath);
            return false;
        }
    }

    if (bake_validation_failures_ > 0)
    {
        Logger::Error("Bake: " + std::to_string(bake_validation_failures_) +
            " unresolved asset reference(s) across scenes. Build is broken.");
        return false;
    }
    return wrote;
}

void Game::Run()
{
    Setup();

    // Seed previous-frame tick so the first WaitTime call produces a sane delta.
    milliseconds_previous_frame_ = SDL_GetTicks();

    while (s_is_running_)
    {
        ProcessInput();
        const float deltaTime = WaitTime();
        Update(deltaTime);
        Render(deltaTime);
    }
}

void Game::Setup()
{
    auto& gameConfig = registry_->Get<GameConfig>();

    // Shared bootstrap spine (see engine_bootstrap/EngineBootstrap.h). withSpriteRenderCache=true
    // — the live game loop reads the cache from the parallel sprite-render pass.
    engine_bootstrap::InstallCoreSingletons(*registry_, registry_->Get<EngineContext>(), gameConfig.windowWidth,
                                            gameConfig.windowHeight, /*withSpriteRenderCache=*/true);

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

    if (startup_mode_ == "editor")
    {
        gameConfig.SetIsEditorMode(true);
    }

    if (IsBenchMode() && startup_mode_ != "editor")
    {
        // Bench runs shouldn't pay for debug overlays. Force-zero every toggle that would
        auto& options = gameConfig.GetEngineOptions();
        options.showDebugGUI = false;
#ifdef OCTARINE_WITH_IMGUI
        options.showFpsCounter = false;
        options.showEntityInfo = false;
#endif
#ifdef OCTARINE_WITH_EDITOR
        auto& bench_editor = registry_->Get<EditorPersistence>();
        bench_editor.showProfiler = false;
        bench_editor.showHierarchy = false;
        bench_editor.showAssetBrowser = false;
        bench_editor.showSceneWindow = false;
        bench_editor.showLuaConsole = false;
        bench_editor.showSceneManagement = false;
        options.showImGuiDemoWindow = false;
#endif
        options.drawColliders = false;
    }

    auto& assetManager = registry_->Get<AssetManager>();
    if (gameConfig.HasLoadedConfig())
    {
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
        if (allowManifest)
        {
            const std::string pakPath = (std::filesystem::path(assetManager.GetBasePath()) /
                                         "asset_bundle.pak").string();
            auto& pak = registry_->Set<AssetPak>(AssetPak());
            if (pak.Open(pakPath))
            {
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
    if (!audioSystem.Init(registry_.get(), event_bus_))
    {
        Logger::Error("AudioSystem failed to initialize; audio disabled this session.");
    }
    // Mixer is owned by AudioSystem; publish it on the context so asset loads + the master-
    // volume sync (in Game::Update) can reach it without going through the Registry's typed slot.
    registry_->Get<EngineContext>().mixer = audioSystem.Mixer();

    if (gameConfig.HasLoadedConfig())
    {
        LoadGame(lua, assetManager, gameConfig);
    }

    registry_->RegisterParallelSystem<SpriteComponent, AnimationComponent>(AnimationSystem());
    // ProjectileEmitSystem has no per-frame system pass anymore — gameplay drives shots via
    // fire_projectile(...) in Lua. The instance is Set + Init'd earlier in Setup so GameModule
    // can bind fire_projectile against it during module install.
    registry_->RegisterParallelSystem<ProjectileComponent>(ProjectileLifecycleSystem());

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
    // is the one that creates the AudioSinkComponent's MIX_Track that these mutate.
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
    registry_
        ->RegisterSystem<GlobalTransformComponent, RigidBodyComponent, AudioSourceComponent, AudioSinkComponent>(
            DopplerSystem());

    // Camera follows after gameplay-driven transform updates
    registry_->RegisterSystem<PositionComponent, CameraFollowComponent>(CameraFollowSystem());

    // Render queue producers
    registry_->RegisterParallelSystem<GlobalTransformComponent, SpriteComponent>(RenderSpriteSystem());
    registry_->RegisterSystem<TextLabelComponent>(RenderTextSystem());
    registry_->RegisterParallelSystem<SquarePrimitiveComponent, GlobalTransformComponent>(RenderPrimitiveSystem());

    // Event subscriptions (one-time)
    event_bus_->SubscribeEvent<Game, KeyInputEvent>(this, &Game::OnKeyInputEvent);
    // Event-driven systems with no per-frame Update — owned by the Registry instead of
    // living as parallel members on Game. Keeps the registry as the single source of truth.
    auto& uiButtonSystem = registry_->Set<UIButtonSystem>(UIButtonSystem());
    auto& damageSystem = registry_->Set<DamageSystem>(DamageSystem());
    auto& obstacleBounceSystem = registry_->Set<ObstacleBounceSystem>(ObstacleBounceSystem());
    uiButtonSystem.Init(registry_.get(), event_bus_);
    damageSystem.Init(registry_.get(), event_bus_);
    obstacleBounceSystem.Init(registry_.get(), event_bus_);

    // Pre-build the debug-collider query once so we don't allocate per render frame.
    collider_query_ = registry_->CreateQuery<GlobalTransformComponent, BoxColliderComponent>();

    // Hot reload owns its own ScriptWatcher + (path -> entities) discovery loop. Compiled out
    // under OCTARINE_SHIPPED; in dev/editor builds the runtime gate lives on EngineOptions.
    registry_->Set<ScriptHotReload>(ScriptHotReload());
}

void Game::ProcessInput() const
{
    PROFILE_NAMED_SCOPE("Game::ProcessInput");
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
#ifdef OCTARINE_WITH_IMGUI
        ImGui_ImplSDL3_ProcessEvent(&event);
        auto& io = ImGui::GetIO();
        float mouseX, mouseY;
        const unsigned int buttons = SDL_GetMouseState(&mouseX, &mouseY);
        io.MousePos = ImVec2(mouseX, mouseY);
        io.MouseDown[0] = buttons & SDL_BUTTON_MASK(SDL_BUTTON_LEFT);
        io.MouseDown[1] = buttons & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT);
#endif

        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            s_is_running_ = false;
            break;

        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP:
            {
                KeyInputEvent keyInputEvent = GetKeyInputEvent(&event.key);
                event_bus_->EmitEvent<KeyInputEvent>(keyInputEvent);
                break;
            }
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        case SDL_EVENT_MOUSE_BUTTON_UP:
            {
                SDL_MouseButtonEvent mouseButtonEvent = event.button;
                event_bus_->EmitEvent<MouseInputEvent>(mouseButtonEvent);
                break;
            }
        case SDL_EVENT_MOUSE_WHEEL:
            {
                event_bus_->EmitEvent<MouseWheelEvent>(event.wheel.x, event.wheel.y);
                break;
            }
        case SDL_EVENT_WINDOW_RESIZED:
        case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
            {
                auto& viewportInfo = registry_->Get<ViewportInfo>();
                int windowW, windowH;
                SDL_GetWindowSize(window_, &windowW, &windowH);
                viewportInfo.width = static_cast<float>(windowW);
                viewportInfo.height = static_cast<float>(windowH);
                break;
            }
        default:
            break;
        }
    }
}

void Game::Update(const float deltaTime)
{
#ifdef OCTARINE_PROFILING
    PerfUtils::ProfilingAccumulator::Clear();
    PerfUtils::PerfCounters::ResetValues();
#endif
    PROFILE_NAMED_SCOPE("Game::Update (total)");

    auto& options = registry_->Get<GameConfig>().GetEngineOptions();

    // Master volume + mute live at the mixer level so they apply to every track (including loops
    // already playing). Synced every frame — and outside the pause gate — so toggling mute reacts
    // immediately even while execution is paused.
    if (auto* mixer = registry_->Get<EngineContext>().mixer)
    {
        const float masterGain = options.audioEnabled ? std::max(0.0F, options.masterVolume) : 0.0F;
        MIX_SetMixerGain(mixer, masterGain);
    }

    auto* inputSystem = registry_->TryGet<InputSystem>();
    if (inputSystem)
    {
        inputSystem->BeginFrame();
    }

#ifndef OCTARINE_SHIPPED
    // Run hot reload even when paused — authors iterate code with the editor paused all the time,
    // and the swap is cheap. Uses real deltaTime (not time-scaled) so the poll cadence is stable.
    if (options.hotReloadEnabled)
    {
        if (auto* hotReload = registry_->TryGet<ScriptHotReload>())
        {
            hotReload->Tick(*registry_, lua, deltaTime, options.hotReloadPollSeconds);
        }
    }
#endif

    if (!options.isPaused || options.stepFrame)
    {
        registry_->Update(deltaTime * options.timeScale);
        options.stepFrame = false;
    }
    else
    {
        // If paused, we might still want to clear some per-frame signals so they don't get stuck.
    }

    // Pressed/released keys and wheel deltas are per-frame edge signals — clear after every
    // system in this frame has observed them.
    if (inputSystem)
    {
        inputSystem->ClearPerFrameInput();
    }
}

void Game::Render([[maybe_unused]] const float deltaTime)
{
    PROFILE_NAMED_SCOPE("Game::Render (total)");
    auto& renderQueue = registry_->Get<RenderQueue>();
    auto& gameConfig = registry_->Get<GameConfig>();

    PROFILE_COUNTER_SET("RenderQueue: Size", static_cast<long long>(renderQueue.Size()));
    PROFILE_COUNTER_SET("Entities: User", static_cast<long long>(registry_->GetUserEntityCount()));

    SDL_SetRenderTarget(sdl_renderer_, game_render_texture_);
    SDL_SetRenderDrawColor(sdl_renderer_, GREY_COLOR, GREY_COLOR, GREY_COLOR, Constants::kUint8Max);
    SDL_RenderClear(sdl_renderer_);

#ifdef OCTARINE_PROFILING
    {
        PerfUtils::ScopedTimer sortTimer("Render: Sort");
        renderQueue.Sort();
    }
    {
        PerfUtils::ScopedTimer drawTimer("Render: Draw");
        renderer_->Render(renderQueue, sdl_renderer_);
    }
#else
    renderQueue.Sort();
    renderer_->Render(renderQueue, sdl_renderer_);
#endif

    if (gameConfig.GetEngineOptions().drawColliders && collider_query_)
    {
        collider_query_->Update();
        DrawColliderSystem drawColliderSystem;
        collider_query_->ForEach(drawColliderSystem);
    }

    SDL_SetRenderTarget(sdl_renderer_, nullptr);
    SDL_SetRenderDrawColor(sdl_renderer_, BLACK_COLOR, BLACK_COLOR, BLACK_COLOR, Constants::kUint8Max);
    SDL_RenderClear(sdl_renderer_);

    auto& options = gameConfig.GetEngineOptions();
    const bool editorSession = gameConfig.IsEditorMode() || !gameConfig.HasLoadedConfig();

    // Update viewport info for non-editor sessions or when ImGui is disabled.
    // In editor mode with ImGui, RenderDebugGUISystem::Render will override this with the Scene window bounds.
    auto& viewportInfo = registry_->Get<ViewportInfo>();
    viewportInfo.x = 0;
    viewportInfo.y = 0;
    int windowW, windowH;
    SDL_GetWindowSize(window_, &windowW, &windowH);
    viewportInfo.width = static_cast<float>(windowW);
    viewportInfo.height = static_cast<float>(windowH);

#ifdef OCTARINE_WITH_IMGUI
    auto& io = ImGui::GetIO();
    viewportInfo.isHovered = !io.WantCaptureMouse;
    viewportInfo.isFocused = !io.WantCaptureKeyboard;
#else
    viewportInfo.isHovered = true;
    viewportInfo.isFocused = true;
#endif

    if (!IsBenchMode())
    {
        // Only draw the game texture to the full window if we are NOT in an editor session
        // and NOT showing debug overlays. In editor mode, the Scene window handles drawing this texture.
        if (!editorSession && !options.showDebugGUI)
        {
            SDL_RenderTexture(sdl_renderer_, game_render_texture_, nullptr, nullptr);
        }
#ifdef OCTARINE_WITH_IMGUI
        RenderDebugGUISystem::Render(this, sdl_renderer_, game_render_texture_, deltaTime);
#endif
    }

#ifdef OCTARINE_PROFILING
    {
        PerfUtils::ScopedTimer presentTimer("Render: Present");
        SDL_RenderPresent(sdl_renderer_);
    }
#else
    SDL_RenderPresent(sdl_renderer_);
#endif
    // Emit per-frame counters as COUNTER lines so headless bench runs can capture them
    // alongside TIMER lines. All systems for this frame have already written their values.
    PROFILE_COUNTERS_REPORT();
    renderQueue.Clear();
}

float Game::WaitTime()
{
    PROFILE_NAMED_SCOPE("Game::WaitTime");
    const Uint64 elapsedTime = SDL_GetTicks() - milliseconds_previous_frame_;
    if (elapsedTime < Constants::kMillisecondsPerFrame)
    {
        const Uint32 timeToWait = Constants::kMillisecondsPerFrame - static_cast<Uint32>(elapsedTime);
        SDL_Delay(timeToWait);
    }

    // Calculate delta time
    const auto intermediate = static_cast<double>(SDL_GetTicks() - milliseconds_previous_frame_) /
        static_cast<double>(Constants::kMillisecondsPerSecond);
    const auto deltaTime = static_cast<float>(intermediate);

    milliseconds_previous_frame_ = SDL_GetTicks();

    return deltaTime;
}

void Game::OnKeyInputEvent(const KeyInputEvent& event)
{
    if (!event.isPressed)
    {
        return;
    }

    auto& gameConfig = registry_->Get<GameConfig>();

    switch (event.inputKey)
    {
    case SDLK_ESCAPE:
        s_is_running_ = false;
        break;
    case SDLK_GRAVE:
        gameConfig.GetEngineOptions().showDebugGUI = !gameConfig.GetEngineOptions().showDebugGUI;
        break;
    default:
        break;
    }
}

void Game::LoadScene(const std::string& scenePath)
{
    if (scenePath.empty())
    {
        Logger::Warn("LoadScene called with empty path.");
        return;
    }

    auto& assetManager = registry_->Get<AssetManager>();

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
    if (auto* editorPersistence = registry_->TryGet<EditorPersistence>())
    {
        editorPersistence->currentScenePath = scenePath;
        editorPersistence->showSceneWindow = true;
    }
#endif

    auto sceneBytes = ReadFileViaSDL(fullPath);
    sol::protected_function_result result;
    if (sceneBytes)
    {
        result = lua.safe_script(*sceneBytes, sol::script_pass_on_error, "@" + fullPath);
    }
    if (!sceneBytes || !result.valid())
    {
        if (!sceneBytes)
        {
            Logger::Error("Failed to read scene '" + fullPath + "'");
        }
        else
        {
            const sol::error err = result;
            Logger::Error("Failed to load scene '" + fullPath + "': " + std::string(err.what()));
        }
        releasePrevious();
    }
    else if (result.return_count() > 0)
    {
        if (result[0].is<sol::table>())
        {
            Logger::Info("Scene script returned a table. Attempting to load assets and entities...");
            sol::table sceneTable = result[0];

            // 1. Load Assets
            sol::optional<sol::table> assets = sceneTable["assets"];
            if (assets && assets->valid())
            {
                auto& am = registry_->Get<AssetManager>();
                auto* mixer = registry_->Get<EngineContext>().mixer;
                int assetCount = 0;
                for (auto& [key, value] : *assets)
                {
                    if (value.is<sol::table>())
                    {
                        sol::table asset = value.as<sol::table>();
                        std::string type = asset["type"];
                        std::string id = asset["id"];
                        std::string file = asset["file"];
                        if (type == "texture")
                        {
                            am.AddTexture(sdl_renderer_, id, file);
                        }
                        else if (type == "font")
                        {
                            float fontSize = asset["font_size"];
                            am.AddFont(id, file, fontSize);
                        }
                        else if (type == "audio_clip")
                        {
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

                if (const int failures = am.Validate(refs); failures > 0)
                {
                    Logger::Error("Scene '" + fullPath + "' has " + std::to_string(failures) +
                        " unresolved asset reference(s).");
                    if (registry_->Get<GameConfig>().GetEngineOptions().assetValidationFatal)
                    {
                        Logger::Error("assetValidationFatal is set — aborting scene load.");
                        releasePrevious();
                        return;
                    }
                }

                const int acquired = am.AcquireAll(refs, sdl_renderer_, mixer);
                Logger::Info("Scene scan acquired " + std::to_string(acquired) + " required asset(s).");

                // New scene's assets are now resident. Track them, then release the previous
                // scene's set — ids shared by both stay loaded thanks to refcounting.
                std::vector<std::string> newSceneAssets;
                std::set<std::string> seen;
                for (const auto& ref : refs)
                {
                    if (seen.insert(ref.id).second) newSceneAssets.push_back(ref.id);
                }
                TrackSceneAssets(newSceneAssets);
                releasePrevious();
            }

            // 2. Load Entities
            sol::optional<sol::table> entities = sceneTable["entities"];
            if (entities && entities->valid())
            {
                int entityCount = 0;
                for (auto& [key, value] : *entities)
                {
                    if (value.is<sol::table>())
                    {
                        LuaEntityLoader::LoadEntityFromLua(registry_.get(), value.as<sol::table>());
                        entityCount++;
                    }
                }
                Logger::Info("Loaded " + std::to_string(entityCount) + " entities from scene table.");
            }

            // 3. Try to call a 'run' or 'load' or 'setup' function if present
            sol::optional<sol::function> runFunc = sceneTable["run"];
            if (!runFunc) runFunc = sceneTable["load"].get<sol::optional<sol::function>>();
            if (!runFunc) runFunc = sceneTable["setup"].get<sol::optional<sol::function>>();

            if (runFunc && runFunc->valid())
            {
                Logger::Info("Found 'run/load/setup' function in scene table. Calling it...");
                auto funcResult = (*runFunc)(sceneTable);
                if (!funcResult.valid())
                {
                    sol::error err = funcResult;
                    Logger::Error("Failed to run scene function: " + std::string(err.what()));
                }
            }
        }
        else if (result[0].is<sol::function>())
        {
            // No scene table to scan up front. The returned function spawns the scene (and may
            // call acquire_scene_assets, which tracks its ids via TrackSceneAssets). Run it, then
            // release the previous scene's assets — shared ids it already re-acquired survive.
            Logger::Info("Scene script returned a function. Calling it...");
            sol::function sceneFunc = result[0].as<sol::function>();
            auto funcResult = sceneFunc();
            if (!funcResult.valid())
            {
                sol::error err = funcResult;
                Logger::Error("Failed to run scene function: " + std::string(err.what()));
            }
            releasePrevious();
        }
        else
        {
            releasePrevious();
        }
    }
    else
    {
        // No return value: the script built the scene as a side effect while it ran (entities +
        // any acquire_scene_assets call). Those are already in place; just release the old set.
        releasePrevious();
    }

    scene_running_ = true;
}

void Game::ReloadScene()
{
#ifdef OCTARINE_WITH_EDITOR
    if (auto* editorPersistence = registry_->TryGet<EditorPersistence>(); editorPersistence != nullptr &&
        !editorPersistence->currentScenePath.empty())
    {
        LoadScene(editorPersistence->currentScenePath);
        return;
    }
#endif
    Logger::Warn("ReloadScene called but no scene is currently loaded.");
}

void Game::clearSceneEntities()
{
    registry_->ClearUserEntities();
    if (auto* inputSystem = registry_->TryGet<InputSystem>())
    {
        inputSystem->ResetLuaState();
    }
    // Drop cached SDL_Texture* lookups for entities that just got blammed; the next sprite-emit
    // pass repopulates as those entities are recreated by the new scene's load.
    if (auto* spriteCache = registry_->TryGet<SpriteRenderCache>())
    {
        spriteCache->Clear();
    }
}

void Game::TrackSceneAssets(const std::vector<std::string>& assetIds)
{
    for (const auto& id : assetIds)
    {
        if (std::ranges::find(current_scene_assets_, id) == current_scene_assets_.end())
        {
            current_scene_assets_.push_back(id);
        }
    }
}

void Game::StopScene()
{
    Logger::Info("Stopping current scene (clearing entities).");
    clearSceneEntities();

    // Release the assets this scene acquired. (LoadScene sequences acquire-before-release itself
    // and does not route through here; this is the explicit-stop path.)
    if (!current_scene_assets_.empty())
    {
        if (auto* assetManager = registry_->TryGet<AssetManager>())
        {
            assetManager->ReleaseAll(current_scene_assets_);
        }
        current_scene_assets_.clear();
    }

    scene_running_ = false;
}

KeyInputEvent Game::GetKeyInputEvent(SDL_KeyboardEvent* event)
{
    bool isPressed = event->down;
    return {event->key, event->mod, isPressed};
}
