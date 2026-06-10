#include "RenderDebugGUISystem.h"

#ifdef OCTARINE_WITH_IMGUI
#include <algorithm>
#include <chrono>
#include <vector>

#include "Game/Game.h"
#include "Game/GameConfig.h"
#include "General/PerfUtils.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

#ifdef OCTARINE_WITH_EDITOR
#include "AssetManager/AssetManager.h"
#include "Editor/EditorPersistence.h"
#include "Editor/ExportBuilder.h"
#include "Editor/HotPusher.h"
#include "Editor/Panels/EditorPanelHost.h"
#include "Editor/PlayerLauncher.h"
#endif

void RenderDebugGUISystem::Render(Game* game, SDL_Renderer* renderer, [[maybe_unused]] SDL_Texture* gameTexture,
                                  const float deltaTime) {
  auto* registry = game->GetRegistry();
  auto& gameConfig = registry->Get<GameConfig>();
  auto& engineOptions = gameConfig.GetEngineOptions();
  const bool projectLoaded = gameConfig.HasLoadedConfig();
#ifdef OCTARINE_WITH_EDITOR
  auto& editorPersistence = registry->Get<EditorPersistence>();
  auto& playerLauncher = registry->Get<octarine::editor::PlayerLauncher>();
  playerLauncher.Pump();
  auto& exportBuilder = registry->Get<octarine::editor::ExportBuilder>();
  exportBuilder.Pump();
  auto& hotPusher = registry->Get<octarine::editor::HotPusher>();
  const std::string& projectBase = registry->Get<AssetManager>().GetBasePath();
  hotPusher.Tick(ImGui::GetTime(), std::filesystem::path(projectBase));
  const bool showEditorUI = gameConfig.IsEditorMode() || !projectLoaded;
#endif
  const bool showGameOverlays = engineOptions.showDebugGUI;

  // The toast must survive every "nothing else to draw" early-out below, otherwise a script
  // error in a from-editor play session (editor chrome hidden, overlays off) stays invisible —
  // the exact failure mode it exists to surface.
  const bool toastActive = HasActiveScriptErrorToast();

#ifdef OCTARINE_WITH_EDITOR
  if (!showEditorUI && !showGameOverlays && !engineOptions.showFpsCounter && !toastActive) {
    return;
  }
#else
  // Player-with-ImGui build: the ImGui FPS window is editor-only (showFpsCounter is ignored here);
  // the player-facing FPS readout is the renderer perf overlay (PerfOverlay config). So nothing
  // ImGui draws unless the in-game debug overlays are toggled on or a script error is toasting.
  if (!showGameOverlays && !toastActive) {
    return;
  }
#endif

  PROFILE_NAMED_SCOPE("RenderDebugGUISystem::Render");

#ifdef OCTARINE_WITH_EDITOR
  octarine::editor::panels::ApplyPendingLayout(editorPersistence);
#endif

  ImGui_ImplSDLRenderer3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();

#ifdef OCTARINE_WITH_EDITOR
  if (showEditorUI) {
    octarine::editor::panels::DrawEditorChrome(game, gameTexture);
  }
#endif

  if (showGameOverlays) {
    if (projectLoaded) {
      auto query = registry->CreateQuery<ScriptComponent>();
      RenderDebugGUISystem system;
      query->ForEach(system);
    }
    EntityInfoWindow(registry);
#ifdef OCTARINE_WITH_EDITOR
    FPSWindow(deltaTime);
#endif
  }
#ifdef OCTARINE_WITH_EDITOR
  else if (engineOptions.showFpsCounter) {
    FPSWindow(deltaTime);
  }
#endif

#ifdef OCTARINE_WITH_EDITOR
  octarine::editor::panels::DrawProjectSelectorIfNeeded(game, projectLoaded);
#endif

  if (toastActive) {
    ScriptErrorToastWindow();
  }

  ImGui::Render();
  ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);

#ifdef OCTARINE_WITH_EDITOR
  octarine::editor::panels::ProcessFontRebuild(editorPersistence);
#endif
}

void RenderDebugGUISystem::FPSWindow(const float deltaTime) {
  static float fpsHistory[120] = {};
  static int fpsOffset = 0;
  const float fps = (deltaTime > 0.0f) ? 1.0f / deltaTime : 0.0f;
  fpsHistory[fpsOffset] = fps;
  fpsOffset = (fpsOffset + 1) % 120;

  ImGui::Begin("FPS");
  char overlay[32];
  snprintf(overlay, sizeof(overlay), "%.1f FPS", static_cast<double>(fps));
  ImGui::PlotLines("##fps", fpsHistory, 120, fpsOffset, overlay, 0.0f, 120.0f, ImVec2(0, 50));
  ImGui::End();
}

void RenderDebugGUISystem::EntityInfoWindow(const Registry* registry) {
  const auto count = registry->GetUserEntityCount();
  ImGui::Begin("Entity Info");
  ImGui::Text("Entity Count: %llu", static_cast<unsigned long long>(count));
  ImGui::End();
}

namespace {
// How long one script error stays on screen. Each new error restarts the clock for itself only,
// so a burst shows its tail and quiet errors age out individually.
constexpr double kScriptErrorToastSeconds = 10.0;

bool IsToastFresh(const Logger::ScriptError& error) {
  const auto age = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() -
                                                                             error.when);
  return age.count() < kScriptErrorToastSeconds;
}
}  // namespace

bool RenderDebugGUISystem::HasActiveScriptErrorToast() {
  const auto errors = Logger::RecentScriptErrors();
  return std::any_of(errors.begin(), errors.end(), [](const Logger::ScriptError& e) { return IsToastFresh(e); });
}

void RenderDebugGUISystem::ScriptErrorToastWindow() {
  const auto errors = Logger::RecentScriptErrors();
  std::vector<const Logger::ScriptError*> fresh;
  for (const auto& error : errors) {
    if (IsToastFresh(error)) fresh.push_back(&error);
  }
  if (fresh.empty()) return;

  // Bottom-right overlay: no decoration, no focus steal, no input capture — purely informative.
  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  constexpr float kMargin = 12.0f;
  ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + viewport->WorkSize.x - kMargin,
                                 viewport->WorkPos.y + viewport->WorkSize.y - kMargin),
                          ImGuiCond_Always, ImVec2(1.0f, 1.0f));
  ImGui::SetNextWindowBgAlpha(0.85f);
  constexpr ImGuiWindowFlags kFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                                      ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                                      ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoInputs |
                                      ImGuiWindowFlags_AlwaysAutoResize;
  ImGui::Begin("##script-error-toast", nullptr, kFlags);
  ImGui::TextColored(ImVec4(1.0f, 0.35f, 0.35f, 1.0f), "Script error%s (%d)", fresh.size() == 1 ? "" : "s",
                     static_cast<int>(fresh.size()));
  ImGui::Separator();
  ImGui::PushTextWrapPos(420.0f);
  for (const auto* error : fresh) {
    ImGui::TextWrapped("%s", error->message.c_str());
  }
  ImGui::PopTextWrapPos();
  ImGui::End();
}
#endif
