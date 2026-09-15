#pragma once

#include <utility>
#include <vector>

#include "ECS/Entity.h"
#include "EventBus/Event.h"

// Emitted once per frame carrying separating collision pairs (exit phase).
// Borrowed reference; valid only during dispatch — subscribers must consume synchronously.
class CollisionExitBatchEvent : public Event {
 public:
  const std::vector<std::pair<Entity, Entity>>& pairs;

  explicit CollisionExitBatchEvent(const std::vector<std::pair<Entity, Entity>>& exitingPairs) : pairs(exitingPairs) {}
};
