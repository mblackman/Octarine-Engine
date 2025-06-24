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
#include "Components/CameraComponents.h"
#include "Components/HealthComponent.h"
#include "Components/ScriptComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/TransformComponent.h"
#include "ECS/Registry.h"
#include "GameConfig.h"
#include "Systems/AnimationSystem.h"
#include "Systems/RenderSpriteSystem.h"
#include "Systems/ScriptSystem.h"
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
  registry_->Set<RenderQueue>(RenderQueue());
  registry_->Set<AssetManager>(AssetManager());

  ScriptSystem scriptSystem;
  // registry_->AddSystem<TransformSystem>();
  // registry_->AddSystem<CameraFollowSystem>();
  // registry_->AddSystem<ProjectileEmitSystem>();
  // registry_->AddSystem<ProjectileLifecycleSystem>();
  // registry_->AddSystem<DisplayHealthSystem>();
  // registry_->AddSystem<DamageSystem>();
  // registry_->AddSystem<MovementSystem>();
  //
  registry_->RegisterSystem<TransformComponent, SpriteComponent>(RenderSpriteSystem());
  // registry_->AddSystem<RenderTextSystem>();
  // registry_->AddSystem<RenderPrimitiveSystem>();
  // registry_->AddSystem<RenderDebugGUISystem>();
  registry_->RegisterSystem<SpriteComponent, AnimationComponent>(AnimationSystem());
  // registry_->AddSystem<CollisionSystem>();
  // registry_->AddSystem<DrawColliderSystem>();
  // registry_->AddSystem<KeyboardControlSystem>();

  // registry_->AddSystem<UIButtonSystem>();

  lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::io, sol::lib::string, sol::lib::table);
  scriptSystem.CreateLuaBindings(lua, *this);
  lua["game_window_width"] = gameConfig.windowWidth;
  lua["game_window_height"] = gameConfig.windowHeight;

  auto &assetManager = registry_->Get<AssetManager>();
  assetManager.LoadGameConfig(gameConfig);
  LoadGame(lua, assetManager, gameConfig);

  registry_->RegisterSystem<ScriptComponent>(std::move(scriptSystem));
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
        // event_bus_->EmitEvent<MouseInputEvent>(mouseButtonEvent);
        break;
      }
      default:
        break;
    }
  }
}

void Game::Update(const float deltaTime) {
  auto query = registry_->CreateQuery<TransformComponent, HealthComponent>();
  query.ForEach([](TransformComponent &transform, HealthComponent &health) {
    if (transform.position.x < 0) {
      transform.position.x = 0;
    }
  });
  // Subscribe to events
  event_bus_->Reset();
  // registry_->GetSystem<DamageSystem>().SubscribeToEvents(event_bus_);
  // registry_->GetSystem<KeyboardControlSystem>().SubscribeToEvents(event_bus_);
  // registry_->GetSystem<ProjectileEmitSystem>().SubscribeToEvents(event_bus_);
  // registry_->GetSystem<MovementSystem>().SubscribeToEvents(event_bus_);
  // registry_->GetSystem<UIButtonSystem>().SubscribeToEvents(event_bus_);
  // registry_->GetSystem<ScriptSystem>().SubscribeToEvents(event_bus_);
  SubscribeToEvents(event_bus_);

  // Update our systems and registry
  // registry_->GetSystem<TransformSystem>().Update();  // Important to update first as it updates global positions.
  // registry_->GetSystem<MovementSystem>().Update(deltaTime);
  // registry_->GetSystem<AnimationSystem>().Update(deltaTime);
  // registry_->GetSystem<CollisionSystem>().Update(event_bus_);
  // registry_->GetSystem<KeyboardControlSystem>().Update();
  // registry_->GetSystem<CameraFollowSystem>().Update(camera_);
  // registry_->GetSystem<ProjectileEmitSystem>().Update(deltaTime, registry_);
  // registry_->GetSystem<ProjectileLifecycleSystem>().Update(deltaTime);
  // registry_->GetSystem<DisplayHealthSystem>().Update(registry_);
  // registry_->GetSystem<ScriptSystem>().Update(deltaTime);

  registry_->Update(deltaTime);
}

void Game::Render(const float deltaTime) {
  auto &renderQueue = registry_->Get<RenderQueue>();
  auto &gameConfig = registry_->Get<GameConfig>();

  SDL_SetRenderDrawColor(sdl_renderer_, GREY_COLOR, GREY_COLOR, GREY_COLOR, Constants::kUnt8Max);
  SDL_RenderClear(sdl_renderer_);

  // Render the game
  // registry_->GetSystem<RenderSpriteSystem>().Update(render_queue_, camera_);
  // registry_->GetSystem<RenderTextSystem>().Update(render_queue_);
  // registry_->GetSystem<RenderPrimitiveSystem>().Update(render_queue_);

  renderQueue.Sort();
  renderer_->Render(registry_.get(), sdl_renderer_);

  if (gameConfig.GetEngineOptions().drawColliders) {
    // registry_->GetSystem<DrawColliderSystem>().Update(sdl_renderer_, camera_);
  }

  // registry_->GetSystem<RenderDebugGUISystem>().Update(deltaTime, sdl_renderer_);

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

void Game::SubscribeToEvents(const std::unique_ptr<EventBus> &eventBus) {
  eventBus->SubscribeEvent<Game, KeyInputEvent>(this, &Game::OnKeyInputEvent);
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
