#pragma once

#include <utility>
#include <vector>

#include "ECS/Entity.h"
#include "EventBus/Event.h"

// One event carrying the whole frame's intersecting-pair list, so collision-response subscribers
// (DamageSystem, ObstacleBounceSystem) iterate the batch directly instead of paying per-pair
// event-bus dispatch — a std::map<type_index> lookup + std::function indirection for *every* pair,
// which dominated CollisionSystem at scale (~7.4 ms / frame at ~126k pairs).
//
// `pairs` is borrowed from the CollisionSystem result and is only valid for the duration of the
// EmitEvent dispatch; subscribers must consume it synchronously and must not store the reference.
class CollisionBatchEvent : public Event {
 public:
  const std::vector<std::pair<Entity, Entity>>& pairs;

  explicit CollisionBatchEvent(const std::vector<std::pair<Entity, Entity>>& collisionPairs) : pairs(collisionPairs) {}
};
