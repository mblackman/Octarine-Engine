#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <imgui.h>

#include <optional>

#include "Components/UIButtonComponent.h"
#include "Editor/Inspectors/EditorInspector.h"
#include "Editor/Inspectors/InspectorWidgets.h"

template <>
struct EditorInspector<UIButtonComponent> {
  static constexpr const char* kDisplayName = "UIButton";
  static void draw(Registry* /*registry*/, Entity /*entity*/, UIButtonComponent& ub) {
    ImGui::Checkbox("Active", &ub.isActive);
    ImGui::Checkbox("Fixed", &ub.isFixed);
    octarine::editor::inspectors::InputTextString("Key", ub.key);
    octarine::editor::inspectors::InputTextString("Controller Button", ub.controllerButton);
    octarine::editor::inspectors::InputTextString("Action", ub.action);
  }
  static std::optional<UIButtonComponent> makeDefault() { return UIButtonComponent{}; }
};

#endif  // OCTARINE_WITH_EDITOR
