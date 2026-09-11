#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <imgui.h>

#include <optional>

#include "Components/ColorGridComponent.h"
#include "Editor/Inspectors/EditorInspector.h"
#include "Editor/Inspectors/InspectorWidgets.h"

template <>
struct EditorInspector<ColorGridComponent> {
  static constexpr const char* kDisplayName = "ColorGrid";
  static constexpr float kMinCellSize = ColorGridComponent::kMinCellSize;
  static constexpr float kMaxCellSize = 1024.0f;
  static void draw(Registry* /*registry*/, Entity /*entity*/, ColorGridComponent& cg) {
    ImGui::DragFloat("Cell Width", &cg.cellWidth, 1.0f, kMinCellSize, kMaxCellSize);
    ImGui::DragFloat("Cell Height", &cg.cellHeight, 1.0f, kMinCellSize, kMaxCellSize);
    octarine::editor::inspectors::DrawColorEdit("Color 1", cg.color1);
    octarine::editor::inspectors::DrawColorEdit("Color 2", cg.color2);
    ImGui::DragFloat2("Offset", &cg.offset.x);
    ImGui::DragFloat2("Scroll Velocity", &cg.scrollVelocity.x);
    ImGui::DragFloat2("Bounds", &cg.bounds.x);
    ImGui::DragInt("Layer", &cg.layer);
    ImGui::Checkbox("Fixed", &cg.isFixed);
  }
  static std::optional<ColorGridComponent> makeDefault() { return ColorGridComponent{}; }
};

#endif  // OCTARINE_WITH_EDITOR
