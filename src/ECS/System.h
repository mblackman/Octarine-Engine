#pragma once

class Registry;

class System {
 public:
  explicit System(Registry* registry) {}

  System(const System&) = delete;
  System& operator=(const System&) = delete;

  System(System&&) = delete;
  System& operator=(System&&) = delete;

  ~System() = default;
};