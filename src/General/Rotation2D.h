#pragma once

#include <cmath>
#include <glm/glm.hpp>

namespace octarine {

// A 2D rotation stored as precomputed cosine and sine values to avoid
// repeated trigonometric calls during collision and hierarchy updates.
struct Rotation2D {
  float cos{1.0F};
  float sin{0.0F};

  // Identity fast-path avoiding std::cos / std::sin.
  [[nodiscard]] static Rotation2D FromRadians(const float radians) {
    if (radians == 0.0F) return {};
    return {std::cos(radians), std::sin(radians)};
  }

  [[nodiscard]] bool IsIdentity() const { return sin == 0.0F && cos == 1.0F; }
};

// Rotate a vector about the origin.
[[nodiscard]] inline glm::vec2 Rotate(const glm::vec2 v, const Rotation2D r) {
  return {v.x * r.cos - v.y * r.sin, v.x * r.sin + v.y * r.cos};
}

// Rotate a point about an arbitrary center (e.g. pivoting around an anchor).
[[nodiscard]] inline glm::vec2 RotateAround(const glm::vec2 point, const glm::vec2 origin, const Rotation2D r) {
  return origin + Rotate(point - origin, r);
}

// Conservative axis-aligned half-extents of a rotated box for broadphase queries.
[[nodiscard]] inline glm::vec2 RotatedHalfExtents(const glm::vec2 half, const Rotation2D r) {
  const float absCos = std::abs(r.cos);
  const float absSin = std::abs(r.sin);
  return {half.x * absCos + half.y * absSin, half.x * absSin + half.y * absCos};
}

}  // namespace octarine
