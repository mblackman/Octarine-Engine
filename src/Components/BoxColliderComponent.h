#pragma once

#include <glm/glm.hpp>

#include "../ECS/Entity.h"
#include "../General/Constants.h"

struct BoxColliderComponent {
  int width;
  int height;
  glm::vec2 offset{};
  bool isFixed;
  EntityMask collisionMask;

  explicit BoxColliderComponent(const int t_width = 0, const int t_height = 0,
                                const glm::vec2 t_offset = glm::vec2(0, 0), const bool t_isFixed = false,
                                const EntityMask t_collisionMask = Constants::kDefaultEntityMask)
      : width(t_width), height(t_height), offset(t_offset), isFixed(t_isFixed), collisionMask(t_collisionMask) {}
};
