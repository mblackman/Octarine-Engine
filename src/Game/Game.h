#pragma once

#include <SDL3/SDL.h>

#include <memory>
#include <sol/sol.hpp>

#include "../AssetManager/AssetManager.h"
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyInputEvent.h"
#include "../Renderer/RenderQueue.h"
#include "../Renderer/Renderer.h"

class Game {
 public:
  Game();

  Game(const Game&) = delete;
  Game& operator=(const Game&) = delete;

  Game(Game&&) = delete;
  Game& operator=(Game&&) = delete;

  ~Game();

  bool Initialize(const std::string& assetPath);
  void Destroy() const;
  void Run();
  static void Quit() { s_is_running_ = false; }

  [[nodiscard]] SDL_Renderer* GetRenderer() const { return sdl_renderer_; }

  [[nodiscard]] AssetManager* GetAssetManager() const { return asset_manager_.get(); }

  [[nodiscard]] Registry* GetRegistry() const { return registry_.get(); }

 private:
  void ProcessInput() const;
  void Update(float deltaTime);
  void Render(float deltaTime);
  [[nodiscard]] float WaitTime();
  void Setup();
  void SubscribeToEvents(const std::unique_ptr<EventBus>& eventBus);
  void OnKeyInputEvent(const KeyInputEvent& event);
  static KeyInputEvent GetKeyInputEvent(SDL_KeyboardEvent* event);

  SDL_Window* window_;
  SDL_Renderer* sdl_renderer_;
  SDL_FRect camera_;
  static inline bool s_is_running_{false};
  Uint64 milliseconds_previous_frame_ = 0;

  sol::state lua;
  std::unique_ptr<Registry> registry_;
  std::unique_ptr<AssetManager> asset_manager_;
  std::unique_ptr<EventBus> event_bus_;
  std::unique_ptr<Renderer> renderer_;
  RenderQueue render_queue_;
};
