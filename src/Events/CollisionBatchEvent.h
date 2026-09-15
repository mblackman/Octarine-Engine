#pragma once

#include <utility>
#include <vector>

#include "ECS/Entity.h"
#include "EventBus/Event.h"

// Emitted once per frame carrying newly contacting collision pairs (enter phase).
// Borrowed reference; valid only during dispatch — subscribers must consume synchronously.
class CollisionBatchEvent : public Event {
 public:
  const std::vector<std::pair<Entity, Entity>>& pairs;

  explicit CollisionBatchEvent(const std::vector<std::pair<Entity, Entity>>& collisionPairs) : pairs(collisionPairs) {}
};
