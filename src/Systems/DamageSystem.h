#pragma once

#include <utility>
#include <vector>

#include "Components/HealthComponent.h"
#include "Components/ProjectileComponent.h"
#include "ECS/Iterable.h"
#include "EventBus/EventBus.h"
#include "Events/CollisionBatchEvent.h"
#include "General/Constants.h"
#include "Systems/CollisionResponseParallel.h"

class DamageSystem {
 public:
  void Init(Registry* registry, const std::unique_ptr<EventBus>& eventBus) {
    registry_ = registry;
    projectiles_ = registry_->Tag<ProjectileTag>();
    enemies_ = registry_->TagId("enemies");
    player_ = registry_->TagId("player");
    subscription_ = eventBus->SubscribeEvent<DamageSystem, CollisionBatchEvent>(this, &DamageSystem::OnCollisionBatch);
  }

  // Process the whole frame's collision pairs (W2.3), parallelising the per-pair tag classification
  // above a density threshold (parallel collision response). The classify step does registry reads
  // only (safe to run across worker threads — the registry is structurally stable during the
  // response); the damage/despawn mutations run serially in apply(). See CollisionResponseParallel.h.
  void OnCollisionBatch(const CollisionBatchEvent& event) {
    using Hit = std::pair<Entity, Entity>;  // (projectile, target)
    RunCollisionResponse<Hit>(
        event.pairs, Constants::kCollisionResponseParallelThreshold,
        [this](const Entity a, const Entity b, std::vector<Hit>& sink) {
          if (registry_->HasTag(a, projectiles_) && (registry_->HasTag(b, player_) || registry_->HasTag(b, enemies_))) {
            sink.emplace_back(a, b);
          }
          if (registry_->HasTag(b, projectiles_) && (registry_->HasTag(a, player_) || registry_->HasTag(a, enemies_))) {
            sink.emplace_back(b, a);
          }
        },
        [this](const Hit& hit) { OnProjectileHit(hit.first, hit.second); });
  }

  void OnProjectileHit(const Entity projectile, const Entity target) const {
    const auto projectileComponent = registry_->GetComponent<ProjectileComponent>(projectile);
    auto& targetComponent = registry_->GetComponent<HealthComponent>(target);

    targetComponent.currentHealth -= projectileComponent.damage;

    if (targetComponent.currentHealth <= 0) {
      registry_->QueueDespawnEntity(target);
    }

    registry_->QueueDespawnEntity(projectile);
  }

 private:
  Registry* registry_ = nullptr;
  Entity projectiles_{};
  Entity enemies_{};
  Entity player_{};
  EventBus::SubscriptionHandle subscription_;
};