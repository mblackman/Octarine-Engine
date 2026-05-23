#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <imgui.h>

#include <optional>

#include "Components/RigidBodyComponent.h"
#include "Editor/Inspectors/EditorInspector.h"

template <>
struct EditorInspector<RigidBodyComponent> {
  static constexpr const char* kDisplayName = "RigidBody";
  static void draw(Registry* /*registry*/, Entity /*entity*/, RigidBodyComponent& rb) {
    ImGui::DragFloat2("Velocity", &rb.velocity.x, 1.0F);
  }
  static std::optional<RigidBodyComponent> makeDefault() { return RigidBodyComponent{}; }
};

#endif  // OCTARINE_WITH_EDITOR
