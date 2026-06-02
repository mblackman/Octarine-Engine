#pragma once

#include <SDL3/SDL.h>

#include <string>

class GameConfig;
class Renderer;

// Owns the platform/runtime lifecycle that Game::Initialize and Game::Destroy used to inline:
// SDL + TTF subsystem init, the window/renderer pair, the ImGui backend, and orderly teardown.
// It deliberately knows nothing about GameConfig loading, project resolution, or editor
// persistence — Game still drives those and feeds this the window title/size it derived. The
// off-screen scene render target stays owned by Renderer (Stage 7); Game sequences DestroyScene
// around Shutdown so the SDL renderer is still live when the scene target is freed.
class EngineRuntime {
 public:
  EngineRuntime() = default;

  EngineRuntime(const EngineRuntime&) = delete;
  EngineRuntime& operator=(const EngineRuntime&) = delete;
  EngineRuntime(EngineRuntime&&) = delete;
  EngineRuntime& operator=(EngineRuntime&&) = delete;

  ~EngineRuntime() = default;

  // SDL_Init(video/audio/events/gamepad) + TTF_Init. Returns false (logging the SDL error) on
  // failure so Game::Initialize can abort.
  [[nodiscard]] bool InitSubsystems();

  // Create the window + accelerated renderer (resizable). Returns false on failure; on success
  // Window()/SdlRenderer() are non-null.
  [[nodiscard]] bool CreateWindow(const std::string& title, int width, int height);

#ifdef OCTARINE_WITH_IMGUI
  // Stand up the ImGui context + SDL3/SDLRenderer3 backends and the docking/nav config flags.
  // `iniPath` becomes io.IniFilename (must outlive the context). Fonts/style are the caller's
  // job afterwards (editor builds rebuild the editor font; player builds add the default).
  void InitImGui(const char* iniPath);
#endif

  // Tear down the ImGui backend (if built) and destroy the renderer, window, and SDL subsystems.
  // Safe to call when CreateWindow was never reached (no-ops on null handles). Call AFTER the
  // owner has released anything that needs a live SDL renderer (scene target, GPU textures).
  void Shutdown();

  [[nodiscard]] SDL_Window* Window() const { return window_; }
  [[nodiscard]] SDL_Renderer* SdlRenderer() const { return sdl_renderer_; }

 private:
  SDL_Window* window_ = nullptr;
  SDL_Renderer* sdl_renderer_ = nullptr;
};
