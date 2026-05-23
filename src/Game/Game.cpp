#include "Game.h"

#include <SDL3/SDL.h>

#include <algorithm>
#include <memory>
#include <sol/sol.hpp>
#include <string>

#include "../Events/KeyInputEvent.h"
#include "../General/Logger.h"
#include "../Renderer/RenderQueue.h"
#include "../Renderer/Renderer.h"
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
#include "ECS/Iterable.h"
#include "ECS/Query.h"
#include "ECS/Registry.h"
#include "Events/MouseInputEvent.h"
#include "Events/MouseWheelEvent.h"
#include "GameConfig.h"
#include "General/PerfUtils.h"
#include "Lua/Bindings/InputSystemLuaBinding.h"
#include "Lua/Bindings/LuaSystemRegistry.h"
#include "Lua/Bindings/RegisterAllBindings.h"
#include "Lua/LuaApiManifest.h"
#include "Lua/LuaEntityLoader.h"
#include "Lua/Modules/RegisterAllModules.h"
#include "Systems/AnimationSystem.h"
#include "Systems/AudioSystem.h"
#include "Systems/CameraFollowSystem.h"
#include "Systems/CollisionSystem.h"
#include "Systems/DamageSystem.h"
#include "Systems/DrawColliderSystem.h"
#include "Systems/EntityPoolSystem.h"
#include "Systems/InputSystem.h"
#include "Systems/ObstacleBounceSystem.h"
#include "Systems/OffScreenDespawnSystem.h"
#include "Systems/ProjectileEmitSystem.h"
#include "Systems/ProjectileLifecycleSystem.h"
#include "Systems/RenderPrimitiveSystem.h"
#include "Systems/RenderSpriteSystem.h"
#include "Systems/RenderTextSystem.h"
#include "Systems/ScriptSystem.h"
#include "Systems/TransformSystem.h"
#include "Systems/UIButtonSystem.h"
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
#include "../Editor/Inspectors/RegisterAllInspectors.h"
#endif

constexpr Uint8 GREY_COLOR = 24;
constexpr Uint8 BLACK_COLOR = 0;

inline void LoadGame(sol::state& lua, const AssetManager& assetManager, const GameConfig& gameConfig)
{
    const auto filePath = assetManager.GetFullPath(gameConfig.GetStartupScript());

    Logger::Info("Loading entry script: " + filePath);
    sol::protected_function_result result;
    try
    {
        result = lua.safe_script_file(filePath);
    }
    catch (const std::exception& ex)
    {
        Logger::Error(ex.what());
        return;
    }

    if (!result.valid())
    {
        Logger::Error("Failed to load entry script: " + filePath);
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
    if (effectivePath.empty())
    {
        effectivePath = editorPersistence.lastProjectPath;
        if (!effectivePath.empty())
        {
            Logger::Info("No project path provided, attempting to load last project: " + effectivePath);
        }
    }
#endif

    bool projectLoaded = false;
    if (!effectivePath.empty())
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

    registry_->Set<SDL_Renderer*>(sdl_renderer_);
    registry_->Set<EventBus*>(event_bus_.get());

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
    const SDL_FRect camera(0, 0, static_cast<float>(gameConfig.windowWidth),
                           static_cast<float>(gameConfig.windowHeight));

    registry_->Set<RenderQueue>(RenderQueue());
    registry_->Set<CameraComponent>(CameraComponent{camera});
    registry_->Set<AssetManager>(AssetManager());

    // Gameplay
    auto& scriptSystem = registry_->RegisterSystem<ScriptComponent>(ScriptSystem());
    // InputSystem owned by the Registry so Lua bindings + event subscriptions outlive any single
    // scene script reload. Must be Set before CreateLuaBindings so the `input` table is installed
    // alongside ScriptSystem's globals.
    auto& inputSystem = registry_->Set<InputSystem>(InputSystem());
    inputSystem.SubscribeToEvents(event_bus_);

    // Component bindings (LuaBinding<T> specializations) must be registered BEFORE
    // ScriptSystem's CreateLuaBindings runs — usertypes + registry.has_/get_ are
    // populated from LuaComponentRegistry at that point.
    RegisterAllLuaBindings();

#ifdef OCTARINE_WITH_EDITOR
    // Editor inspector surface — declarative parallel to the Lua bindings.
    RegisterAllComponentInspectors();
#endif

    // Systems that expose a Lua surface register here; bindAll installs them all once
    // the sol::state has its libraries open.
    LuaSystemRegistry::clear();
    LuaSystemRegistry::registerSystem(inputSystem);

    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::io, sol::lib::string, sol::lib::table);
    // Snapshot stdlib globals before binding so the API manifest can diff out everything the
    // engine adds. Cheap; the actual dump only fires when OCTARINE_DUMP_LUA_API is set.
    const auto preBindingGlobals = LuaApiManifest::SnapshotGlobals(lua);

    // EntityPoolManager Set before ProjectileEmitSystem::Init — Init calls
    // Get<EntityPoolManager>().RegisterPool<...>. Was Set later (after audio); moved up to
    // satisfy the new init order.
    registry_->Set<EntityPoolManager>(EntityPoolManager());

    // ProjectileEmitSystem set + Init early so GameModule's fire_projectile can capture it from
    // the Registry during RegisterAllLuaModules. Init only does RegisterPool<...> calls — safe
    // to run before scene/system registration.
    auto& projectileEmitSystem = registry_->Set<ProjectileEmitSystem>(ProjectileEmitSystem());
    projectileEmitSystem.Init(*registry_);

    scriptSystem.CreateLuaBindings(lua);
    RegisterAllLuaModules(lua, *this);
    LuaSystemRegistry::bindAll(lua);
    lua["game_window_width"] = gameConfig.windowWidth;
    lua["game_window_height"] = gameConfig.windowHeight;
    lua["oct_startup_mode"] = startup_mode_;

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

    // Camera follows after gameplay-driven transform updates
    registry_->RegisterSystem<PositionComponent, CameraFollowComponent>(CameraFollowSystem());

    // Render queue producers
    registry_->RegisterParallelSystem<GlobalTransformComponent, SpriteComponent>(RenderSpriteSystem());
    registry_->RegisterSystem<TextLabelComponent>(RenderTextSystem());
    registry_->RegisterParallelSystem<SquarePrimitiveComponent, GlobalTransformComponent>(RenderPrimitiveSystem());

    // Event subscriptions (one-time)
    event_bus_->SubscribeEvent < Game, KeyInputEvent > (this, &Game::OnKeyInputEvent);
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
    if (auto* mixer = registry_->TryGet<MIX_Mixer*>())
    {
        const float masterGain = options.audioEnabled ? std::max(0.0F, options.masterVolume) : 0.0F;
        MIX_SetMixerGain(*mixer, masterGain);
    }

    auto* inputSystem = registry_->TryGet<InputSystem>();
    if (inputSystem)
    {
        inputSystem->BeginFrame();
    }

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

    StopScene();

#ifdef OCTARINE_WITH_EDITOR
    if (auto* editorPersistence = registry_->TryGet<EditorPersistence>())
    {
        editorPersistence->currentScenePath = scenePath;
        editorPersistence->showSceneWindow = true;
    }
#endif

    sol::protected_function_result result = lua.safe_script_file(fullPath);
    if (!result.valid())
    {
        const sol::error err = result;
        Logger::Error("Failed to load scene '" + fullPath + "': " + std::string(err.what()));
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
                auto* mixer = registry_->TryGet<MIX_Mixer*>();
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
                            if (mixer) am.AddAudioClip(*mixer, id, file);
                        }
                        assetCount++;
                    }
                }
                Logger::Info("Loaded " + std::to_string(assetCount) + " assets from scene table.");
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
            Logger::Info("Scene script returned a function. Calling it...");
            sol::function sceneFunc = result[0].as<sol::function>();
            auto funcResult = sceneFunc();
            if (!funcResult.valid())
            {
                sol::error err = funcResult;
                Logger::Error("Failed to run scene function: " + std::string(err.what()));
            }
        }
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

void Game::StopScene()
{
    Logger::Info("Stopping current scene (clearing entities).");
    registry_->ClearUserEntities();
    if (auto* inputSystem = registry_->TryGet<InputSystem>())
    {
        inputSystem->ResetLuaState();
    }
    scene_running_ = false;
}

KeyInputEvent Game::GetKeyInputEvent(SDL_KeyboardEvent* event)
{
    bool isPressed = event->down;
    return {event->key, event->mod, isPressed};
}
