#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <imgui.h>

#include <optional>

#include "Components/BoxColliderComponent.h"
#include "Editor/Inspectors/EditorInspector.h"

template <>
struct EditorInspector<BoxColliderComponent> {
  static constexpr const char* kDisplayName = "BoxCollider";
  static void draw(Registry* /*registry*/, Entity /*entity*/, BoxColliderComponent& bc) {
    ImGui::DragInt("Width", &bc.width);
    ImGui::DragInt("Height", &bc.height);
    ImGui::DragFloat2("Offset", &bc.offset.x, 1.0F);
  }
  static std::optional<BoxColliderComponent> makeDefault() { return BoxColliderComponent{}; }
};

#endif  // OCTARINE_WITH_EDITOR
