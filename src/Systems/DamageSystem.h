#pragma once

#include "Components/HealthComponent.h"
#include "Components/ProjectileComponent.h"
#include "ECS/Iterable.h"
#include "EventBus/EventBus.h"
#include "Events/CollisionBatchEvent.h"

class DamageSystem {
 public:
  void Init(Registry* registry, const std::unique_ptr<EventBus>& eventBus) {
    registry_ = registry;
    projectiles_ = registry_->Tag<ProjectileTag>();
    enemies_ = registry_->TagId("enemies");
    player_ = registry_->TagId("player");
    subscription_ = eventBus->SubscribeEvent<DamageSystem, CollisionBatchEvent>(this, &DamageSystem::OnCollisionBatch);
  }

  // Process the whole frame's collision pairs in one call (W2.3): the per-pair tag logic is
  // unchanged, but it no longer rides the event bus once per pair.
  void OnCollisionBatch(const CollisionBatchEvent& event) {
    for (const auto& [a, b] : event.pairs) {
      if (registry_->HasTag(a, projectiles_) && (registry_->HasTag(b, player_) || registry_->HasTag(b, enemies_))) {
        OnProjectileHit(a, b);
      }

      if (registry_->HasTag(b, projectiles_) && (registry_->HasTag(a, player_) || registry_->HasTag(a, enemies_))) {
        OnProjectileHit(b, a);
      }
    }
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