#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <imgui.h>

#include <optional>

#include "Components/UIButtonComponent.h"
#include "Editor/Inspectors/EditorInspector.h"

template <>
struct EditorInspector<UIButtonComponent> {
  static constexpr const char* kDisplayName = "UIButton";
  static void draw(Registry* /*registry*/, Entity /*entity*/, UIButtonComponent& ub) {
    ImGui::Checkbox("Active", &ub.isActive);
    ImGui::Checkbox("Fixed", &ub.isFixed);
  }
  static std::optional<UIButtonComponent> makeDefault() { return UIButtonComponent{}; }
};

#endif  // OCTARINE_WITH_EDITOR
