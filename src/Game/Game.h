#pragma once

#include <SDL2/SDL.h>

#include <memory>
#include <sol/sol.hpp>

#include "../AssetManager/AssetManager.h"
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyInputEvent.h"
#include "../Renderer/RenderQueue.h"
#include "../Renderer/Renderer.h"

constexpr int kFps = 60;
constexpr int kMillisecondsPerFrame = 1000 / kFps;

class Game {
 public:
  explicit Game(const std::string& assetPath);
  ~Game();

  void Initialize();
  void Destroy();
  void Run();
  static void Quit() { s_is_running_ = false; }

  SDL_Renderer* GetRenderer() const { return sdl_renderer_; }
  AssetManager* GetAssetManager() const { return asset_manager_.get(); }
  Registry* GetRegistry() const { return registry_.get(); }

  static int windowWidth;
  static int windowHeight;
  static float mapWidth;
  static float mapHeight;

private:
  void ProcessInput();
  void Update();
  void Render();
  void Setup();
  void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus);
  void OnKeyInputEvent(KeyInputEvent& event);
  static KeyInputEvent GetKeyInputEvent(SDL_KeyboardEvent* event);

  SDL_Window* window_;
  SDL_Renderer* sdl_renderer_;
  SDL_Rect camera_;
  static inline bool s_is_running_{false};
  bool show_colliders_;
  int milliseconds_previous_frame_ = 0;

  sol::state lua;
  std::unique_ptr<Registry> registry_;
  std::unique_ptr<AssetManager> asset_manager_;
  std::unique_ptr<EventBus> event_bus_;
  std::unique_ptr<Renderer> renderer_;
  RenderQueue render_queue_;
};
