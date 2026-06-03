#include "Engine/FrameLoop.h"

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <algorithm>

#include "Components/ViewportInfo.h"
#include "ECS/Registry.h"
#include "Engine/EngineContext.h"
#include "Engine/EngineRuntime.h"
#include "EventBus/EventBus.h"
#include "Events/KeyInputEvent.h"
#include "Events/MouseInputEvent.h"
#include "Events/MouseWheelEvent.h"
#include "Game/Game.h"
#include "Game/GameConfig.h"
#include "General/Constants.h"
#include "General/PerfUtils.h"
#include "Lua/HotReload/ScriptHotReload.h"
#include "Renderer/RenderQueue.h"
#include "Renderer/Renderer.h"
#include "Systems/DrawColliderSystem.h"
#include "Systems/InputSystem.h"

#ifndef OCTARINE_SHIPPED
#include "Dev/DevListenServer.h"
#endif

#ifdef OCTARINE_WITH_IMGUI
#include "Systems/RenderDebugGUISystem.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#endif

FrameLoop::FrameLoop(Game* game, Registry* registry, EventBus* eventBus, Renderer* renderer, EngineRuntime* runtime,
                     sol::state& lua)
    : game_(game), registry_(registry), event_bus_(eventBus), renderer_(renderer), runtime_(runtime), lua_(lua) {
  // Pre-build the debug-collider query once so we don't allocate per render frame.
  collider_query_ = registry_->CreateQuery<GlobalTransformComponent, BoxColliderComponent>();
}

void FrameLoop::SubscribeToEvents() {
  key_input_subscription_ = event_bus_->SubscribeEvent<FrameLoop, KeyInputEvent>(this, &FrameLoop::OnKeyInputEvent);
}

void FrameLoop::Begin() { milliseconds_previous_frame_ = SDL_GetTicks(); }

void FrameLoop::ProcessInput() {
  PROFILE_NAMED_SCOPE("Game::ProcessInput");
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
#ifdef OCTARINE_WITH_IMGUI
    ImGui_ImplSDL3_ProcessEvent(&event);
    auto& io = ImGui::GetIO();
    float mouseX, mouseY;
    const unsigned int buttons = SDL_GetMouseState(&mouseX, &mouseY);
    io.MousePos = ImVec2(mouseX, mouseY);
    io.MouseDown[0] = buttons & SDL_BUTTON_MASK(SDL_BUTTON_LEFT);
    io.MouseDown[1] = buttons & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT);
#endif

    switch (event.type) {
      case SDL_EVENT_QUIT:
        Game::Quit();
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
      case SDL_EVENT_MOUSE_WHEEL: {
        event_bus_->EmitEvent<MouseWheelEvent>(event.wheel.x, event.wheel.y);
        break;
      }
      case SDL_EVENT_WINDOW_RESIZED:
      case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: {
        auto& viewportInfo = registry_->Get<ViewportInfo>();
        int windowW, windowH;
        SDL_GetWindowSize(runtime_->Window(), &windowW, &windowH);
        viewportInfo.width = static_cast<float>(windowW);
        viewportInfo.height = static_cast<float>(windowH);
        break;
      }
      default:
        break;
    }
  }
}

void FrameLoop::Update(const float deltaTime) {
#ifdef OCTARINE_PROFILING
  PerfUtils::ProfilingAccumulator::Clear();
  PerfUtils::PerfCounters::ResetValues();
#endif
  PROFILE_NAMED_SCOPE("Game::Update (total)");

#ifndef OCTARINE_SHIPPED
  if (auto* devListen = registry_->TryGet<octarine::dev::DevListenServer>()) {
    devListen->Pump();
  }
#endif

  auto& options = registry_->Get<GameConfig>().GetEngineOptions();

  // Master volume + mute live at the mixer level so they apply to every track (including loops
  // already playing). Synced every frame — and outside the pause gate — so toggling mute reacts
  // immediately even while execution is paused.
  if (auto* mixer = registry_->Get<EngineContext>().mixer) {
    const float masterGain = options.audioEnabled ? std::max(0.0F, options.masterVolume) : 0.0F;
    MIX_SetMixerGain(mixer, masterGain);
  }

  auto* inputSystem = registry_->TryGet<InputSystem>();
  if (inputSystem) {
    inputSystem->BeginFrame();
  }

#ifndef OCTARINE_SHIPPED
  // Run hot reload even when paused — authors iterate code with the editor paused all the time,
  // and the swap is cheap. Uses real deltaTime (not time-scaled) so the poll cadence is stable.
  if (options.hotReloadEnabled) {
    if (auto* hotReload = registry_->TryGet<ScriptHotReload>()) {
      hotReload->Tick(*registry_, lua_, deltaTime, options.hotReloadPollSeconds);
    }
  }
#endif

  if (!options.isPaused || options.stepFrame) {
    registry_->Update(deltaTime * options.timeScale);
    options.stepFrame = false;
  } else {
    // If paused, we might still want to clear some per-frame signals so they don't get stuck.
  }

  // Pressed/released keys and wheel deltas are per-frame edge signals — clear after every
  // system in this frame has observed them.
  if (inputSystem) {
    inputSystem->ClearPerFrameInput();
  }
}

void FrameLoop::Render([[maybe_unused]] const float deltaTime) {
  PROFILE_NAMED_SCOPE("Game::Render (total)");
  auto& renderQueue = registry_->Get<RenderQueue>();
  auto& gameConfig = registry_->Get<GameConfig>();

  PROFILE_COUNTER_SET("RenderQueue: Size", static_cast<long long>(renderQueue.Size()));
  PROFILE_COUNTER_SET("Entities: User", static_cast<long long>(registry_->GetUserEntityCount()));

  renderer_->BeginScene(runtime_->SdlRenderer());

#ifdef OCTARINE_PROFILING
  {
    PerfUtils::ScopedTimer sortTimer("Render: Sort");
    renderQueue.Sort();
  }
  {
    PerfUtils::ScopedTimer drawTimer("Render: Draw");
    renderer_->DrawQueue(renderQueue, runtime_->SdlRenderer());
  }
#else
  renderQueue.Sort();
  renderer_->DrawQueue(renderQueue, runtime_->SdlRenderer());
#endif

  if (gameConfig.GetEngineOptions().drawColliders && collider_query_) {
    collider_query_->Update();
    DrawColliderSystem drawColliderSystem;
    collider_query_->ForEach(drawColliderSystem);
  }

  renderer_->EndScene(runtime_->SdlRenderer());

  auto& options = gameConfig.GetEngineOptions();
  const bool editorSession = gameConfig.IsEditorMode() || !gameConfig.HasLoadedConfig();

  // Update viewport info for non-editor sessions or when ImGui is disabled.
  // In editor mode with ImGui, RenderDebugGUISystem::Render will override this with the Scene window bounds.
  auto& viewportInfo = registry_->Get<ViewportInfo>();
  viewportInfo.x = 0;
  viewportInfo.y = 0;
  int windowW, windowH;
  SDL_GetWindowSize(runtime_->Window(), &windowW, &windowH);
  viewportInfo.width = static_cast<float>(windowW);
  viewportInfo.height = static_cast<float>(windowH);

#ifdef OCTARINE_WITH_IMGUI
  auto& io = ImGui::GetIO();
  viewportInfo.isHovered = !io.WantCaptureMouse;
  viewportInfo.isFocused = !io.WantCaptureKeyboard;
#else
  viewportInfo.isHovered = true;
  viewportInfo.isFocused = true;
#endif

  if (!game_->IsBenchMode()) {
    // Only draw the game texture to the full window if we are NOT in an editor session
    // and NOT showing debug overlays. In editor mode, the Scene window handles drawing this texture.
    if (!editorSession && !options.showDebugGUI) {
      renderer_->CompositeSceneToWindow(runtime_->SdlRenderer());
    }
#ifdef OCTARINE_WITH_IMGUI
    RenderDebugGUISystem::Render(game_, runtime_->SdlRenderer(), renderer_->GetSceneTexture(), deltaTime);
#endif
  }

#ifdef OCTARINE_PROFILING
  {
    PerfUtils::ScopedTimer presentTimer("Render: Present");
    renderer_->Present(runtime_->SdlRenderer());
  }
#else
  renderer_->Present(runtime_->SdlRenderer());
#endif
  // Emit per-frame counters as COUNTER lines so headless bench runs can capture them
  // alongside TIMER lines. All systems for this frame have already written their values.
  PROFILE_COUNTERS_REPORT();
  renderQueue.Clear();
}

float FrameLoop::WaitTime() {
  PROFILE_NAMED_SCOPE("Game::WaitTime");
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

void FrameLoop::OnKeyInputEvent(const KeyInputEvent& event) {
  if (!event.isPressed) {
    return;
  }

  auto& gameConfig = registry_->Get<GameConfig>();

  switch (event.inputKey) {
    case SDLK_ESCAPE:
      Game::Quit();
      break;
    case SDLK_GRAVE:
      gameConfig.GetEngineOptions().showDebugGUI = !gameConfig.GetEngineOptions().showDebugGUI;
      break;
    default:
      break;
  }
}

KeyInputEvent FrameLoop::GetKeyInputEvent(SDL_KeyboardEvent* event) {
  bool isPressed = event->down;
  return {event->key, event->mod, isPressed};
}
