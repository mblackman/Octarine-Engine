#pragma once

#include <utility>
#include <vector>

#include "ECS/Entity.h"
#include "EventBus/Event.h"

// One event carrying the frame's *exiting* collision pairs — overlaps that were present last frame
// but are no longer present this frame. Mirrors CollisionBatchEvent (enter) in shape and lifetime:
// `pairs` is borrowed from a CollisionSystem-local vector and is only valid for the duration of
// the EmitEvent dispatch; subscribers must consume it synchronously and must not store the reference.
//
// If an entity in a pair was despawned since the overlap was first detected, the handle may be
// stale — callers should guard with registry checks if they read components on exit.
class CollisionExitBatchEvent : public Event {
 public:
  const std::vector<std::pair<Entity, Entity>>& pairs;

  explicit CollisionExitBatchEvent(const std::vector<std::pair<Entity, Entity>>& exitingPairs) : pairs(exitingPairs) {}
};
