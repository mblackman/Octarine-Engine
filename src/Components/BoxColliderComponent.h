#pragma once

#include <glm/glm.hpp>

struct BoxColliderComponent {
  int width;
  int height;
  glm::vec2 offset{};
  EntityMask collisionMask;

  explicit BoxColliderComponent(const int width = 0, const int height = 0, const glm::vec2 offset = glm::vec2(0, 0),
                                const EntityMask collisionMask = Constants::kDefaultEntityMask)
      : width(width), height(height), offset(offset), collisionMask(collisionMask) {}
};
