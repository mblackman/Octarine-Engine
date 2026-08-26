// Checks for system logic + the Registry's system-dispatch plumbing. Part 1 unit-tests a real
// gameplay system's math by calling its functor directly (no Registry needed). Part 2 wires a
// system through Registry::RegisterSystem + Update to prove the query -> ForEach dispatch actually
// moves matching entities and skips non-matching ones. gtest-free; exit code = failed-check count.
// Registered with ctest as SystemLogicTest. Links the ECS core only.

#include <cmath>
#include <iostream>

#include <glm/glm.hpp>

#include "ECS/Query.h"  // full ComponentQuery definition for RegisterSystem dispatch
#include "ECS/Registry.h"
#include "General/Rotation2D.h"
#include "Systems/VelocityIntegrationSystem.h"
#include "TestHarness.h"

using octarine::test::Check;

namespace {
struct Position {
  float x = 0.0f;
  float y = 0.0f;
};
struct Velocity {
  float dx = 0.0f;
  float dy = 0.0f;
};
}  // namespace

namespace {
constexpr double kQuarterTurn = 1.5707963267948966;  // pi/2

bool NearVec(const glm::vec2 got, const glm::vec2 want, const float eps = 1e-4F) {
  return std::fabs(got.x - want.x) < eps && std::fabs(got.y - want.y) < eps;
}

// The shared rotation helpers every pivot-aware consumer (transform propagation, collision,
// collider debug draw, the primitive renderer) is built on.
void CheckRotation2D() {
  using octarine::Rotation2D;

  std::cout << "[rotation2d] identity short-circuit and basic rotation\n";
  // FromRadians(0) must be *exactly* identity, not merely close: CollisionSystem uses
  // IsIdentity() to skip the oriented narrowphase, and a 0.9999999 cosine would defeat it.
  const Rotation2D identity = Rotation2D::FromRadians(0.0);
  Check(identity.IsIdentity(), "zero radians resolves to exact identity");
  Check(identity.cos == 1.0F && identity.sin == 0.0F, "identity holds exact cos/sin");
  Check(!Rotation2D::FromRadians(kQuarterTurn).IsIdentity(), "a quarter turn is not identity");

  const Rotation2D quarter = Rotation2D::FromRadians(kQuarterTurn);
  Check(NearVec(octarine::Rotate({1.0F, 0.0F}, quarter), {0.0F, 1.0F}), "a quarter turn maps +x onto +y");
  Check(NearVec(octarine::Rotate({0.0F, 1.0F}, quarter), {-1.0F, 0.0F}), "a quarter turn maps +y onto -x");

  std::cout << "[rotation2d] rotation about an arbitrary centre\n";
  const glm::vec2 origin{10.0F, 10.0F};
  // The centre of rotation is the one fixed point, and it must be bit-exact — TransformSystem
  // relies on a child sitting on its parent's pivot not drifting frame over frame.
  Check(octarine::RotateAround(origin, origin, quarter) == origin, "the centre is a fixed point");
  Check(NearVec(octarine::RotateAround({11.0F, 10.0F}, origin, quarter), {10.0F, 11.0F}),
        "a point offset from the centre swings around it");
  const glm::vec2 arbitrary{3.0F, -7.0F};
  Check(octarine::RotateAround(arbitrary, origin, identity) == arbitrary, "identity leaves a point untouched");

  std::cout << "[rotation2d] enclosing AABB half-extents\n";
  const glm::vec2 half{4.0F, 2.0F};
  Check(octarine::RotatedHalfExtents(half, identity) == half, "an unrotated box encloses itself");
  Check(NearVec(octarine::RotatedHalfExtents(half, quarter), {2.0F, 4.0F}),
        "a quarter turn swaps the enclosing extents");
  // A 45-degree box needs a strictly larger AABB than either axis-aligned extent.
  const glm::vec2 diagonal = octarine::RotatedHalfExtents(half, Rotation2D::FromRadians(kQuarterTurn / 2.0));
  Check(diagonal.x > half.x && diagonal.y > half.x, "a diagonal box grows its enclosing AABB");
}
}  // namespace

int main() {
  CheckRotation2D();

  // Part 1 — real gameplay math: position advances by velocity * dt.
  {
    VelocityIntegrationSystem system;
    PositionComponent position(glm::vec2(0.0f, 0.0f));
    const RigidBodyComponent body(glm::vec2(3.0f, 4.0f));
    system(0.5f, position, body);
    Check(position.value.x == 1.5f, "VelocityIntegrationSystem advances x by velocity.x * dt");
    Check(position.value.y == 2.0f, "VelocityIntegrationSystem advances y by velocity.y * dt");
  }

  // Part 2 — dispatch plumbing: a registered system runs over matching entities on Update and
  // leaves non-matching entities untouched.
  {
    Registry registry;
    registry.RegisterSystem<Position, Velocity>([](Position& position, Velocity& velocity) {
      position.x += velocity.dx;
      position.y += velocity.dy;
    });

    const Entity moving = registry.CreateEntity();
    registry.AddComponent(moving, Position{0.0f, 0.0f});
    registry.AddComponent(moving, Velocity{2.0f, -1.0f});

    const Entity stationary = registry.CreateEntity();
    registry.AddComponent(stationary, Position{10.0f, 10.0f});  // no Velocity — must be skipped

    registry.Update(1.0f / 60.0f);

    Check(registry.GetComponent<Position>(moving).x == 2.0f && registry.GetComponent<Position>(moving).y == -1.0f,
          "system advanced the entity matching its full component query");
    Check(registry.GetComponent<Position>(stationary).x == 10.0f &&
              registry.GetComponent<Position>(stationary).y == 10.0f,
          "system skipped the entity missing a queried component");
  }

  return octarine::test::Result();
}
