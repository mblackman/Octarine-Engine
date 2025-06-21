#pragma once

class System {
 public:
  System() = default;

  System(const System&) = delete;
  System& operator=(const System&) = delete;

  System(System&&) = delete;
  System& operator=(System&&) = delete;

  ~System() = default;
};