#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <imgui.h>

#include <optional>

#include "Components/EntityMaskComponent.h"
#include "Editor/Inspectors/EditorInspector.h"

template <>
struct EditorInspector<EntityMaskComponent> {
  static constexpr const char* kDisplayName = "EntityMask";
  static void draw(Registry* /*registry*/, Entity /*entity*/, EntityMaskComponent& em) {
    ImGui::Text("Mask: %s", em.mask.to_string().c_str());
  }
  static std::optional<EntityMaskComponent> makeDefault() { return std::nullopt; }
};

#endif  // OCTARINE_WITH_EDITOR
