#pragma once

#include "../ECS/Entity.h"
#include "../General/Constants.h"

struct EntityMaskComponent {
  EntityMask mask;

  explicit EntityMaskComponent(const EntityMask mask = EntityMask(Constants::kDefaultEntityMask)) : mask(mask) {}
};
