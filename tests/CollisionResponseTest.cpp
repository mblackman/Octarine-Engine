// Parity + correctness tests for the parallel collision-response path (RunCollisionResponse +
// ThreadPool::ParallelChunks, wired into DamageSystem / ObstacleBounceSystem).
//
// Part 1 drives the shared scaffold directly with the threshold knob: every scenario runs once
// forced-serial (threshold = SIZE_MAX) and once forced-parallel (threshold = 0) over IDENTICAL
// input, and asserts identical final state — the core race/parity guarantee. Part 2 exercises the
// REAL systems' OnCollisionBatch (fixed threshold) against an independent analytic expectation,
// covering both the serial and parallel sides of the real threshold and the no-dedup bounce parity.
// gtest-free; exit code = failed-check count. Links the ECS core only.

#include <cstdint>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "Components/HealthComponent.h"
#include "Components/ProjectileComponent.h"
#include "Components/RigidBodyComponent.h"
#include "ECS/Registry.h"
#include "EventBus/EventBus.h"
#include "Events/CollisionBatchEvent.h"
#include "Systems/CollisionResponseParallel.h"
#include "Systems/DamageSystem.h"
#include "Systems/ObstacleBounceSystem.h"
#include "TestHarness.h"

using octarine::test::Check;
using octarine::test::CheckEq;

namespace {
using Pairs = std::vector<std::pair<Entity, Entity>>;

struct DamageOutcome {
  int health = 0;  // target's currentHealth after the run (0 if it was despawned)
  bool targetAlive = false;
  int projectilesAlive = 0;
};

// K projectiles (each damage=dmg) all paired with one shared target (maxHealth=hp). Runs them
// through RunCollisionResponse at the given threshold (SIZE_MAX => serial, 0 => parallel), using the
// REAL DamageSystem::OnProjectileHit as the apply step and a classify mirroring DamageSystem. If
// `reversed`, pairs are emitted as (target, projectile) to exercise the symmetric classify branch.
DamageOutcome RunDamageScenario(size_t threshold, int hp, int dmg, int k, bool reversed) {
  Registry reg;
  auto bus = std::make_unique<EventBus>();
  DamageSystem damage;
  damage.Init(&reg, bus);

  const Entity projTag = reg.Tag<ProjectileTag>();
  const Entity enemyTag = reg.TagId("enemies");

  const Entity target = reg.CreateEntity();
  reg.AddComponent(target, HealthComponent(hp, hp));
  reg.AddTag(target, enemyTag);

  Pairs pairs;
  std::vector<Entity> projectiles;
  pairs.reserve(static_cast<size_t>(k));
  projectiles.reserve(static_cast<size_t>(k));
  for (int i = 0; i < k; ++i) {
    const Entity p = reg.CreateEntity();
    reg.AddComponent(p, ProjectileComponent{dmg});
    reg.AddTag(p, projTag);
    projectiles.push_back(p);
    if (reversed) {
      pairs.emplace_back(target, p);
    } else {
      pairs.emplace_back(p, target);
    }
  }

  using Hit = std::pair<Entity, Entity>;
  RunCollisionResponse<Hit>(
      pairs, threshold,
      [&](const Entity a, const Entity b, std::vector<Hit>& sink) {
        if (reg.HasTag(a, projTag) && reg.HasTag(b, enemyTag)) sink.emplace_back(a, b);
        if (reg.HasTag(b, projTag) && reg.HasTag(a, enemyTag)) sink.emplace_back(b, a);
      },
      [&](const Hit& h) { damage.OnProjectileHit(h.first, h.second); });

  reg.Update(1.0F / 60.0F);  // play back deferred despawns

  int alive = 0;
  for (const Entity p : projectiles) {
    if (reg.IsAlive(p)) ++alive;
  }
  const bool targetAlive = reg.IsAlive(target);
  return {targetAlive ? reg.GetComponent<HealthComponent>(target).currentHealth : 0, targetAlive, alive};
}

// Real DamageSystem end-to-end: k projectiles vs one (effectively invulnerable) target; returns the
// target's currentHealth after OnCollisionBatch. Picks serial/parallel by k vs the fixed threshold.
int RealDamageHealth(int k) {
  Registry reg;
  auto bus = std::make_unique<EventBus>();
  DamageSystem damage;
  damage.Init(&reg, bus);
  const Entity projTag = reg.Tag<ProjectileTag>();
  const Entity enemyTag = reg.TagId("enemies");
  const Entity target = reg.CreateEntity();
  reg.AddComponent(target, HealthComponent(1'000'000, 1'000'000));
  reg.AddTag(target, enemyTag);
  Pairs pairs;
  pairs.reserve(static_cast<size_t>(k));
  for (int i = 0; i < k; ++i) {
    const Entity p = reg.CreateEntity();
    reg.AddComponent(p, ProjectileComponent{1});
    reg.AddTag(p, projTag);
    pairs.emplace_back(p, target);
  }
  damage.OnCollisionBatch(CollisionBatchEvent{pairs});
  return reg.GetComponent<HealthComponent>(target).currentHealth;
}

// Real ObstacleBounceSystem end-to-end: one enemy paired with k distinct obstacles; returns the
// enemy's velocity after OnCollisionBatch. velocity flips once per occurrence (no dedup), so the net
// is the original for even k and negated for odd k — on both the serial and parallel sides of k.
glm::vec2 RealBounceVelocity(int k) {
  Registry reg;
  auto bus = std::make_unique<EventBus>();
  ObstacleBounceSystem bounce;
  bounce.Init(&reg, bus);
  const Entity enemyTag = reg.TagId("enemies");
  const Entity obstacleTag = reg.TagId("obstacles");
  const Entity enemy = reg.CreateEntity();
  reg.AddComponent(enemy, RigidBodyComponent(glm::vec2(3.0F, 4.0F)));
  reg.AddTag(enemy, enemyTag);
  Pairs pairs;
  pairs.reserve(static_cast<size_t>(k));
  for (int i = 0; i < k; ++i) {
    const Entity obstacle = reg.CreateEntity();
    reg.AddTag(obstacle, obstacleTag);
    pairs.emplace_back(enemy, obstacle);
  }
  bounce.OnCollisionBatch(CollisionBatchEvent{pairs});
  return reg.GetComponent<RigidBodyComponent>(enemy).velocity;
}
}  // namespace

int main() {
  // --- Part 1: scaffold parity — serial (threshold=MAX) vs parallel (threshold=0), identical input.
  // Counts span the realistic threshold boundary (2048), a ragged non-multiple of the pool width
  // (1001, 2503), and tiny sizes that still split when forced parallel.
  for (const int k : {1, 2, 2047, 2048, 2049, 1001, 2503, 3000}) {
    const std::string tag = "damage k=" + std::to_string(k);
    const DamageOutcome serial = RunDamageScenario(SIZE_MAX, 1'000'000, 1, k, false);
    const DamageOutcome parallel = RunDamageScenario(0, 1'000'000, 1, k, false);
    CheckEq(parallel.health, serial.health, tag + ": parallel health == serial");
    CheckEq(parallel.projectilesAlive, serial.projectilesAlive, tag + ": parallel despawns == serial");
    CheckEq(serial.health, 1'000'000 - k, tag + ": serial health matches analytic (commutative)");
    CheckEq(parallel.projectilesAlive, 0, tag + ": every projectile despawned (parallel)");
  }

  // Symmetric classify branch: pairs given as (target, projectile).
  {
    const DamageOutcome serial = RunDamageScenario(SIZE_MAX, 1'000'000, 1, 3000, true);
    const DamageOutcome parallel = RunDamageScenario(0, 1'000'000, 1, 3000, true);
    CheckEq(parallel.health, serial.health, "damage reversed-order: parallel == serial");
    CheckEq(serial.health, 1'000'000 - 3000, "damage reversed-order: hits found via the b/a branch");
  }

  // Death + despawn dedup across chunk boundaries: total damage exceeds health.
  {
    const DamageOutcome serial = RunDamageScenario(SIZE_MAX, 2000, 1, 3000, false);
    const DamageOutcome parallel = RunDamageScenario(0, 2000, 1, 3000, false);
    Check(!serial.targetAlive, "damage death: target despawned (serial)");
    Check(!parallel.targetAlive, "damage death: target despawned (parallel)");
    CheckEq(parallel.projectilesAlive, 0, "damage death: every projectile despawned (parallel)");
    // No crash here implies the target was queued/blammed exactly once despite 3000 hits.
  }

  // --- Part 2: real systems, fixed threshold, vs analytic truth.
  CheckEq(RealDamageHealth(100), 1'000'000 - 100, "real DamageSystem serial path (k=100)");
  CheckEq(RealDamageHealth(3000), 1'000'000 - 3000, "real DamageSystem parallel path (k=3000)");

  // Interleaved no-op pairs (untagged) so chunk boundaries land on non-hits; only real hits apply.
  {
    Registry reg;
    auto bus = std::make_unique<EventBus>();
    DamageSystem damage;
    damage.Init(&reg, bus);
    const Entity projTag = reg.Tag<ProjectileTag>();
    const Entity enemyTag = reg.TagId("enemies");
    const Entity target = reg.CreateEntity();
    reg.AddComponent(target, HealthComponent(1'000'000, 1'000'000));
    reg.AddTag(target, enemyTag);
    Pairs pairs;
    int realHits = 0;
    for (int i = 0; i < 3000; ++i) {
      if (i % 3 == 0) {
        const Entity p = reg.CreateEntity();
        reg.AddComponent(p, ProjectileComponent{1});
        reg.AddTag(p, projTag);
        pairs.emplace_back(p, target);
        ++realHits;
      } else {
        pairs.emplace_back(reg.CreateEntity(), reg.CreateEntity());  // untagged — no-op
      }
    }
    damage.OnCollisionBatch(CollisionBatchEvent{pairs});
    CheckEq(reg.GetComponent<HealthComponent>(target).currentHealth, 1'000'000 - realHits,
            "interleaved no-op pairs: only real hits applied (real system, parallel)");
  }

  // Bounce no-dedup parity: even occurrence count is a net no-op, odd negates — serial and parallel.
  {
    const glm::vec2 even2 = RealBounceVelocity(2);
    Check(even2.x == 3.0F && even2.y == 4.0F, "bounce k=2 (serial, even): velocity unchanged");
    const glm::vec2 odd3 = RealBounceVelocity(3);
    Check(odd3.x == -3.0F && odd3.y == -4.0F, "bounce k=3 (serial, odd): velocity negated");
    const glm::vec2 even3000 = RealBounceVelocity(3000);
    Check(even3000.x == 3.0F && even3000.y == 4.0F, "bounce k=3000 (parallel, even): velocity unchanged");
    const glm::vec2 odd3001 = RealBounceVelocity(3001);
    Check(odd3001.x == -3.0F && odd3001.y == -4.0F, "bounce k=3001 (parallel, odd): velocity negated");
  }

  return octarine::test::Result();
}
