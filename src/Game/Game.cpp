#include "Game.h"

#include <SDL3/SDL.h>

#include <glm/glm.hpp>
#include <memory>
#include <sol/sol.hpp>
#include <string>

#include "../ECS/ECS.h"
#include "../Events/KeyInputEvent.h"
#include "../General/Logger.h"
#include "../Renderer/RenderQueue.h"
#include "../Renderer/Renderer.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CameraFollowSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/DisplayHealthSystem.h"
#include "../Systems/DrawColliderSystem.h"
#include "../Systems/KeyboardControlSystem.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/ProjectileEmitSystem.h"
#include "../Systems/ProjectileLifecycleSystem.h"
#include "../Systems/RenderDebugGUISystem.h"
#include "../Systems/RenderPrimitiveSystem.h"
#include "../Systems/RenderSpriteSystem.h"
#include "../Systems/RenderTextSystem.h"
#include "../Systems/ScriptSystem.h"
#include "../Systems/UIButtonSystem.h"
#include "Systems/TransformSystem.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

int Game::windowWidth;
int Game::windowHeight;
float Game::mapWidth;
float Game::mapHeight;

constexpr Uint8 GREY_COLOR = 24;

inline void LoadGame(sol::state &lua, const AssetManager *assetManager, const GameConfig &gameConfig) {
  const auto filePath = assetManager->GetFullPath(gameConfig.GetStartupScript());

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

Game::Game() : window_(nullptr), sdl_renderer_(nullptr), camera_(), show_colliders_(false) {
  registry_ = std::make_unique<Registry>();
  asset_manager_ = std::make_unique<AssetManager>();
  event_bus_ = std::make_unique<EventBus>();
  renderer_ = std::make_unique<Renderer>();
  game_config_ = std::make_unique<GameConfig>();
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

  if (!game_config_->LoadConfigFromFile(assetPath)) {
    Logger::Error("Failed to load game config.");
    return false;
  }

  windowWidth = game_config_->GetDefaultWidth();
  windowHeight = game_config_->GetDefaultHeight();
  SDL_CreateWindowAndRenderer(game_config_->GetGameTitle().c_str(), windowWidth, windowHeight, SDL_WINDOW_RESIZABLE,
                              &window_, &sdl_renderer_);

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

  camera_.x = 0;
  camera_.y = 0;
  camera_.w = static_cast<float>(windowWidth);
  camera_.h = static_cast<float>(windowHeight);

  SDL_SetRenderDrawColor(sdl_renderer_, GREY_COLOR, GREY_COLOR, GREY_COLOR, Constants::kUnt8Max);
  asset_manager_->SetGameConfig(*game_config_);
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
    Update();
    Render();
  }
}

void Game::Setup() {
  registry_->AddSystem<TransformSystem>();
  registry_->AddSystem<CameraFollowSystem>();
  registry_->AddSystem<ProjectileEmitSystem>();
  registry_->AddSystem<ProjectileLifecycleSystem>();
  registry_->AddSystem<DisplayHealthSystem>();
  registry_->AddSystem<DamageSystem>();
  registry_->AddSystem<MovementSystem>();

  registry_->AddSystem<RenderSpriteSystem>();
  registry_->AddSystem<RenderTextSystem>();
  registry_->AddSystem<RenderPrimitiveSystem>();
  registry_->AddSystem<RenderDebugGUISystem>();
  registry_->AddSystem<AnimationSystem>();
  registry_->AddSystem<CollisionSystem>();
  registry_->AddSystem<DrawColliderSystem>();
  registry_->AddSystem<KeyboardControlSystem>();
  registry_->AddSystem<ScriptSystem>();
  registry_->AddSystem<UIButtonSystem>();

  lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::io, sol::lib::string, sol::lib::table);
  registry_->GetSystem<ScriptSystem>().CreateLuaBindings(lua, *this);
  lua["game_window_width"] = windowWidth;
  lua["game_window_height"] = windowHeight;

  LoadGame(lua, asset_manager_.get(), *game_config_);
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

void Game::Update() {
  // If we are too fast, waste some time until we reach the frame time
  const Uint64 elapsedTime = SDL_GetTicks() - milliseconds_previous_frame_;
  if (elapsedTime < Constants::kMillisecondsPerFrame) {
    const Uint32 timeToWait = Constants::kMillisecondsPerFrame - static_cast<Uint32>(elapsedTime);
    SDL_Delay(timeToWait);
  }

  // Subscribe to events
  event_bus_->Reset();
  registry_->GetSystem<DamageSystem>().SubscribeToEvents(event_bus_);
  registry_->GetSystem<KeyboardControlSystem>().SubscribeToEvents(event_bus_);
  registry_->GetSystem<ProjectileEmitSystem>().SubscribeToEvents(event_bus_);
  registry_->GetSystem<MovementSystem>().SubscribeToEvents(event_bus_);
  registry_->GetSystem<UIButtonSystem>().SubscribeToEvents(event_bus_);
  registry_->GetSystem<ScriptSystem>().SubscribeToEvents(event_bus_);
  SubscribeToEvents(event_bus_);

  // Calculate delta time
  const auto intermediate = static_cast<double>(SDL_GetTicks() - milliseconds_previous_frame_) /
                            static_cast<double>(Constants::kMillisecondsPerSecond);
  const auto deltaTime = static_cast<float>(intermediate);

  milliseconds_previous_frame_ = SDL_GetTicks();

  registry_->GetSystem<TransformSystem>().Update();  // Important to update first as it updates global positions.
  registry_->GetSystem<MovementSystem>().Update(deltaTime);
  registry_->GetSystem<AnimationSystem>().Update(deltaTime);
  registry_->GetSystem<CollisionSystem>().Update(event_bus_);
  registry_->GetSystem<KeyboardControlSystem>().Update();
  registry_->GetSystem<CameraFollowSystem>().Update(camera_);
  registry_->GetSystem<ProjectileEmitSystem>().Update(deltaTime, registry_);
  registry_->GetSystem<ProjectileLifecycleSystem>().Update(deltaTime);
  registry_->GetSystem<DisplayHealthSystem>().Update(registry_);
  registry_->GetSystem<ScriptSystem>().Update(deltaTime);
  registry_->Update();
}

void Game::Render() {
  SDL_SetRenderDrawColor(sdl_renderer_, GREY_COLOR, GREY_COLOR, GREY_COLOR, Constants::kUnt8Max);
  SDL_RenderClear(sdl_renderer_);

  // Render the game
  render_queue_.Clear();
  registry_->GetSystem<RenderSpriteSystem>().Update(render_queue_, camera_);
  registry_->GetSystem<RenderTextSystem>().Update(render_queue_);
  registry_->GetSystem<RenderPrimitiveSystem>().Update(render_queue_);

  render_queue_.Sort();
  renderer_->Render(render_queue_, sdl_renderer_, camera_, asset_manager_);

  if (show_colliders_) {
    registry_->GetSystem<DrawColliderSystem>().Update(sdl_renderer_, camera_);
    registry_->GetSystem<RenderDebugGUISystem>().Update(sdl_renderer_);
  }

  SDL_RenderPresent(sdl_renderer_);
}

void Game::SubscribeToEvents(const std::unique_ptr<EventBus> &eventBus) {
  eventBus->SubscribeEvent<Game, KeyInputEvent>(this, &Game::OnKeyInputEvent);
}

void Game::OnKeyInputEvent(const KeyInputEvent &event) {
  if (!event.isPressed) {
    return;
  }

  switch (event.inputKey) {
    case SDLK_ESCAPE:
      s_is_running_ = false;
      break;
    case SDLK_F5:
      show_colliders_ = !show_colliders_;
      break;
    default:
      break;
  }
}

KeyInputEvent Game::GetKeyInputEvent(SDL_KeyboardEvent *event) {
  bool isPressed = event->down;
  return {event->key, event->mod, isPressed};
}
