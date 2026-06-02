#include "Editor/Panels/EditorPanelHost.h"

#ifdef OCTARINE_WITH_EDITOR

#include "Editor/EditorLayoutPresets.h"
#include "Editor/EditorPersistence.h"
#include "Editor/Panels/EditorAssetBrowserPanel.h"
#include "Editor/Panels/EditorBuildDialog.h"
#include "Editor/Panels/EditorConsolePanel.h"
#include "Editor/Panels/EditorHierarchyPanel.h"
#include "Editor/Panels/EditorImGuiBackend.h"
#include "Editor/Panels/EditorOutputPanels.h"
#include "Editor/Panels/EditorProjectPanels.h"
#include "Editor/Panels/EditorSceneWindow.h"
#include "Editor/Panels/EditorSettingsPanels.h"
#include "Editor/Panels/EditorSigningPanel.h"
#include "Editor/Panels/EditorToolbarPanel.h"
#include "Game/Game.h"
#include "Game/GameConfig.h"
#include "imgui.h"

namespace octarine::editor::panels {
namespace {
// Project Selector visibility. Toggled by File > Open Project inside DrawEditorChrome and consumed
// by DrawProjectSelectorIfNeeded, which runs in a separate hook later in the same frame — so the
// flag lives at TU scope rather than as a local in either function.
bool g_showProjectSelector = false;
}  // namespace

void ApplyPendingLayout(EditorPersistence& persistence) {
  if (!persistence.pendingLayoutLoad.empty()) {
    if (persistence.pendingLayoutLoad == "__default__") {
      octarine::editor::layouts::ApplyDefaultPreset(persistence);
    } else {
      octarine::editor::layouts::LoadPreset(persistence.pendingLayoutLoad, persistence);
    }
    persistence.pendingLayoutLoad.clear();
  }
}

void DrawEditorChrome(Game* game, SDL_Texture* gameTexture) {
  auto* registry = game->GetRegistry();
  auto& editorPersistence = registry->Get<EditorPersistence>();
  auto& engineOptions = registry->Get<GameConfig>().GetEngineOptions();

  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
  ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
  ImGui::PopStyleColor();

  // Toolbar raises these when the user requests a modal; DrawBuildDialogs opens + draws them.
  bool openSaveLayoutModal = false;
  bool openExportBuildModal = false;
  DrawToolbar(game, g_showProjectSelector, openSaveLayoutModal, openExportBuildModal);
  DrawBuildDialogs(game, openSaveLayoutModal, openExportBuildModal);

  if (editorPersistence.showSceneWindow) DrawSceneWindow(game, gameTexture, &editorPersistence.showSceneWindow);
  if (editorPersistence.showSceneManagement) DrawSceneManagementWindow(game);
  if (editorPersistence.showHierarchy) DrawHierarchyWindow(registry);
  if (editorPersistence.showAssetBrowser) DrawAssetBrowserWindow(registry);
  if (editorPersistence.showLuaConsole) DrawLuaConsoleWindow(game->GetLua());
  if (editorPersistence.showProfiler) DrawPerformanceProfilerWindow();
  if (editorPersistence.showEngineOptions) DrawEngineOptionsWindow(engineOptions, &editorPersistence.showEngineOptions);
  if (editorPersistence.showEditorSettings)
    DrawEditorSettingsWindow(editorPersistence, &editorPersistence.showEditorSettings);
  if (editorPersistence.showPlayerOutput) DrawPlayerOutputWindow(game, &editorPersistence.showPlayerOutput);
  if (editorPersistence.showExportOutput) DrawExportOutputWindow(game, &editorPersistence.showExportOutput);
  if (editorPersistence.showSigningSettings) DrawSigningSettingsWindow(&editorPersistence.showSigningSettings);

  if (engineOptions.showImGuiDemoWindow) {
    ImGui::ShowDemoWindow();
  }
}

void DrawProjectSelectorIfNeeded(Game* game, bool projectLoaded) {
  if (!projectLoaded || g_showProjectSelector) {
    DrawProjectSelectorWindow(game, &g_showProjectSelector);
  }
}

void ProcessFontRebuild(EditorPersistence& persistence) {
  // Honor any deferred font-rebuild requests now that the ImGui frame has ended.
  // Atlas rebuilds inside an active frame would corrupt draw state.
  if (persistence.fontRebuildPending) {
    RebuildEditorFont(persistence.editorFontSize);
    ApplyEditorStyle(persistence.editorStyleIndex, persistence.editorFontSize);
    persistence.fontRebuildPending = false;
  }
}

}  // namespace octarine::editor::panels

#endif  // OCTARINE_WITH_EDITOR
