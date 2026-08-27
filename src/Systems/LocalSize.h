#pragma once

#include <glm/glm.hpp>

#include "Components/BoxColliderComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/SquarePrimitiveComponent.h"

namespace octarine {

// The entity's unscaled local dimensions. Precedence is sprite, then primitive, then collider: an
// entity carrying more than one is measured by the thing it actually draws. No size source gives
// zero, which collapses any anchor derived from it onto `position`.
//
// Dimensions only — the primitive's and collider's own offsets are deliberately excluded, so an
// anchor derived from this is normalized against a box sitting at `position` rather than against
// the shifted quad. RenderPrimitiveSystem rebases the pivot onto its destRect at draw time. The
// consequence is that on a primitive with a non-zero offset, a {0.5, 0.5} anchor is not the quad's
// visual centre, so scaling one does not hold it in place.
[[nodiscard]] inline glm::vec2 LocalSize(const SpriteComponent* sprite, const SquarePrimitiveComponent* square,
                                         const BoxColliderComponent* collider) {
  if (sprite) return {sprite->width, sprite->height};
  if (square) return {square->width, square->height};
  if (collider) return {static_cast<float>(collider->width), static_cast<float>(collider->height)};
  return {0.0F, 0.0F};
}

}  // namespace octarine
