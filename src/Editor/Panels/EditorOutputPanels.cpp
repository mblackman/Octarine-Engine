#include "Editor/Panels/EditorOutputPanels.h"

#ifdef OCTARINE_WITH_EDITOR

#include "Editor/ExportBuilder.h"
#include "Editor/PlayerLauncher.h"
#include "Game/Game.h"
#include "imgui.h"

namespace octarine::editor::panels {

void DrawPlayerOutputWindow(Game* game, bool* p_open) {
  auto& launcher = game->GetRegistry()->Get<octarine::editor::PlayerLauncher>();

  ImGui::SetNextWindowSize(ImVec2(560, 400), ImGuiCond_FirstUseEver);
  if (!ImGui::Begin("Player Output", p_open, ImGuiWindowFlags_NoScrollbar)) {
    ImGui::End();
    return;
  }

  const auto status = launcher.Status();
  const char* statusText = "Idle";
  ImVec4 statusColor = ImVec4(0.7F, 0.7F, 0.7F, 1.0F);
  switch (status) {
    case octarine::editor::PlayerStatus::Running:
      statusText = "Running";
      statusColor = ImVec4(0.4F, 1.0F, 0.4F, 1.0F);
      break;
    case octarine::editor::PlayerStatus::Exited:
      statusText = "Exited";
      statusColor = ImVec4(1.0F, 1.0F, 0.4F, 1.0F);
      break;
    case octarine::editor::PlayerStatus::FailedToLaunch:
      statusText = "Failed to launch";
      statusColor = ImVec4(1.0F, 0.4F, 0.4F, 1.0F);
      break;
    case octarine::editor::PlayerStatus::Idle:
    default:
      break;
  }
  ImGui::TextDisabled("Status:");
  ImGui::SameLine();
  ImGui::TextColored(statusColor, "%s", statusText);
  if (const auto code = launcher.LastExitCode()) {
    ImGui::SameLine();
    ImGui::TextDisabled("(exit %d)", *code);
  }
  if (!launcher.ResolvedBinary().empty()) {
    ImGui::TextDisabled("Binary: %s", launcher.ResolvedBinary().string().c_str());
  }

  if (ImGui::Button("Clear")) {
    launcher.ClearLog();
  }
  ImGui::SameLine();
  static bool autoScroll = true;
  ImGui::Checkbox("Auto-scroll", &autoScroll);

  ImGui::Separator();

  ImGui::BeginChild("PlayerOutputScroll", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
  const auto snap = launcher.LogCopy();
  if (snap.total_dropped > 0) {
    ImGui::TextDisabled("[older %zu lines dropped]", snap.total_dropped);
  }
  for (const auto& line : snap.lines) {
    if (line.is_stderr) {
      ImGui::TextColored(ImVec4(1.0F, 0.5F, 0.5F, 1.0F), "%s", line.text.c_str());
    } else {
      ImGui::TextUnformatted(line.text.c_str());
    }
  }
  if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
    ImGui::SetScrollHereY(1.0F);
  }
  ImGui::EndChild();

  ImGui::End();
}

void DrawExportOutputWindow(Game* game, bool* p_open) {
  auto& builder = game->GetRegistry()->Get<octarine::editor::ExportBuilder>();

  ImGui::SetNextWindowSize(ImVec2(620, 420), ImGuiCond_FirstUseEver);
  if (!ImGui::Begin("Export Output", p_open, ImGuiWindowFlags_NoScrollbar)) {
    ImGui::End();
    return;
  }

  const auto status = builder.Status();
  const char* statusText = "Idle";
  ImVec4 statusColor = ImVec4(0.7F, 0.7F, 0.7F, 1.0F);
  switch (status) {
    case octarine::editor::ExportStatus::Building:
      statusText = "Building";
      statusColor = ImVec4(0.4F, 1.0F, 0.4F, 1.0F);
      break;
    case octarine::editor::ExportStatus::Succeeded:
      statusText = "Succeeded";
      statusColor = ImVec4(0.4F, 1.0F, 0.4F, 1.0F);
      break;
    case octarine::editor::ExportStatus::Failed:
      statusText = "Failed";
      statusColor = ImVec4(1.0F, 0.4F, 0.4F, 1.0F);
      break;
    case octarine::editor::ExportStatus::Idle:
    default:
      break;
  }
  ImGui::TextDisabled("Status:");
  ImGui::SameLine();
  ImGui::TextColored(statusColor, "%s", statusText);
  if (const auto code = builder.LastExitCode()) {
    ImGui::SameLine();
    ImGui::TextDisabled("(exit %d)", *code);
  }
  if (!builder.ResolvedScript().empty()) {
    ImGui::TextDisabled("Script: %s", builder.ResolvedScript().string().c_str());
  }

  if (ImGui::Button("Clear")) {
    builder.ClearLog();
  }
  ImGui::SameLine();
  static bool autoScroll = true;
  ImGui::Checkbox("Auto-scroll", &autoScroll);

  ImGui::Separator();

  ImGui::BeginChild("ExportOutputScroll", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
  const auto snap = builder.LogCopy();
  if (snap.total_dropped > 0) {
    ImGui::TextDisabled("[older %zu lines dropped]", snap.total_dropped);
  }
  for (const auto& line : snap.lines) {
    if (line.is_stderr) {
      ImGui::TextColored(ImVec4(1.0F, 0.5F, 0.5F, 1.0F), "%s", line.text.c_str());
    } else {
      ImGui::TextUnformatted(line.text.c_str());
    }
  }
  if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
    ImGui::SetScrollHereY(1.0F);
  }
  ImGui::EndChild();

  ImGui::End();
}

}  // namespace octarine::editor::panels

#endif  // OCTARINE_WITH_EDITOR
