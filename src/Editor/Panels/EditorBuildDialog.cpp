#include "Editor/Panels/EditorBuildDialog.h"

#ifdef OCTARINE_WITH_EDITOR

#include <cstdio>
#include <filesystem>
#include <string>
#include <vector>

#include "AssetManager/AssetManager.h"
#include "Editor/EditorLayoutPresets.h"
#include "Editor/EditorPersistence.h"
#include "Editor/ExportBuilder.h"
#include "Game/Game.h"
#include "Project/ProjectIni.h"
#include "imgui.h"

namespace octarine::editor::panels {

void DrawBuildDialogs(Game* game, bool openSaveLayoutModal, bool openExportBuildModal) {
  auto* registry = game->GetRegistry();
  auto& editorPersistence = registry->Get<EditorPersistence>();
  auto& exportBuilder = registry->Get<octarine::editor::ExportBuilder>();

  if (openSaveLayoutModal) {
    ImGui::OpenPopup("Save Layout Preset");
  }
  if (openExportBuildModal) {
    ImGui::OpenPopup("Export Build");
  }

  if (ImGui::BeginPopupModal("Export Build", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
    static char versionNameBuf[32] = "";
    static char versionCodeBuf[16] = "";
    static std::string lastProjectDir;
    static std::vector<std::string> validationErrors;
    static int targetIdx = 0;  // 0=Desktop, 1=AndroidDebug, 2=AndroidRelease

    constexpr const char* kTargetLabels[] = {
        "Desktop (host OS, ship-release)",
        "Android (debug APK)",
        "Android (release AAB)",
    };
    const auto targetForIdx = [](int idx) {
      switch (idx) {
        case 1:
          return octarine::editor::ExportTarget::AndroidDebug;
        case 2:
          return octarine::editor::ExportTarget::AndroidRelease;
        default:
          return octarine::editor::ExportTarget::Desktop;
      }
    };

    const std::string projectDir = registry->Get<AssetManager>().GetBasePath();

    // Refresh the version-field defaults and validation when the project switches under the
    // modal. The buffers themselves are static so a dev can edit and have the override stick
    // across reopens within the same project.
    if (projectDir != lastProjectDir) {
      lastProjectDir = projectDir;
      versionNameBuf[0] = '\0';
      versionCodeBuf[0] = '\0';
      if (!projectDir.empty()) {
        if (auto ini = octarine::project::ProjectIni::Load(std::filesystem::path(projectDir))) {
          std::snprintf(versionNameBuf, sizeof(versionNameBuf), "%s", ini->version_name.c_str());
          std::snprintf(versionCodeBuf, sizeof(versionCodeBuf), "%s", ini->version_code.c_str());
        }
      }
      validationErrors =
          octarine::editor::ExportBuilder::Validate(std::filesystem::path(projectDir), targetForIdx(targetIdx));
    }

    ImGui::TextDisabled("Project: %s", projectDir.empty() ? "(none)" : projectDir.c_str());
    ImGui::SetNextItemWidth(260);
    if (ImGui::Combo("Target", &targetIdx, kTargetLabels, IM_ARRAYSIZE(kTargetLabels))) {
      validationErrors =
          octarine::editor::ExportBuilder::Validate(std::filesystem::path(projectDir), targetForIdx(targetIdx));
    }
    ImGui::Separator();

    ImGui::SetNextItemWidth(180);
    ImGui::InputText("Version name", versionNameBuf, sizeof(versionNameBuf));
    ImGui::SetNextItemWidth(180);
    ImGui::InputText("Version code", versionCodeBuf, sizeof(versionCodeBuf));
    ImGui::TextDisabled("Leave blank to fall through to project.ini.");

    if (targetIdx == 2) {
      ImGui::Separator();
      ImGui::TextColored(ImVec4(1.0F, 0.85F, 0.4F, 1.0F), "Android-release expects signing creds in env:");
      ImGui::BulletText("OCTARINE_ANDROID_KEYSTORE_PATH");
      ImGui::BulletText("OCTARINE_ANDROID_STORE_PASSWORD");
      ImGui::BulletText("OCTARINE_ANDROID_KEY_ALIAS");
      ImGui::BulletText("OCTARINE_ANDROID_KEY_PASSWORD");
      ImGui::TextDisabled("Unset = falls back to the Gradle debug key. In-editor secret storage lands in PR-C.");
    }

    if (!validationErrors.empty()) {
      ImGui::Separator();
      ImGui::TextColored(ImVec4(1.0F, 0.5F, 0.5F, 1.0F), "Validation errors:");
      for (const auto& e : validationErrors) {
        ImGui::BulletText("%s", e.c_str());
      }
    }

    ImGui::Separator();

    const bool canBuild = validationErrors.empty() && !projectDir.empty();
    ImGui::BeginDisabled(!canBuild);
    if (ImGui::Button("Build")) {
      octarine::editor::ExportOptions opts;
      opts.project_dir = std::filesystem::path(projectDir);
      opts.target = targetForIdx(targetIdx);
      opts.version_name = versionNameBuf;
      opts.version_code = versionCodeBuf;

      editorPersistence.showExportOutput = true;
      exportBuilder.ClearLog();
      exportBuilder.Run(opts);
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndDisabled();
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("Re-validate")) {
      validationErrors =
          octarine::editor::ExportBuilder::Validate(std::filesystem::path(projectDir), targetForIdx(targetIdx));
    }
    ImGui::EndPopup();
  }

  if (ImGui::BeginPopupModal("Save Layout Preset", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
    static char nameBuf[64] = "";
    ImGui::Text("Preset name:");
    ImGui::SetNextItemWidth(260);
    const bool enter = ImGui::InputText("##layoutname", nameBuf, sizeof(nameBuf), ImGuiInputTextFlags_EnterReturnsTrue);
    const bool hasName = nameBuf[0] != '\0';
    ImGui::BeginDisabled(!hasName);
    if (ImGui::Button("Save") || (enter && hasName)) {
      octarine::editor::layouts::SavePreset(nameBuf, editorPersistence);
      nameBuf[0] = '\0';
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndDisabled();
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      nameBuf[0] = '\0';
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}

}  // namespace octarine::editor::panels

#endif  // OCTARINE_WITH_EDITOR
