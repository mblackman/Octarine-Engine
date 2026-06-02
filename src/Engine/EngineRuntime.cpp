#include "Engine/EngineRuntime.h"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <string>

#include "General/Logger.h"

#ifdef OCTARINE_WITH_IMGUI
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#endif

bool EngineRuntime::InitSubsystems() {
  constexpr auto SDL_INI = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD;

  if (!SDL_Init(SDL_INI)) {
    Logger::Error("SDL_Init Error: " + std::string(SDL_GetError()));
    return false;
  }

  if (!TTF_Init()) {
    Logger::Error("TTF_Init Error: " + std::string(SDL_GetError()));
    return false;
  }

  return true;
}

bool EngineRuntime::CreateWindow(const std::string& title, int width, int height) {
  SDL_CreateWindowAndRenderer(title.c_str(), width, height, SDL_WINDOW_RESIZABLE, &window_, &sdl_renderer_);

  if (!window_) {
    Logger::Error("SDL_CreateWindow Error: " + std::string(SDL_GetError()));
    return false;
  }

  if (!sdl_renderer_) {
    Logger::Error("SDL_CreateRenderer Error: " + std::string(SDL_GetError()));
    return false;
  }

  return true;
}

#ifdef OCTARINE_WITH_IMGUI
void EngineRuntime::InitImGui(const char* iniPath) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.IniFilename = iniPath;

  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  ImGui_ImplSDL3_InitForSDLRenderer(window_, sdl_renderer_);
  ImGui_ImplSDLRenderer3_Init(sdl_renderer_);
}
#endif

void EngineRuntime::Shutdown() {
  if (sdl_renderer_) {
#ifdef OCTARINE_WITH_IMGUI
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
#endif
    SDL_DestroyRenderer(sdl_renderer_);
    sdl_renderer_ = nullptr;
  }

  if (window_) {
    SDL_DestroyWindow(window_);
    window_ = nullptr;
  }

  SDL_Quit();
}
