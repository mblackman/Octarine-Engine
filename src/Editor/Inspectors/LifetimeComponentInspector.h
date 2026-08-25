#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <imgui.h>

#include <optional>

#include "Components/LifetimeComponent.h"
#include "Editor/Inspectors/EditorInspector.h"

template <>
struct EditorInspector<LifetimeComponent> {
  static constexpr const char* kDisplayName = "Lifetime";
  static constexpr float kDragSpeed = 0.1F;
  static constexpr float kMinDuration = 0.0F;
  static constexpr float kMaxDuration = 1000.0F;
  static constexpr float kDefaultDuration = 1.0F;

  static void draw(Registry* /*registry*/, Entity /*entity*/, LifetimeComponent& lifetime) {
    ImGui::DragFloat("Duration", &lifetime.lifetimeDuration, kDragSpeed, kMinDuration, kMaxDuration);
    ImGui::DragFloat("Remaining", &lifetime.remainingDuration, kDragSpeed, kMinDuration, lifetime.lifetimeDuration);
  }
  static std::optional<LifetimeComponent> makeDefault() { return LifetimeComponent(kDefaultDuration); }
};

#endif  // OCTARINE_WITH_EDITOR
