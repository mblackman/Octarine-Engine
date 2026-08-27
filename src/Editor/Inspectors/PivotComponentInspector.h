#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <imgui.h>

#include <optional>

#include "Components/BoxColliderComponent.h"
#include "Components/PivotComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/SquarePrimitiveComponent.h"
#include "ECS/Registry.h"
#include "Editor/Inspectors/EditorInspector.h"
#include "Systems/LocalSize.h"

template <>
struct EditorInspector<PivotComponent> {
  static constexpr const char* kDisplayName = "Pivot";
  // 0..1 across the whole entity, so it needs a finer drag than pixel-valued fields.
  static constexpr float kDragSpeed = 0.01F;

  static void draw(Registry* registry, Entity entity, PivotComponent& pivot) {
    // Unclamped on purpose — anchors outside 0..1 are a legitimate way to orbit.
    ImGui::DragFloat2("Pivot", &pivot.value.x, kDragSpeed);
    const glm::vec2 resolved =
        octarine::LocalSize(Peek<SpriteComponent>(registry, entity), Peek<SquarePrimitiveComponent>(registry, entity),
                            Peek<BoxColliderComponent>(registry, entity)) *
        pivot.value;
    ImGui::Text("Resolved (local px): %.2f, %.2f", resolved.x, resolved.y);
  }

  template <typename T>
  static const T* Peek(Registry* registry, const Entity entity) {
    return registry->HasComponent<T>(entity) ? &registry->GetComponent<T>(entity) : nullptr;
  }
  static std::optional<PivotComponent> makeDefault() { return PivotComponent{}; }
};

#endif  // OCTARINE_WITH_EDITOR
