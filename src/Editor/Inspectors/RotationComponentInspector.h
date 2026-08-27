#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <imgui.h>

#include <optional>

#include "Components/RotationComponent.h"
#include "Editor/Inspectors/EditorInspector.h"
#include "General/AngleUnit.h"

template <>
struct EditorInspector<RotationComponent> {
  static constexpr const char* kDisplayName = "Rotation";

  static void draw(Registry* /*registry*/, Entity /*entity*/, RotationComponent& rotationComp) {
    // Dragged and shown in the configured AngleUnit; storage stays radians.
    float authored = octarine::AngleUnits::ToAuthored(rotationComp.value);
    if (ImGui::DragFloat(octarine::AngleUnits::IsDegrees() ? "Rotation (deg)" : "Rotation (rad)", &authored)) {
      rotationComp.value = octarine::AngleUnits::ToRadians(authored);
    }
  }
  static std::optional<RotationComponent> makeDefault() { return RotationComponent{}; }
};

#endif  // OCTARINE_WITH_EDITOR
