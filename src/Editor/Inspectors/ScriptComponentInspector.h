#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <imgui.h>

#include <optional>

#include "Components/ScriptComponent.h"
#include "Editor/Inspectors/EditorInspector.h"

template <>
struct EditorInspector<ScriptComponent> {
  static constexpr const char* kDisplayName = "Script";
  static void draw(Registry* /*registry*/, Entity /*entity*/, ScriptComponent& /*script*/) {
    ImGui::Text("Has Lua script table and callbacks.");
  }
  static std::optional<ScriptComponent> makeDefault() { return std::nullopt; }
};

#endif  // OCTARINE_WITH_EDITOR
