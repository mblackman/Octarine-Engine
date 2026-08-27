#pragma once

#include <cmath>
#include <glm/glm.hpp>

namespace octarine {

// A 2D rotation carried as its cosine/sine pair rather than an angle.
//
// Rotations are consumed far more often than they are authored: a collider's orientation is
// resolved once per frame but tested against every neighbour the broadphase hands it, and one
// parent's rotation is applied to every child in its family. Holding the resolved pair keeps the
// two transcendental calls at the point where the angle is known and off every use site.
struct Rotation2D {
  float cos{1.0F};
  float sin{0.0F};

  // Identity is the overwhelmingly common case — most entities never rotate — and it is the one
  // angle whose pair is exactly known, so it skips the library calls outright.
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

// Rotate a point about an arbitrary centre. This is the shape every pivot-aware consumer needs:
// world-space geometry that has to swing around the entity's anchor rather than its own middle.
[[nodiscard]] inline glm::vec2 RotateAround(const glm::vec2 point, const glm::vec2 origin, const Rotation2D r) {
  return origin + Rotate(point - origin, r);
}

// Half-extents of the axis-aligned box enclosing a rotated box, for broadphase code that wants a
// conservative AABB before committing to an exact oriented test.
[[nodiscard]] inline glm::vec2 RotatedHalfExtents(const glm::vec2 half, const Rotation2D r) {
  const float absCos = std::abs(r.cos);
  const float absSin = std::abs(r.sin);
  return {half.x * absCos + half.y * absSin, half.x * absSin + half.y * absCos};
}

}  // namespace octarine
