#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <imgui.h>

#include <optional>

#include "Components/TextLabelComponent.h"
#include "Editor/Inspectors/EditorInspector.h"
#include "Editor/Inspectors/InspectorWidgets.h"

template <>
struct EditorInspector<TextLabelComponent> {
  static constexpr const char* kDisplayName = "TextLabel";
  static void draw(Registry* /*registry*/, Entity /*entity*/, TextLabelComponent& tl) {
    octarine::editor::inspectors::InputTextString("Text", tl.text);
    ImGui::Text("Font ID: %s", tl.fontId.c_str());
    octarine::editor::inspectors::DrawColorEdit("Color", tl.color);
    ImGui::Checkbox("Fixed", &tl.isFixed);
  }
  static std::optional<TextLabelComponent> makeDefault() { return TextLabelComponent{}; }
};

#endif  // OCTARINE_WITH_EDITOR
