// Checks for system logic + the Registry's system-dispatch plumbing. Part 1 unit-tests a real
// gameplay system's math by calling its functor directly (no Registry needed). Part 2 wires a
// system through Registry::RegisterSystem + Update to prove the query -> ForEach dispatch actually
// moves matching entities and skips non-matching ones. gtest-free; exit code = failed-check count.
// Registered with ctest as SystemLogicTest. Links the ECS core only.

#include <glm/glm.hpp>

#include "ECS/Query.h"  // full ComponentQuery definition for RegisterSystem dispatch
#include "ECS/Registry.h"
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

int main() {
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
