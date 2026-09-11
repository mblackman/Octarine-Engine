#include "RenderDebugGUISystem.h"

#ifdef OCTARINE_WITH_IMGUI
#include <algorithm>
#include <chrono>
#include <vector>

#include "Components/ViewportInfo.h"
#include "Game/Game.h"
#include "Game/GameConfig.h"
#include "General/PerfUtils.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include "imgui_internal.h"

#ifdef OCTARINE_WITH_EDITOR
#include "AssetManager/AssetManager.h"
#include "Editor/EditorPersistence.h"
#include "Editor/ExportBuilder.h"
#include "Editor/HotPusher.h"
#include "Editor/Panels/EditorPanelHost.h"
#include "Editor/PlayerLauncher.h"
#endif

void RenderDebugGUISystem::ReturnFocusToGame(Game* game) {
  if (!game) return;
  auto* registry = game->GetRegistry();
  if (!registry) return;
  auto& viewportInfo = registry->Get<ViewportInfo>();

#ifdef OCTARINE_WITH_IMGUI
  if (ImGui::GetCurrentContext()) {
    ImGuiIO& io = ImGui::GetIO();
    io.ClearInputKeys();
    io.ClearEventsQueue();
    ImGui::ClearActiveID();

#ifdef OCTARINE_WITH_EDITOR
    auto& gameConfig = registry->Get<GameConfig>();
    const bool projectLoaded = gameConfig.HasLoadedConfig();
    const bool showEditorUI = gameConfig.IsEditorMode() || !projectLoaded;
    if (showEditorUI) {
      auto* editorPersistence = registry->TryGet<EditorPersistence>();
      if (!editorPersistence || editorPersistence->showSceneWindow) {
        ImGui::SetWindowFocus("Scene View");
        viewportInfo.isFocused = true;
      } else {
        ImGui::SetWindowFocus(nullptr);
        viewportInfo.isFocused = false;
      }
    } else {
      ImGui::SetWindowFocus(nullptr);
      viewportInfo.isFocused = true;
      io.WantCaptureKeyboard = false;
      io.NavActive = false;
    }
#else
    ImGui::SetWindowFocus(nullptr);
    viewportInfo.isFocused = true;
    io.WantCaptureKeyboard = false;
    io.NavActive = false;
#endif
  } else {
    viewportInfo.isFocused = true;
  }

#else
  viewportInfo.isFocused = true;
#endif
}

namespace {
constexpr float kColorChannelMax = 255.0f;

ImVec4 ColorToImVec4(const octarine::Color& color) {
  return {static_cast<float>(color.r) / kColorChannelMax, static_cast<float>(color.g) / kColorChannelMax,
          static_cast<float>(color.b) / kColorChannelMax, static_cast<float>(color.a) / kColorChannelMax};
}
}  // namespace

void RenderDebugGUISystem::DrawDebugOverlays(Registry* registry, const EngineOptions& engineOptions,
                                             const bool showGameOverlays, const bool projectLoaded,
                                             [[maybe_unused]] const float deltaTime) {
  if (!showGameOverlays && !engineOptions.showFpsCounter) {
    return;
  }
  const ImVec4 bgColor = ColorToImVec4(engineOptions.debugOverlayBackgroundColor);
  ImGui::PushStyleColor(ImGuiCol_WindowBg, bgColor);

  if (showGameOverlays) {
    if (projectLoaded) {
      auto query = registry->CreateQuery<ScriptComponent>();
      RenderDebugGUISystem system;
      query->ForEach(system);
    }
    if (engineOptions.showEntityInfo) {
      EntityInfoWindow(registry);
    }
#ifdef OCTARINE_WITH_EDITOR
    FPSWindow(deltaTime);
#endif
  }
#ifdef OCTARINE_WITH_EDITOR
  else if (engineOptions.showFpsCounter) {
    FPSWindow(deltaTime);
  }
#endif

  ImGui::PopStyleColor();
}

void RenderDebugGUISystem::Render(Game* game, SDL_Renderer* renderer, [[maybe_unused]] SDL_Texture* gameTexture,
                                  [[maybe_unused]] const float deltaTime) {
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

  static bool wasShowingGameOverlays = false;
  const bool justClosedOverlays = wasShowingGameOverlays && !showGameOverlays;
  wasShowingGameOverlays = showGameOverlays;

  if (justClosedOverlays) {
    ReturnFocusToGame(game);
  }

#ifdef OCTARINE_WITH_EDITOR
  if (!showEditorUI && !showGameOverlays && !engineOptions.showFpsCounter && !toastActive && !justClosedOverlays) {
    return;
  }
#else
  // Player-with-ImGui build: the ImGui FPS window is editor-only (showFpsCounter is ignored here);
  // the player-facing FPS readout is the renderer perf overlay (PerfOverlay config). So nothing
  // ImGui draws unless the in-game debug overlays are toggled on or a script error is toasting.
  if (!showGameOverlays && !toastActive && !justClosedOverlays) {
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

  if (justClosedOverlays) {
    ReturnFocusToGame(game);
  }

#ifdef OCTARINE_WITH_EDITOR
  if (showEditorUI) {
    octarine::editor::panels::DrawEditorChrome(game, gameTexture);
  }
#endif

  DrawDebugOverlays(registry, engineOptions, showGameOverlays, projectLoaded, deltaTime);

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
  static constexpr int kFpsHistoryCount = 120;
  static constexpr float kFpsPlotScaleMax = 120.0f;
  static constexpr float kFpsPlotHeight = 50.0f;

  static float fpsHistory[kFpsHistoryCount] = {};
  static int fpsOffset = 0;
  const float fps = (deltaTime > 0.0f) ? 1.0f / deltaTime : 0.0f;
  fpsHistory[fpsOffset] = fps;
  fpsOffset = (fpsOffset + 1) % kFpsHistoryCount;

  ImGui::Begin("FPS", nullptr, ImGuiWindowFlags_NoNavInputs);
  char overlay[32];
  snprintf(overlay, sizeof(overlay), "%.1f FPS", static_cast<double>(fps));
  ImGui::PlotLines("##fps", fpsHistory, kFpsHistoryCount, fpsOffset, overlay, 0.0f, kFpsPlotScaleMax,
                   ImVec2(0, kFpsPlotHeight));
  ImGui::End();
}

void RenderDebugGUISystem::EntityInfoWindow(const Registry* registry) {
  const auto count = registry->GetUserEntityCount();
  ImGui::Begin("Entity Info", nullptr, ImGuiWindowFlags_NoNavInputs);
  ImGui::Text("Entity Count: %llu", static_cast<unsigned long long>(count));
  ImGui::End();
}

namespace {
// How long one script error stays on screen. Each new error restarts the clock for itself only,
// so a burst shows its tail and quiet errors age out individually.
constexpr double kScriptErrorToastSeconds = 10.0;
constexpr float kToastBgAlpha = 0.85f;
constexpr float kToastWrapWidthPx = 420.0f;
constexpr ImVec4 kToastHeaderColor{1.0f, 0.35f, 0.35f, 1.0f};

bool IsToastFresh(const Logger::ScriptError& error) {
  const auto age =
      std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - error.when);
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
  ImGui::SetNextWindowBgAlpha(kToastBgAlpha);
  constexpr ImGuiWindowFlags kFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                                      ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                                      ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoInputs |
                                      ImGuiWindowFlags_AlwaysAutoResize;
  ImGui::Begin("##script-error-toast", nullptr, kFlags);
  ImGui::TextColored(kToastHeaderColor, "Script error%s (%d)", fresh.size() == 1 ? "" : "s",
                     static_cast<int>(fresh.size()));
  ImGui::Separator();
  ImGui::PushTextWrapPos(kToastWrapWidthPx);
  for (const auto* error : fresh) {
    ImGui::TextWrapped("%s", error->message.c_str());
  }
  ImGui::PopTextWrapPos();
  ImGui::End();
}
#endif
