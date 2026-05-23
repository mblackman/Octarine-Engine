#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <imgui.h>

#include <optional>

#include "Components/ProjectileComponent.h"
#include "Editor/Inspectors/EditorInspector.h"

template <>
struct EditorInspector<ProjectileComponent> {
  static constexpr const char* kDisplayName = "Projectile";
  static void draw(Registry* /*registry*/, Entity /*entity*/, ProjectileComponent& pc) {
    ImGui::DragInt("Damage", &pc.damage);
    ImGui::Text("Timer: %.2f / %.2f", pc.timer, pc.duration);
  }
  static std::optional<ProjectileComponent> makeDefault() { return std::nullopt; }
};

#endif  // OCTARINE_WITH_EDITOR
