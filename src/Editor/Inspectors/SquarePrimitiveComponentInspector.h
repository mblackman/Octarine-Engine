#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <imgui.h>

#include <optional>

#include "Components/SquarePrimitiveComponent.h"
#include "Editor/Inspectors/EditorInspector.h"
#include "Editor/Inspectors/InspectorWidgets.h"

template <>
struct EditorInspector<SquarePrimitiveComponent> {
  static constexpr const char* kDisplayName = "SquarePrimitive";
  static void draw(Registry* /*registry*/, Entity /*entity*/, SquarePrimitiveComponent& sp) {
    ImGui::DragFloat2("Position", &sp.position.x);
    ImGui::DragInt("Layer", &sp.layer);
    ImGui::DragFloat("Width", &sp.width);
    ImGui::DragFloat("Height", &sp.height);
    octarine::editor::inspectors::DrawColorEdit("Color", sp.color);
    ImGui::Checkbox("Fixed", &sp.isFixed);
  }
  static std::optional<SquarePrimitiveComponent> makeDefault() { return SquarePrimitiveComponent{}; }
};

#endif  // OCTARINE_WITH_EDITOR
