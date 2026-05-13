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
  void Destroy();
  void Run();
  static void Quit() { s_is_running_ = false; }

  // Optional startup-mode hint exposed to Lua as `oct_startup_mode`. Empty string means
  // "no override" — the startup script can show its normal menu. Set by Main from the
  // --startup-mode flag; consumed by the game's bootstrap script. A non-empty mode also
  // suppresses debug UI rendering so benchmarks don't pay that overhead.
  void SetStartupMode(const std::string& mode) { startup_mode_ = mode; }
  [[nodiscard]] bool IsBenchMode() const { return !startup_mode_.empty() && startup_mode_ != "editor"; }

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
  SDL_Texture* game_render_texture_{nullptr};
  static inline bool s_is_running_{false};
  Uint64 milliseconds_previous_frame_ = 0;

  sol::state lua;
  std::string startup_mode_;
  std::unique_ptr<Registry> registry_;
  std::unique_ptr<EventBus> event_bus_;
  std::unique_ptr<Renderer> renderer_;
  ScriptSystem* script_system_{nullptr};
  std::unique_ptr<ComponentQuery<TransformComponent, BoxColliderComponent>> collider_query_;
};
