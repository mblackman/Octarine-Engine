#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <imgui.h>

#include <optional>

#include "Components/HealthComponent.h"
#include "Editor/Inspectors/EditorInspector.h"

template <>
struct EditorInspector<HealthComponent> {
  static constexpr const char* kDisplayName = "Health";
  static void draw(Registry* /*registry*/, Entity /*entity*/, HealthComponent& health) {
    ImGui::SliderInt("HP", &health.currentHealth, 0, health.maxHealth);
  }
  static std::optional<HealthComponent> makeDefault() { return HealthComponent(100); }
};

#endif  // OCTARINE_WITH_EDITOR
