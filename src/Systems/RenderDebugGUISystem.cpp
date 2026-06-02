#include "RenderDebugGUISystem.h"

#ifdef OCTARINE_WITH_IMGUI
#include "../Game/Game.h"
#include "../Game/GameConfig.h"
#include "../General/PerfUtils.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

#ifdef OCTARINE_WITH_EDITOR
#include "../Editor/EditorPersistence.h"
#include "../Editor/ExportBuilder.h"
#include "../Editor/Panels/EditorPanelHost.h"
#include "../Editor/PlayerLauncher.h"
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
  const bool showEditorUI = gameConfig.IsEditorMode() || !projectLoaded;
#endif
  const bool showGameOverlays = engineOptions.showDebugGUI;

#ifdef OCTARINE_WITH_EDITOR
  if (!showEditorUI && !showGameOverlays && !engineOptions.showFpsCounter) {
    return;
  }
#else
  if (!showGameOverlays && !engineOptions.showFpsCounter) {
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
    FPSWindow(deltaTime);
  } else if (engineOptions.showFpsCounter) {
    FPSWindow(deltaTime);
  }

#ifdef OCTARINE_WITH_EDITOR
  octarine::editor::panels::DrawProjectSelectorIfNeeded(game, projectLoaded);
#endif

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
#endif
