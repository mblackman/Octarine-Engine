#pragma once

#include <SDL3/SDL.h>

#include <memory>
#include <sol/sol.hpp>
#include <string>

#include "Components/BoxColliderComponent.h"
#include "Components/GlobalTransformComponent.h"
#include "ECS/Query.h"
#include "EventBus/EventBus.h"
#include "Systems/PerfOverlaySystem.h"

class Game;
class Registry;
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

  // Subscribe OnKeyInputEvent to the bus. Called once by Game during Setup; the returned RAII
  // handle is held as a member so the subscription drops when this FrameLoop is destroyed.
  void SubscribeToEvents();

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
  EventBus::SubscriptionHandle key_input_subscription_;
  // Built-in (no-ImGui) FPS + frame-time overlay. Holds a small per-line text-texture cache, so it
  // is a long-lived member rather than reconstructed each frame.
  PerfOverlaySystem perf_overlay_;

#ifndef OCTARINE_SHIPPED
  // Headless frame-capture (env-driven, dev/bench only). When OCTARINE_CAPTURE_PATH is set, the
  // loop renders up to OCTARINE_CAPTURE_FRAME (default 180 ≈ 3s @60fps, past the stress warmup),
  // writes that frame's scene texture to the path as BMP, then quits. Lets a headless run produce
  // a viewable rendered frame.
  static constexpr long kDefaultCaptureFrame = 180;  // ≈3s @60fps, past the stress warmup
  std::string capture_path_;
  long capture_frame_ = kDefaultCaptureFrame;
  long frame_index_ = 0;
  bool capture_done_ = false;
#endif
};
