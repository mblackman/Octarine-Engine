#pragma once

#include <SDL3/SDL.h>

#include <string>

class GameConfig;
class Renderer;

// Manages the platform and runtime lifecycle: SDL and TTF subsystem initialization,
// the main window and SDL renderer, ImGui backend setup, and orderly teardown.
// Keeps low-level platform handles separate from game configuration and editor state.
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
