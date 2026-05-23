#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <imgui.h>

#include <optional>

#include "Components/RotationComponent.h"
#include "Editor/Inspectors/EditorInspector.h"

template <>
struct EditorInspector<RotationComponent> {
  static constexpr const char* kDisplayName = "Rotation";
  static void draw(Registry* /*registry*/, Entity /*entity*/, RotationComponent& rotationComp) {
    auto rotation = static_cast<float>(rotationComp.value);
    if (ImGui::DragFloat("Rotation", &rotation)) {
      rotationComp.value = rotation;
    }
  }
  static std::optional<RotationComponent> makeDefault() { return RotationComponent{}; }
};

#endif  // OCTARINE_WITH_EDITOR
