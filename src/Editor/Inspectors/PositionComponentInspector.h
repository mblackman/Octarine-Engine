#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <imgui.h>

#include <optional>

#include "Components/PositionComponent.h"
#include "Editor/Inspectors/EditorInspector.h"

template <>
struct EditorInspector<PositionComponent> {
  static constexpr const char* kDisplayName = "Position";
  static void draw(Registry* /*registry*/, Entity /*entity*/, PositionComponent& position) {
    ImGui::DragFloat2("Position", &position.value.x);
  }
  static std::optional<PositionComponent> makeDefault() { return PositionComponent{}; }
};

#endif  // OCTARINE_WITH_EDITOR
