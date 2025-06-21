#pragma once

class Registry;

struct Iter {
  const Registry* registry;
  float deltaTime;
  Entity entity;
};