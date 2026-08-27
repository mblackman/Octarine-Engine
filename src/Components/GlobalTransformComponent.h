#pragma once

#include <glm/glm.hpp>

// World-space transform resolved by TransformSystem. `position` is the entity's top-left corner;
// `pivot` is the offset from that corner to the rotation centre, in world (scale-applied) pixels.
//
// Every consumer that rotates geometry — the sprite and primitive renderers, collision, the
// collider debug draw — must spin about `position + pivot`. TransformSystem composes child
// transforms around that same point, so a consumer that rotates about anything else (a quad
// centre, say) drifts away from where the hierarchy thinks the entity is.
struct GlobalTransformComponent {
  glm::vec2 position{0.0f, 0.0f};
  glm::vec2 scale{1.0f, 1.0f};
  float rotation{0.0F};
  glm::vec2 pivot{0.0f, 0.0f};
};
