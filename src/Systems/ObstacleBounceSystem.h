#pragma once

#include <memory>
#include <vector>

#include "Components/RigidBodyComponent.h"
#include "Components/SpriteComponent.h"
#include "ECS/Entity.h"
#include "ECS/Registry.h"
#include "EventBus/EventBus.h"
#include "Events/CollisionBatchEvent.h"
#include "General/Constants.h"
#include "General/SpriteFlip.h"
#include "Systems/CollisionResponseParallel.h"

class ObstacleBounceSystem {
 public:
  void Init(Registry* registry, const std::unique_ptr<EventBus>& eventBus) {
    registry_ = registry;
    enemies_ = registry_->TagId("enemies");
    obstacles_ = registry_->TagId("obstacles");
    subscription_ = eventBus->SubscribeEvent<ObstacleBounceSystem, CollisionBatchEvent>(
        this, &ObstacleBounceSystem::OnCollisionBatch);
  }

  // Process the whole frame's collision pairs (W2.3), parallelising the per-pair tag classification
  // above a density threshold. classify does registry reads only; the velocity/sprite flips run
  // serially in apply(). See CollisionResponseParallel.h.
  void OnCollisionBatch(const CollisionBatchEvent& event) {
    RunCollisionResponse<Entity>(
        event.pairs, Constants::kCollisionResponseParallelThreshold,
        // Record EVERY bounce occurrence — do NOT dedup. OnObstacleCollision flips velocity each
        // call, so an enemy overlapping two obstacles must flip twice (net no-op) to match the
        // serial behaviour; deduping here would silently change the outcome.
        [this](const Entity a, const Entity b, std::vector<Entity>& sink) {
          if (registry_->HasTag(a, enemies_) && registry_->HasTag(b, obstacles_)) {
            sink.push_back(a);
          }
          if (registry_->HasTag(b, enemies_) && registry_->HasTag(a, obstacles_)) {
            sink.push_back(b);
          }
        },
        [this](const Entity enemy) { OnObstacleCollision(enemy); });
  }

 private:
  void OnObstacleCollision(const Entity enemy) const {
    auto& rigidBody = registry_->GetComponent<RigidBodyComponent>(enemy);
    rigidBody.velocity = rigidBody.velocity * -1.0f;

    if (registry_->HasComponent<SpriteComponent>(enemy)) {
      auto& sprite = registry_->GetComponent<SpriteComponent>(enemy);
      sprite.flip =
          sprite.flip == octarine::SpriteFlip::None ? octarine::SpriteFlip::Horizontal : octarine::SpriteFlip::None;
    }
  }

  Registry* registry_ = nullptr;
  Entity enemies_{};
  Entity obstacles_{};
  EventBus::SubscriptionHandle subscription_;
};
