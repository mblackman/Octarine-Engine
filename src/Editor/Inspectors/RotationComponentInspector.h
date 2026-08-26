#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <imgui.h>

#include <optional>

#include "Components/RotationComponent.h"
#include "Editor/Inspectors/EditorInspector.h"

template <>
struct EditorInspector<RotationComponent> {
  static constexpr const char* kDisplayName = "Rotation";
  // The pivot spans 0..1 across the whole entity, so it needs a far finer drag than the
  // pixel-valued fields other inspectors expose.
  static constexpr float kPivotDragSpeed = 0.01F;

  static void draw(Registry* /*registry*/, Entity /*entity*/, RotationComponent& rotationComp) {
    auto rotation = static_cast<float>(rotationComp.value);
    if (ImGui::DragFloat("Rotation", &rotation)) {
      rotationComp.value = rotation;
    }
    // Normalized against the entity's bounds: 0 = top/left edge, 0.5 = centre, 1 = bottom/right.
    // Unclamped on purpose — anchors outside the bounds are a legitimate way to orbit.
    ImGui::DragFloat2("Pivot", &rotationComp.pivot.x, kPivotDragSpeed);
    ImGui::Text("Pivot (local px): %.2f, %.2f", rotationComp.pivotOffset.x, rotationComp.pivotOffset.y);
  }
  static std::optional<RotationComponent> makeDefault() { return RotationComponent{}; }
};

#endif  // OCTARINE_WITH_EDITOR
