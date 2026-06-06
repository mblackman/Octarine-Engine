#include "Editor/Panels/EditorToolbarPanel.h"

#ifdef OCTARINE_WITH_EDITOR

#include <filesystem>
#include <string>

#include "AssetManager/AssetManager.h"
#include "Editor/EditorLayoutPresets.h"
#include "Editor/EditorPersistence.h"
#include "Editor/ExportBuilder.h"
#include "Editor/PlayerLauncher.h"
#include "Game/Game.h"
#include "Game/GameConfig.h"
#include "imgui.h"
#include "imgui_internal.h"  // BeginViewportSideBar (playback toolbar pinned under the menu bar)

namespace octarine::editor::panels {

void DrawToolbar(Game* game, bool& showProjectSelector, bool& openSaveLayoutModal, bool& openExportBuildModal) {
  auto* registry = game->GetRegistry();
  auto& gameConfig = registry->Get<GameConfig>();
  auto& engineOptions = gameConfig.GetEngineOptions();
  auto& editorPersistence = registry->Get<EditorPersistence>();
  auto& playerLauncher = registry->Get<octarine::editor::PlayerLauncher>();
  auto& exportBuilder = registry->Get<octarine::editor::ExportBuilder>();
  const bool projectLoaded = gameConfig.HasLoadedConfig();

  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Open Project...", "Ctrl+O")) {
        showProjectSelector = true;
      }
      if (ImGui::MenuItem("Save Preferences", "Ctrl+S")) {
        gameConfig.SaveUserPreferences();
        editorPersistence.audioMuted = !engineOptions.audioEnabled;
        editorPersistence.masterVolume = engineOptions.masterVolume;
        editorPersistence.SaveGlobal();
        if (projectLoaded) editorPersistence.SaveProject(gameConfig.GetAssetPath());
      }
      ImGui::Separator();
      if (ImGui::MenuItem("Quit", "Alt+F4")) {
        Game::Quit();
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Layout")) {
      const auto presets = octarine::editor::layouts::ListPresets();
      if (ImGui::BeginMenu("Apply", !presets.empty())) {
        for (const auto& name : presets) {
          if (ImGui::MenuItem(name.c_str())) {
            editorPersistence.pendingLayoutLoad = name;
          }
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Delete", !presets.empty())) {
        for (const auto& name : presets) {
          if (ImGui::MenuItem(name.c_str())) {
            octarine::editor::layouts::DeletePreset(name);
          }
        }
        ImGui::EndMenu();
      }
      ImGui::Separator();
      if (ImGui::MenuItem("Save Current As...")) {
        openSaveLayoutModal = true;
      }
      if (ImGui::MenuItem("Reset to Default")) {
        editorPersistence.pendingLayoutLoad = "__default__";
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Windows")) {
      ImGui::MenuItem("Scene View", nullptr, &editorPersistence.showSceneWindow);
      ImGui::MenuItem("Scene Management", nullptr, &editorPersistence.showSceneManagement);
      ImGui::MenuItem("Hierarchy", nullptr, &editorPersistence.showHierarchy);
      ImGui::MenuItem("Asset Browser", nullptr, &editorPersistence.showAssetBrowser);
      ImGui::MenuItem("Lua Console", nullptr, &editorPersistence.showLuaConsole);
      ImGui::MenuItem("Performance Profiler", nullptr, &editorPersistence.showProfiler);
      ImGui::MenuItem("Engine Options", nullptr, &editorPersistence.showEngineOptions);
      ImGui::MenuItem("Editor Settings", nullptr, &editorPersistence.showEditorSettings);
      ImGui::MenuItem("Player Output", nullptr, &editorPersistence.showPlayerOutput);
      ImGui::MenuItem("Export Output", nullptr, &editorPersistence.showExportOutput);
      ImGui::MenuItem("Signing Settings", nullptr, &editorPersistence.showSigningSettings);
      ImGui::MenuItem("Devices", nullptr, &editorPersistence.showDevices);
      ImGui::MenuItem("Game Debug Overlays", "Grave", &engineOptions.showDebugGUI);
      ImGui::Separator();
      ImGui::MenuItem("FPS Counter", nullptr, &engineOptions.showFpsCounter);
      ImGui::MenuItem("ImGui Demo Window", nullptr, &engineOptions.showImGuiDemoWindow);
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }

  // Playback toolbar: a fixed full-width bar pinned directly under the main menu bar.
  // Submitting it as an Up side-bar after the menu bar stacks it beneath the menu.
  {
    ImGuiViewport* vp = ImGui::GetMainViewport();
    if (ImGui::BeginViewportSideBar("##PlaybackToolbar", vp, ImGuiDir_Up, ImGui::GetFrameHeight(),
                                    ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar)) {
      if (ImGui::BeginMenuBar()) {
        const bool hasScene = !editorPersistence.currentScenePath.empty();

        // Play — resume execution. If a configured scene was stopped, (re)start it first.
        // Disabled while already running so the button reads as the current state.
        ImGui::BeginDisabled(!engineOptions.isPaused);
        if (ImGui::Button("Play")) {
          if (!game->IsSceneRunning() && hasScene) {
            game->ReloadScene();
          }
          engineOptions.isPaused = false;
        }
        ImGui::EndDisabled();
        ImGui::SameLine();
        ImGui::BeginDisabled(engineOptions.isPaused);
        if (ImGui::Button("Pause")) {
          engineOptions.isPaused = true;
        }
        ImGui::EndDisabled();
        ImGui::SameLine();
        if (ImGui::Button("Step"))  // freeze, then advance exactly one frame
        {
          engineOptions.isPaused = true;
          engineOptions.stepFrame = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Stop")) {
          game->StopScene();
          engineOptions.isPaused = true;  // back to the default ready-but-paused state
        }

        ImGui::SameLine();
        if (ImGui::Button(engineOptions.audioEnabled ? "Mute" : "Unmute")) {
          engineOptions.audioEnabled = !engineOptions.audioEnabled;
          editorPersistence.audioMuted = !engineOptions.audioEnabled;
          editorPersistence.masterVolume = engineOptions.masterVolume;
          editorPersistence.SaveGlobal();  // persist immediately
        }

        ImGui::SameLine();
        // Run Player / Stop Player: spawns the standalone OctarineEngine-player process
        // pointed at the currently-loaded project. Separate from the embedded Play button
        // above, which only resumes the in-editor scene.
        {
          const bool playerRunning = playerLauncher.Status() == octarine::editor::PlayerStatus::Running;
          const bool canRun = projectLoaded || !registry->Get<AssetManager>().GetBasePath().empty();
          ImGui::BeginDisabled(!playerRunning && !canRun);
          if (ImGui::Button(playerRunning ? "Stop Player" : "Run Player")) {
            if (playerRunning) {
              playerLauncher.Stop();
            } else {
              // Player Output panel auto-opens so the dev sees launch / spawn errors.
              editorPersistence.showPlayerOutput = true;
              const std::string projectDir = registry->Get<AssetManager>().GetBasePath();
              playerLauncher.Run(std::filesystem::path(projectDir));
            }
          }
          ImGui::EndDisabled();
        }

        ImGui::SameLine();
        // Export Build: opens a modal to spawn the project's scaffolded
        // scripts/build-desktop.{sh,ps1}. Disabled while a build is in flight; reads as
        // "Stop Export" so the dev can cancel.
        {
          const bool exportRunning = exportBuilder.Status() == octarine::editor::ExportStatus::Building;
          const bool canExport = projectLoaded || !registry->Get<AssetManager>().GetBasePath().empty();
          ImGui::BeginDisabled(!exportRunning && !canExport);
          if (ImGui::Button(exportRunning ? "Stop Export" : "Export Build...")) {
            if (exportRunning) {
              exportBuilder.Stop();
            } else {
              openExportBuildModal = true;
            }
          }
          ImGui::EndDisabled();
        }
        ImGui::EndMenuBar();
      }
    }
    ImGui::End();
  }
}

}  // namespace octarine::editor::panels

#endif  // OCTARINE_WITH_EDITOR
