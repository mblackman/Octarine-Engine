#include "Editor/Panels/EditorProjectPanels.h"

#ifdef OCTARINE_WITH_EDITOR

#include <SDL3/SDL.h>

#include <string>

#include "Editor/EditorPersistence.h"
#include "Editor/Inspectors/InspectorWidgets.h"
#include "Editor/Panels/EditorDialogService.h"
#include "Game/Game.h"
#include "imgui.h"

namespace octarine::editor::panels {

void DrawSceneManagementWindow(Game* game) {
  auto* registry = game->GetRegistry();
  auto& editorPersistence = registry->Get<EditorPersistence>();

  ImGui::Begin("Scene Management", &editorPersistence.showSceneManagement);

  static std::string scenePath;
  static std::string lastKnownScenePath;
  if (lastKnownScenePath != editorPersistence.currentScenePath) {
    scenePath = editorPersistence.currentScenePath;
    lastKnownScenePath = editorPersistence.currentScenePath;
  }

  ImGui::Text("Scene Script Path:");
  // Reserve exactly the width of the trailing "..." + "Load" buttons so the input field doesn't
  // push them off-screen at large editor font sizes.
  const ImGuiStyle& style = ImGui::GetStyle();
  const float sceneButtonsWidth = ImGui::CalcTextSize("...").x + ImGui::CalcTextSize("Load").x +
                                  style.FramePadding.x * 4.0F + style.ItemSpacing.x * 2.0F;
  ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - sceneButtonsWidth);
  octarine::editor::inspectors::InputTextString("##scenepath", scenePath);
  ImGui::SameLine();
  if (ImGui::Button("...")) {
    const SDL_DialogFileFilter filters[] = {{"Lua Scripts", "lua"}, {"All Files", "*"}};
    SDL_ShowOpenFileDialog(OnSceneFileSelected, game, game->GetWindow(), filters, 2, nullptr, false);
  }
  ImGui::SameLine();
  if (ImGui::Button("Load")) {
    game->LoadScene(scenePath);
  }

  ImGui::Separator();

  if (!editorPersistence.currentScenePath.empty()) {
    ImGui::Text("Current Scene: %s", editorPersistence.currentScenePath.c_str());
    if (ImGui::Button("Reload")) {
      game->ReloadScene();
    }
    ImGui::SameLine();
    if (ImGui::Button("Stop")) {
      game->StopScene();
    }
  } else {
    ImGui::Text("No scene currently loaded.");
  }

  ImGui::Separator();
  ImGui::TextDisabled("Enter path relative to project root (e.g. scripts/level1.lua)");

  ImGui::End();
}

void DrawProjectSelectorWindow(Game* game, bool* p_open) {
  ImGui::SetNextWindowSize(ImVec2(400, 240), ImGuiCond_FirstUseEver);
  if (!ImGui::Begin("Project Selector", p_open, ImGuiWindowFlags_NoDocking)) {
    ImGui::End();
    return;
  }
  auto& editorPersistence = game->GetRegistry()->Get<EditorPersistence>();
  static std::string projectPath;
  static std::string lastKnownProjectPath;
  if (lastKnownProjectPath != editorPersistence.lastProjectPath) {
    projectPath = editorPersistence.lastProjectPath;
    lastKnownProjectPath = editorPersistence.lastProjectPath;
  }

  ImGui::Text("Enter Project Path:");
  // Reserve the trailing "..." button width so the input scales with the editor font size.
  const ImGuiStyle& style = ImGui::GetStyle();
  const float browseButtonWidth = ImGui::CalcTextSize("...").x + style.FramePadding.x * 2.0F + style.ItemSpacing.x;
  ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - browseButtonWidth);
  octarine::editor::inspectors::InputTextString("##path", projectPath);
  ImGui::SameLine();
  if (ImGui::Button("...")) {
    SDL_ShowOpenFolderDialog(OnProjectFolderSelected, game, game->GetWindow(), nullptr, false);
  }

  if (ImGui::Button("Open Project", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
    editorPersistence.lastProjectPath = projectPath;
    editorPersistence.SaveGlobal();
    *p_open = false;
    ImGui::OpenPopup("Restart Required");
  }
  if (ImGui::BeginPopupModal("Restart Required", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("Project path saved. Please restart the engine to load the new project.");
    if (ImGui::Button("OK", ImVec2(120, 0))) ImGui::CloseCurrentPopup();
    ImGui::EndPopup();
  }
  if (!editorPersistence.lastProjectPath.empty()) {
    ImGui::Separator();
    ImGui::Text("Last Project: %s", editorPersistence.lastProjectPath.c_str());
    if (ImGui::Button("Load Last Project")) projectPath = editorPersistence.lastProjectPath;
  }
  ImGui::End();
}

}  // namespace octarine::editor::panels

#endif  // OCTARINE_WITH_EDITOR
