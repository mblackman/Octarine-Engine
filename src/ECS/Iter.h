#pragma once

#include "Entity.h"

class Registry;

struct Iter {
  Registry* registry;
  float deltaTime;
  Entity entity;
};