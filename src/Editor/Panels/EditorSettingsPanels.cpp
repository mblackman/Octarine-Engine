#include "Editor/Panels/EditorSettingsPanels.h"

#ifdef OCTARINE_WITH_EDITOR

#include "Editor/EditorPersistence.h"
#include "Editor/Panels/EditorImGuiBackend.h"
#include "Game/EngineOptions.h"
#include "General/PerfUtils.h"
#include "imgui.h"

namespace octarine::editor::panels {

void DrawEngineOptionsWindow(EngineOptions& options, bool* p_open) {
  // Runtime engine options only. Window show/hide lives in the Windows menu (single source of truth).
  ImGui::Begin("Engine Options", p_open);
  ImGui::Checkbox("Show ImGui Demo Window", &options.showImGuiDemoWindow);
  ImGui::Checkbox("Show FPS Counter", &options.showFpsCounter);
  ImGui::Checkbox("Show Entity Info", &options.showEntityInfo);
  ImGui::Checkbox("Draw Colliders", &options.drawColliders);
  ImGui::Checkbox("Log Input Events", &options.logInputEvents);
  ImGui::Checkbox("Audio Enabled", &options.audioEnabled);
  ImGui::SliderFloat("Master Volume", &options.masterVolume, 0.0F, 1.0F);
  ImGui::Separator();
  ImGui::Checkbox("Pause Execution", &options.isPaused);
  ImGui::SameLine();
  if (ImGui::Button("Step Frame")) {
    options.stepFrame = true;
  }
  ImGui::SliderFloat("Time Scale", &options.timeScale, 0.0F, 5.0F);
  ImGui::End();
}

void DrawEditorSettingsWindow(EditorPersistence& editorPersistence, bool* p_open) {
  ImGui::Begin("Editor Settings", p_open);

  ImGui::SeparatorText("Appearance");

  float fontSize = (editorPersistence.editorFontSize > 0.0F) ? editorPersistence.editorFontSize : kBaselineFontSize;
  if (ImGui::SliderFloat("Font Size", &fontSize, 10.0F, 40.0F, "%.0f px")) {
    editorPersistence.editorFontSize = fontSize;
  }
  // Defer the actual atlas rebuild until the user releases the slider — rebuilding
  // every frame while dragging is visibly laggy.
  if (ImGui::IsItemDeactivatedAfterEdit()) {
    editorPersistence.fontRebuildPending = true;
  }
  ImGui::SameLine();
  if (ImGui::Button("Reset")) {
    editorPersistence.editorFontSize = kBaselineFontSize;
    editorPersistence.fontRebuildPending = true;
  }

  const char* styles[] = {"Dark", "Light", "Classic"};
  if (ImGui::Combo("Theme", &editorPersistence.editorStyleIndex, styles, IM_ARRAYSIZE(styles))) {
    ApplyEditorStyle(editorPersistence.editorStyleIndex, editorPersistence.editorFontSize);
  }

  ImGui::End();
}

void DrawPerformanceProfilerWindow() {
  ImGui::Begin("Performance Profiler");
  const auto times = PerfUtils::ProfilingAccumulator::GetAccumulatedTimes();
  if (ImGui::BeginTable("profiler_table", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
    ImGui::TableSetupColumn("System/Scope");
    ImGui::TableSetupColumn("Time (ms)");
    ImGui::TableHeadersRow();

    for (const auto& [name, duration_us] : times) {
      ImGui::TableNextRow();
      ImGui::TableSetColumnIndex(0);
      ImGui::Text("%s", name.c_str());
      ImGui::TableSetColumnIndex(1);
      ImGui::Text("%.3f", static_cast<double>(duration_us) * 0.001);
    }
    ImGui::EndTable();
  }

  ImGui::Separator();
  const auto counters = PerfUtils::PerfCounters::GetCounters();
  if (!counters.empty() && ImGui::BeginTable("counters_table", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
    ImGui::TableSetupColumn("Counter");
    ImGui::TableSetupColumn("Value");
    ImGui::TableHeadersRow();

    for (const auto& [name, value] : counters) {
      ImGui::TableNextRow();
      ImGui::TableSetColumnIndex(0);
      ImGui::Text("%s", name.c_str());
      ImGui::TableSetColumnIndex(1);
      ImGui::Text("%lld", value);
    }
    ImGui::EndTable();
  }
  ImGui::End();
}

}  // namespace octarine::editor::panels

#endif  // OCTARINE_WITH_EDITOR
