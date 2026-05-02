#pragma once

#include <SDL3/SDL.h>

#include <memory>
#include <sol/sol.hpp>

#include "GameConfig.h"
#include "../AssetManager/AssetManager.h"
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyInputEvent.h"
#include "../Renderer/RenderQueue.h"
#include "../Renderer/Renderer.h"

constexpr int kMillisecondsPerSecond = 1000;
constexpr int kFps = 60;
constexpr int kMillisecondsPerFrame = kMillisecondsPerSecond / kFps;

class Game {
public:
  Game();
  ~Game();

  bool Initialize(const std::string& assetPath);
  void Destroy() const;
  void Run();
  static void Quit() { s_is_running_ = false; }

  [[nodiscard]] SDL_Renderer* GetRenderer() const { return sdl_renderer_; }

  [[nodiscard]] AssetManager* GetAssetManager() const {
    return asset_manager_.get();
  }

  [[nodiscard]] Registry* GetRegistry() const { return registry_.get(); }

  static int windowWidth;
  static int windowHeight;
  static float mapWidth;
  static float mapHeight;

private:
  void ProcessInput() const;
  void Update();
  void Render();
  void Setup();
  void SubscribeToEvents(const std::unique_ptr<EventBus>& eventBus);
  void OnKeyInputEvent(const KeyInputEvent& event);
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
  std::unique_ptr<GameConfig> game_config_;
  RenderQueue render_queue_;
};
