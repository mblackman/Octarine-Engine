#pragma once

#include <utility>
#include <vector>

#include "ECS/Entity.h"
#include "EventBus/Event.h"

// One event carrying the frame's *entering* collision pairs — first-contact overlaps that were not
// present in the previous frame. Persistent overlaps (entities that remain in contact across frames)
// are excluded; subscribers only see each contact once, on entry.
//
// Emitted by CollisionSystem once per frame as a single batch rather than one event per pair
// (W2.3); the enter/exit filter (W2.2) further reduces the set to new contacts only.
//
// `pairs` is borrowed from a CollisionSystem-local vector and is only valid for the duration of
// the EmitEvent dispatch; subscribers must consume it synchronously and must not store the reference.
class CollisionBatchEvent : public Event {
 public:
  const std::vector<std::pair<Entity, Entity>>& pairs;

  explicit CollisionBatchEvent(const std::vector<std::pair<Entity, Entity>>& collisionPairs) : pairs(collisionPairs) {}
};
