#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <imgui.h>

#include <optional>

#include "Components/GlobalTransformComponent.h"
#include "Editor/Inspectors/EditorInspector.h"

template <>
struct EditorInspector<GlobalTransformComponent> {
  static constexpr const char* kDisplayName = "Global Transform";
  static void draw(Registry* /*registry*/, Entity /*entity*/, GlobalTransformComponent& global) {
    ImGui::Text("Global Pos: %.2f, %.2f", global.position.x, global.position.y);
    ImGui::Text("Global Scale: %.2f, %.2f", global.scale.x, global.scale.y);
    ImGui::Text("Global Rot: %.2f", global.rotation);
  }
  static std::optional<GlobalTransformComponent> makeDefault() { return GlobalTransformComponent{}; }
};

#endif  // OCTARINE_WITH_EDITOR
