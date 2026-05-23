#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <imgui.h>

#include <optional>

#include "Components/AnimationComponent.h"
#include "Editor/Inspectors/EditorInspector.h"

template <>
struct EditorInspector<AnimationComponent> {
  static constexpr const char* kDisplayName = "Animation";
  static void draw(Registry* /*registry*/, Entity /*entity*/, AnimationComponent& anim) {
    ImGui::Text("Num Frames: %d", anim.numFrames);
    ImGui::SliderInt("Current Frame", &anim.currentFrame, 1, anim.numFrames);
    ImGui::DragInt("Frame Speed (ms)", &anim.frameRateSpeed);
    ImGui::Checkbox("Looping", &anim.shouldLoop);
  }
  static std::optional<AnimationComponent> makeDefault() { return AnimationComponent{}; }
};

#endif  // OCTARINE_WITH_EDITOR
