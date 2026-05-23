#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <imgui.h>

#include <optional>

#include "Components/ScaleComponent.h"
#include "Editor/Inspectors/EditorInspector.h"

template <>
struct EditorInspector<ScaleComponent> {
  static constexpr const char* kDisplayName = "Scale";
  static void draw(Registry* /*registry*/, Entity /*entity*/, ScaleComponent& scale) {
    ImGui::DragFloat2("Scale", &scale.value.x);
  }
  static std::optional<ScaleComponent> makeDefault() { return ScaleComponent{}; }
};

#endif  // OCTARINE_WITH_EDITOR
