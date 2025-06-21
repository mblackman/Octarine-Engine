#pragma once

class Registry;

class ISystem {
 public:
  virtual ~ISystem() = default;
  virtual void Update(const Registry& registry, float deltaTime) = 0;
};