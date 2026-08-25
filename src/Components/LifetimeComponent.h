#pragma once

#include <algorithm>
#include <cassert>

struct LifetimeComponent {
  float lifetimeDuration;
  float remainingDuration;

  explicit LifetimeComponent(const float t_lifetimeDuration = 1.0f)
      : lifetimeDuration(t_lifetimeDuration), remainingDuration(t_lifetimeDuration) {}

  void Decrease(const float amount) {
    assert(amount >= 0);
    remainingDuration = std::max(0.0f, remainingDuration - amount);
  }

  void Extend(const float amount) { remainingDuration += std::max(0.0f, amount); }

  void Reset() { remainingDuration = lifetimeDuration; }

  [[nodiscard]]
  bool IsAlive() const {
    return remainingDuration > 0.0f;
  }
};
