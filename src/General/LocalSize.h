#pragma once

#include <glm/glm.hpp>

#include "Components/BoxColliderComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/SquarePrimitiveComponent.h"

namespace octarine {

// Returns unscaled local dimensions with precedence: Sprite > Primitive > BoxCollider.
// Excludes collider and primitive local offsets.
[[nodiscard]] inline glm::vec2 LocalSize(const SpriteComponent* sprite, const SquarePrimitiveComponent* square,
                                         const BoxColliderComponent* collider) {
  if (sprite) return {sprite->width, sprite->height};
  if (square) return {square->width, square->height};
  if (collider) return {static_cast<float>(collider->width), static_cast<float>(collider->height)};
  return {0.0F, 0.0F};
}

}  // namespace octarine
