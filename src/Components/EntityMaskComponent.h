#pragma once

#include "../ECS/Entity.h"
#include "../General/Constants.h"

struct EntityMaskComponent {
  EntityMask mask;

  explicit EntityMaskComponent(const EntityMask t_mask = EntityMask(Constants::kDefaultEntityMask)) : mask(t_mask) {}
};
