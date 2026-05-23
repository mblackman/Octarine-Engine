#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <imgui.h>

#include <optional>

#include "Components/CameraFollowComponent.h"
#include "Editor/Inspectors/EditorInspector.h"

template <>
struct EditorInspector<CameraFollowComponent> {
  static constexpr const char* kDisplayName = "CameraFollow";
  static void draw(Registry* /*registry*/, Entity /*entity*/, CameraFollowComponent& /*cf*/) {
    ImGui::Text("Entity is being followed by the camera.");
  }
  static std::optional<CameraFollowComponent> makeDefault() { return CameraFollowComponent{}; }
};

#endif  // OCTARINE_WITH_EDITOR
