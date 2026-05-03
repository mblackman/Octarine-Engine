#include "Game.h"

#include <SDL3/SDL.h>

#include <glm/glm.hpp>
#include <memory>
#include <sol/sol.hpp>
#include <string>

#include "../Events/KeyInputEvent.h"
#include "../General/Logger.h"
#include "../Renderer/RenderQueue.h"
#include "../Renderer/Renderer.h"
#include "Components/BoxColliderComponent.h"
#include "Components/CameraComponents.h"
#include "Components/CameraFollowComponent.h"
#include "Components/EntityMaskComponent.h"
#include "Components/HealthComponent.h"
#include "Components/KeyboardControlComponent.h"
#include "Components/ProjectileComponent.h"
#include "Components/ProjectileEmitterComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/ScriptComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/SquarePrimitiveComponent.h"
#include "Components/TextLabelComponent.h"
#include "Components/TransformComponent.h"
#include "ECS/Iterable.h"
#include "ECS/Query.h"
#include "ECS/Registry.h"
#include "Events/MouseInputEvent.h"
#include "GameConfig.h"
#include "Systems/AnimationSystem.h"
#include "Systems/CameraFollowSystem.h"
#include "Systems/CollisionSystem.h"
#include "Systems/DamageSystem.h"
#include "Systems/DisplayHealthSystem.h"
#include "Systems/DrawColliderSystem.h"
#include "Systems/KeyboardControlSystem.h"
#include "Systems/MovementSystem.h"
#include "Systems/ProjectileEmitSystem.h"
#include "Systems/ProjectileLifecycleSystem.h"
#include "Systems/RenderDebugGUISystem.h"
#include "Systems/RenderPrimitiveSystem.h"
#include "Systems/RenderSpriteSystem.h"
#include "Systems/RenderTextSystem.h"
#include "Systems/ScriptSystem.h"
#include "Systems/TransformSystem.h"
#include "General/PerfUtils.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

constexpr Uint8 GREY_COLOR = 24;

inline void LoadGame(sol::state &lua, const AssetManager &assetManager, const GameConfig &gameConfig) {
  const auto filePath = assetManager.GetFullPath(gameConfig.GetStartupScript());

  Logger::Info("Loading entry script: " + filePath);
  sol::protected_function_result result;
  try {
    result = lua.safe_script_file(filePath);
  } catch (const std::exception &ex) {
    Logger::Error(ex.what());
    return;
  }

  if (!result.valid()) {
    Logger::Error("Failed to load entry script: " + filePath);
    return;
  }

  Logger::Info("Just opened entry script: " + filePath);
}

Game::Game() : window_(nullptr), sdl_renderer_(nullptr) {
  registry_ = std::make_unique<Registry>();
  event_bus_ = std::make_unique<EventBus>();
  renderer_ = std::make_unique<Renderer>();
  Logger::Info("Game Constructor called.");
}

Game::~Game() { Logger::Info("Game Destructor called."); }

bool Game::Initialize(const std::string &assetPath) {
  constexpr auto SDL_INI = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD;

  if (!SDL_Init(SDL_INI)) {
    Logger::Error("SDL_Init Error: " + std::string(SDL_GetError()));
    return false;
  }

  if (!TTF_Init()) {
    Logger::Error("TTF_Init Error: " + std::string(SDL_GetError()));
    return false;
  }

  registry_->Set<GameConfig>(GameConfig());
  auto &gameConfig = registry_->Get<GameConfig>();

  if (!gameConfig.LoadConfigFromFile(assetPath)) {
    Logger::Error("Failed to load game config.");
    return false;
  }

  gameConfig.windowWidth = gameConfig.GetDefaultWidth();
  gameConfig.windowHeight = gameConfig.GetDefaultHeight();
  SDL_CreateWindowAndRenderer(gameConfig.GetGameTitle().c_str(), gameConfig.windowWidth, gameConfig.windowHeight,
                              SDL_WINDOW_RESIZABLE, &window_, &sdl_renderer_);

  if (!window_) {
    Logger::Error("SDL_CreateWindow Error: " + std::string(SDL_GetError()));
    return false;
  }

  if (!sdl_renderer_) {
    Logger::Error("SDL_CreateRenderer Error: " + std::string(SDL_GetError()));
    return false;
  }

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

  ImGui_ImplSDL3_InitForSDLRenderer(window_, sdl_renderer_);
  ImGui_ImplSDLRenderer3_Init(sdl_renderer_);

  SDL_SetRenderDrawColor(sdl_renderer_, GREY_COLOR, GREY_COLOR, GREY_COLOR, Constants::kUnt8Max);

  registry_->Set<SDL_Renderer *>(sdl_renderer_);
  registry_->Set<EventBus *>(event_bus_.get());

  s_is_running_ = true;
  return true;
}

void Game::Destroy() const {
  if (sdl_renderer_) {
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(sdl_renderer_);
  }

  if (window_) {
    SDL_DestroyWindow(window_);
  }

  SDL_Quit();
}

void Game::Run() {
  Setup();

  // Seed previous-frame tick so the first WaitTime call produces a sane delta.
  milliseconds_previous_frame_ = SDL_GetTicks();

  while (s_is_running_) {
    ProcessInput();
    const float deltaTime = WaitTime();
    Update(deltaTime);
    Render(deltaTime);
  }
}

void Game::Setup() {
  auto &gameConfig = registry_->Get<GameConfig>();
  const SDL_FRect camera(0, 0, static_cast<float>(gameConfig.windowWidth), static_cast<float>(gameConfig.windowHeight));

  registry_->Set<RenderQueue>(RenderQueue());
  registry_->Set<CameraComponent>(CameraComponent{camera});
  registry_->Set<AssetManager>(AssetManager());

  // Gameplay
  auto &scriptSystem = registry_->RegisterSystem<ScriptComponent>(ScriptSystem());
  script_system_ = &scriptSystem;

  lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::io, sol::lib::string, sol::lib::table);
  scriptSystem.CreateLuaBindings(lua, *this);
  lua["game_window_width"] = gameConfig.windowWidth;
  lua["game_window_height"] = gameConfig.windowHeight;

  auto &assetManager = registry_->Get<AssetManager>();
  assetManager.LoadGameConfig(gameConfig);
  LoadGame(lua, assetManager, gameConfig);

  registry_->RegisterSystem<SpriteComponent, AnimationComponent>(AnimationSystem());
  auto &projectileEmitSystem =
      registry_->RegisterSystem<TransformComponent, ProjectileEmitterComponent>(ProjectileEmitSystem());
  registry_->RegisterSystem<ProjectileComponent>(ProjectileLifecycleSystem());
  auto &keyboardControlSystem =
      registry_->RegisterSystem<KeyboardControlComponent, RigidBodyComponent, SpriteComponent>(KeyboardControlSystem());

  auto &movementSystem = registry_->RegisterBulkSystem<TransformComponent, RigidBodyComponent>(MovementSystem());

  // Collision after movement integrates positions; emits CollisionEvent for damage/movement subscribers
  registry_->RegisterBulkSystem<TransformComponent, BoxColliderComponent, EntityMaskComponent>(CollisionSystem());

  // Transform hierarchy resolution — produces globalPosition/globalScale/globalRotation for downstream
  registry_->RegisterBulkSystem<TransformComponent>(TransformSystem());

  // Camera follows after gameplay-driven transform updates
  registry_->RegisterSystem<TransformComponent, CameraFollowComponent>(CameraFollowSystem());

  // Health UI updates before render so values reflect current frame
  registry_->RegisterSystem<HealthComponent, TextLabelComponent, SquarePrimitiveComponent>(DisplayHealthSystem());

  // Render queue producers
  registry_->RegisterBulkSystem<TransformComponent, SpriteComponent>(RenderSpriteSystem());
  registry_->RegisterSystem<TextLabelComponent>(RenderTextSystem());
  registry_->RegisterSystem<SquarePrimitiveComponent>(RenderPrimitiveSystem());

  // Event subscriptions (one-time)
  event_bus_->SubscribeEvent<Game, KeyInputEvent>(this, &Game::OnKeyInputEvent);
  scriptSystem.SubscribeToEvents(event_bus_);
  keyboardControlSystem.SubscribeToEvents(event_bus_);
  projectileEmitSystem.Init(event_bus_);
  ui_button_system_.Init(registry_.get(), event_bus_);
  damage_system_.Init(registry_.get(), event_bus_);
  movementSystem.Init(registry_.get(), event_bus_);
}

void Game::ProcessInput() const {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    ImGui_ImplSDL3_ProcessEvent(&event);
    auto &io = ImGui::GetIO();
    float mouseX, mouseY;
    const unsigned int buttons = SDL_GetMouseState(&mouseX, &mouseY);
    io.MousePos = ImVec2(mouseX, mouseY);
    io.MouseDown[0] = buttons & SDL_BUTTON_MASK(SDL_BUTTON_LEFT);
    io.MouseDown[1] = buttons & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT);

    switch (event.type) {
      case SDL_EVENT_QUIT:
        s_is_running_ = false;
        break;

      case SDL_EVENT_KEY_DOWN:
      case SDL_EVENT_KEY_UP: {
        KeyInputEvent keyInputEvent = GetKeyInputEvent(&event.key);
        event_bus_->EmitEvent<KeyInputEvent>(keyInputEvent);
        break;
      }
      case SDL_EVENT_MOUSE_BUTTON_DOWN:
      case SDL_EVENT_MOUSE_BUTTON_UP: {
        SDL_MouseButtonEvent mouseButtonEvent = event.button;
        event_bus_->EmitEvent<MouseInputEvent>(mouseButtonEvent);
        break;
      }
      default:
        break;
    }
  }
}

void Game::Update(const float deltaTime) {
  registry_->Update(deltaTime);
  // Pressed-keys are a per-frame edge signal — clear after every script entity in this frame
  // has had a chance to observe them.
  if (script_system_) {
    script_system_->ClearPerFrameInput();
  }
}

void Game::Render(const float deltaTime) {
  PROFILE_NAMED_SCOPE("Game::Render (total)");
  auto &renderQueue = registry_->Get<RenderQueue>();
  auto &gameConfig = registry_->Get<GameConfig>();

  SDL_SetRenderDrawColor(sdl_renderer_, GREY_COLOR, GREY_COLOR, GREY_COLOR, Constants::kUnt8Max);
  SDL_RenderClear(sdl_renderer_);

#ifdef OCTARINE_PROFILING
  {
    PerfUtils::ScopedTimer sortTimer("Render: Sort");
    renderQueue.Sort();
  }
  {
    PerfUtils::ScopedTimer drawTimer("Render: Draw");
    renderer_->Render(registry_.get(), sdl_renderer_);
  }
#else
  renderQueue.Sort();
  renderer_->Render(registry_.get(), sdl_renderer_);
#endif

  if (gameConfig.GetEngineOptions().drawColliders) {
    auto colliderQuery = registry_->CreateQuery<TransformComponent, BoxColliderComponent>();
    DrawColliderSystem drawColliderSystem;
    colliderQuery->ForEach(drawColliderSystem);
  }

  RenderDebugGUISystem::Render(registry_.get(), sdl_renderer_, deltaTime);

  SDL_RenderPresent(sdl_renderer_);
  renderQueue.Clear();
}

float Game::WaitTime() {
  const Uint64 elapsedTime = SDL_GetTicks() - milliseconds_previous_frame_;
  if (elapsedTime < Constants::kMillisecondsPerFrame) {
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

void Game::OnKeyInputEvent(const KeyInputEvent &event) {
  if (!event.isPressed) {
    return;
  }

  auto &gameConfig = registry_->Get<GameConfig>();

  switch (event.inputKey) {
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

KeyInputEvent Game::GetKeyInputEvent(SDL_KeyboardEvent *event) {
  bool isPressed = event->down;
  return {event->key, event->mod, isPressed};
}
