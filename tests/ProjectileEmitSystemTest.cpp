// Tests for ProjectileEmitSystem's per-frame auto-fire tick (W1).
//
// Part 1 calls operator() directly (no Registry interaction) to verify the timer
// arithmetic — the same approach as SystemLogicTest for VelocityIntegrationSystem.
// A minimal TestContext stub is sufficient because the early-return (frequency <= 0)
// and the decrement-only path (timer still positive) never touch the Registry.
//
// Part 2 needs the pool infrastructure (EntityPoolManager + Init) and uses a real
// Registry to verify spawn-and-reset: when countDownTimer expires, a projectile is
// spawned from the pool and the timer is hard-reset to frequency.
//
// gtest-free; exit code = failed-check count. Links the ECS core only.

#include <glm/glm.hpp>

#include "Components/PositionComponent.h"
#include "Components/ProjectileEmitterComponent.h"
#include "ECS/Context.h"  // AnyContext, ContextFacade, Internal::BulkContextImpl
#include "ECS/Query.h"    // full ComponentQuery for RegisterPool's CreateEntityWithBundle
#include "ECS/Registry.h"
#include "Systems/EntityPoolSystem.h"
#include "Systems/ProjectileEmitSystem.h"
#include "TestHarness.h"

using octarine::test::Check;

namespace {

// Minimal AnyContext for tests that need a real entity identity (spawn path).
class TestContext final : public AnyContext {
 public:
  TestContext(Registry* reg, const float dt, const Entity entity) : reg_(reg), dt_(dt), entity_(entity) {}

  [[nodiscard]] Entity GetEntity() const override { return entity_; }
  [[nodiscard]] Registry* GetRegistry() const override { return reg_; }
  [[nodiscard]] float GetDeltaTime() const override { return dt_; }
  void* GetComponentPtr(EntityID /*id*/) override { return nullptr; }

 private:
  Registry* reg_;
  float dt_;
  Entity entity_;
};

}  // namespace

int main() {
  // --- Part 1: timer arithmetic — no Registry interaction needed.

  // frequency = 0 is a complete no-op: operator returns immediately, timer unchanged.
  {
    ProjectileEmitSystem system;

    ProjectileEmitterComponent emitter{};  // frequency=0, countDownTimer=0 by default
    const PositionComponent pos{};

    Registry reg;
    Internal::BulkContextImpl implCtx(&reg, 1.0f / 60.0f);
    ContextFacade ctx(&implCtx);

    system(ctx, emitter, pos);

    Check(emitter.countDownTimer == 0.0f, "frequency=0: timer unchanged (no-op)");
  }

  // Timer decrements by dt when frequency > 0 and expiry not yet reached.
  {
    ProjectileEmitSystem system;

    // frequency=1.0 → countDownTimer initialises to 1.0; dt=0.5 → timer becomes 0.5 (no fire).
    ProjectileEmitterComponent emitter{{}, 1.0f, 1.0f, 10};
    const PositionComponent pos{};

    Registry reg;
    Internal::BulkContextImpl implCtx(&reg, 0.5f);
    ContextFacade ctx(&implCtx);

    system(ctx, emitter, pos);

    Check(emitter.countDownTimer == 0.5f, "timer decrements by dt when frequency > 0");
    Check(emitter.countDownTimer > 0.0f, "timer not expired: no spawn triggered");
  }

  // Negative frequency is also treated as disabled (≤ 0 guard).
  {
    ProjectileEmitSystem system;

    ProjectileEmitterComponent emitter{{}, 1.0f, -1.0f, 10};
    emitter.countDownTimer = -1.0f;
    const PositionComponent pos{};

    Registry reg;
    Internal::BulkContextImpl implCtx(&reg, 1.0f / 60.0f);
    ContextFacade ctx(&implCtx);

    system(ctx, emitter, pos);

    Check(emitter.countDownTimer == -1.0f, "negative frequency: timer unchanged (no-op)");
  }

  // --- Part 2: spawn path — requires pool infrastructure.

  // Timer expiry triggers SpawnProjectile and resets the timer to frequency.
  {
    Registry reg;
    reg.Set<EntityPoolManager>(EntityPoolManager());

    ProjectileEmitSystem system;
    system.Init(reg);  // registers the projectile pool archetype

    // Emitter entity: no SpriteComponent, so SpawnProjectile skips the position-center offset.
    const Entity emitterEntity = reg.CreateEntity();
    reg.AddComponent(emitterEntity, PositionComponent{glm::vec2(100.0f, 200.0f)});

    // frequency=0.5s, countDownTimer just about to expire.
    const glm::vec2 velocity{300.0f, 0.0f};
    ProjectileEmitterComponent emitter{velocity, 1.0f, 0.5f, 15};
    emitter.countDownTimer = 0.001f;
    const PositionComponent pos{glm::vec2(100.0f, 200.0f)};

    // dt=0.05 → timer = 0.001 - 0.05 = −0.049 → fires.
    TestContext testCtx{&reg, 0.05f, emitterEntity};
    ContextFacade ctx{&testCtx};

    system(ctx, emitter, pos);

    Check(emitter.countDownTimer == 0.5f, "timer hard-reset to frequency after firing");
  }

  // Timer counts down across partial ticks and fires only once on expiry.
  // Uses frequency=1.0 and dt=0.5 — both exactly representable in IEEE 754 — so
  // the countdown 1.0→0.5→0.0 hits zero exactly without floating-point drift.
  {
    Registry reg;
    reg.Set<EntityPoolManager>(EntityPoolManager());

    ProjectileEmitSystem system;
    system.Init(reg);

    const Entity emitterEntity = reg.CreateEntity();
    reg.AddComponent(emitterEntity, PositionComponent{});

    ProjectileEmitterComponent emitter{{100.0f, 0.0f}, 1.0f, 1.0f, 10};  // frequency=1.0
    const PositionComponent pos{};

    TestContext testCtx{&reg, 0.5f, emitterEntity};
    ContextFacade ctx{&testCtx};

    system(ctx, emitter, pos);  // timer 1.0 → 0.5, no fire
    Check(emitter.countDownTimer > 0.0f, "tick 1: timer at 0.5, no fire yet");

    system(ctx, emitter, pos);  // timer 0.5 → 0.0 → fires, reset to 1.0
    Check(emitter.countDownTimer == 1.0f, "tick 2: timer expired, reset to frequency (fired)");

    system(ctx, emitter, pos);  // timer 1.0 → 0.5, no second fire
    Check(emitter.countDownTimer > 0.0f, "tick 3: timer counting down again after reset");
  }

  return octarine::test::Result();
}
