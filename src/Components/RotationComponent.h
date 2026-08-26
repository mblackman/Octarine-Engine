#pragma once

#include <glm/glm.hpp>

// Rotation about a pivot. `value` is radians. `pivot` is normalized against the entity's
// renderable bounds: (0, 0) is the top-left corner, (0.5, 0.5) the centre, (1, 1) the
// bottom-right. Values outside [0, 1] are legal and place the pivot outside those bounds.
//
// `pivotOffset` is derived, not authored. PivotResolveSystem multiplies `pivot` by the entity's
// unscaled size every frame and caches the result in local pixels here, so TransformSystem's hot
// loop never has to reach into a size-bearing component to resolve a normalized anchor. Writing
// it by hand has no effect — the next resolve pass overwrites it.
struct RotationComponent {
  // The entity's centre. This is where sprites rotated before pivots were authorable, so it is
  // the default everywhere an anchor can be omitted — component tables, scene files, C++ callers.
  // Kept as scalars because GLM_FORCE_INTRINSICS leaves glm::vec2's constructor non-constexpr.
  static constexpr float kDefaultPivotX = 0.5F;
  static constexpr float kDefaultPivotY = 0.5F;

  double value;
  glm::vec2 pivot;
  glm::vec2 pivotOffset{0.0F, 0.0F};

  explicit RotationComponent(const double t_value = 0.0,
                             const glm::vec2 t_pivot = glm::vec2(kDefaultPivotX, kDefaultPivotY))
      : value(t_value), pivot(t_pivot) {}
};
