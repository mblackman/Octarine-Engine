#pragma once

#include <SDL3/SDL.h>

#include <memory>
#include <sol/sol.hpp>

#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/Query.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyInputEvent.h"
#include "../Renderer/Renderer.h"

class ScriptSystem;

class Registry;

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

  [[nodiscard]] Registry* GetRegistry() const { return registry_.get(); }

 private:
  void ProcessInput() const;
  void Update(float deltaTime);
  void Render(float deltaTime);
  [[nodiscard]] float WaitTime();
  void Setup();
  void OnKeyInputEvent(const KeyInputEvent& event);
  static KeyInputEvent GetKeyInputEvent(SDL_KeyboardEvent* event);

  SDL_Window* window_;
  SDL_Renderer* sdl_renderer_;
  static inline bool s_is_running_{false};
  Uint64 milliseconds_previous_frame_ = 0;

  sol::state lua;
  std::unique_ptr<Registry> registry_;
  std::unique_ptr<EventBus> event_bus_;
  std::unique_ptr<Renderer> renderer_;
  ScriptSystem* script_system_{nullptr};
  std::unique_ptr<ComponentQuery<TransformComponent, BoxColliderComponent>> collider_query_;
};
