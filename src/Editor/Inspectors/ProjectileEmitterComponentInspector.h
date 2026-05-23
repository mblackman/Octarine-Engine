#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <imgui.h>

#include <optional>

#include "Components/ProjectileEmitterComponent.h"
#include "Editor/Inspectors/EditorInspector.h"
#include "Editor/Inspectors/InspectorWidgets.h"

template <>
struct EditorInspector<ProjectileEmitterComponent> {
  static constexpr const char* kDisplayName = "ProjectileEmitter";
  static void draw(Registry* /*registry*/, Entity /*entity*/, ProjectileEmitterComponent& pe) {
    ImGui::DragFloat2("Velocity", &pe.velocity.x, 1.0F);
    ImGui::DragFloat("Frequency (s)", &pe.frequency, 0.1F);
    ImGui::DragFloat("Duration (s)", &pe.duration, 0.1F);
    ImGui::DragInt("Damage", &pe.damage);
    octarine::editor::inspectors::InputTextString("Projectile Name", pe.projectileName);
  }
  static std::optional<ProjectileEmitterComponent> makeDefault() { return ProjectileEmitterComponent{}; }
};

#endif  // OCTARINE_WITH_EDITOR
