#pragma once

#include <SDL3/SDL.h>

#include <memory>
#include <sol/sol.hpp>

#include "Components/BoxColliderComponent.h"
#include "Components/GlobalTransformComponent.h"
#include "ECS/Query.h"

class Game;
class Registry;
class EventBus;
class Renderer;
class EngineRuntime;
// Defined in Events/KeyInputEvent.h. Forward-declared (not included) because that header is not
// self-contained — it derives from EventBus's Event without including it, so pulling it here
// would force a fragile include order on every consumer of FrameLoop.h.
struct KeyInputEvent;

// Owns the per-frame loop body Game::Run drives: poll input, wait out the frame budget, update
// the ECS, and render. Game::Run keeps the while(running) loop and Setup/teardown; FrameLoop is
// the four phases plus the key-event handler that toggles the debug GUI / requests quit.
//
// Holds non-owning pointers to the engine pieces it touches (registry, event bus, renderer, the
// SDL runtime) plus the Game it renders the editor overlay for. Constructed by Game once those
// exist; the debug-collider query is built here so it isn't reallocated per frame.
class FrameLoop {
 public:
  FrameLoop(Game* game, Registry* registry, EventBus* eventBus, Renderer* renderer, EngineRuntime* runtime,
            sol::state& lua);

  FrameLoop(const FrameLoop&) = delete;
  FrameLoop& operator=(const FrameLoop&) = delete;
  FrameLoop(FrameLoop&&) = delete;
  FrameLoop& operator=(FrameLoop&&) = delete;

  ~FrameLoop() = default;

  // Seed the previous-frame tick so the first WaitTime() yields a sane delta. Call once right
  // before entering the loop.
  void Begin();

  void ProcessInput();
  void Update(float deltaTime);
  void Render(float deltaTime);
  [[nodiscard]] float WaitTime();

  // KeyInputEvent subscriber (Esc → quit, backtick → toggle debug GUI). Public so Game can wire
  // it to the EventBus during Setup.
  void OnKeyInputEvent(const KeyInputEvent& event);

 private:
  static KeyInputEvent GetKeyInputEvent(SDL_KeyboardEvent* event);

  Game* game_;
  Registry* registry_;
  EventBus* event_bus_;
  Renderer* renderer_;
  EngineRuntime* runtime_;
  sol::state& lua_;
  Uint64 milliseconds_previous_frame_ = 0;
  std::unique_ptr<ComponentQuery<GlobalTransformComponent, BoxColliderComponent>> collider_query_;
};
