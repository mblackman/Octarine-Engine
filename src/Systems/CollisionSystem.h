#pragma once

#include <algorithm>
#include <chrono>
#include <future>
#include <iostream>
#include <limits>
#include <vector>

#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"
#include "General/PerfUtils.h"

constexpr int kMaxDimensions = 2;
// These values can be tuned for better performance.
constexpr int kMaxRecursionDepth = 8;
constexpr int kBruteforceCutoff = 10;

struct Box {
  Entity entity;
  EntityMask entityMask;
  EntityMask collisionMask;
  float minX, minY;
  float maxX, maxY;

  [[nodiscard]] bool intersectsInDimension(const Box& other, const int dim) const {
    if (dim == 0) return !(maxX < other.minX || minX > other.maxX);
    if (dim == 1) return !(maxY < other.minY || minY > other.maxY);
    return false;  // Invalid dimension
  }

  [[nodiscard]] bool intersects(const Box& other) const {
    const bool canInteract = !(collisionMask & other.entityMask).none() || !(other.collisionMask & entityMask).none();
    if (!canInteract) {
      return false;
    }
    return intersectsInDimension(other, 0) && intersectsInDimension(other, 1);
  }
};

struct CollisionResult {
  std::vector<std::pair<Entity, Entity>> intersectingPairs;
};

struct BoxConstructionData {
  Entity entity;
  EntityMask entityMask;
  EntityMask collisionMask;
  TransformComponent transform;
  BoxColliderComponent collider;
};

struct Partitions {
  int leftEnd;
  int rightStart;
};

class CollisionSystem : public System {
 public:
  CollisionSystem() {
    RequireComponent<TransformComponent>();
    RequireComponent<BoxColliderComponent>();
  }

  CollisionSystem(const CollisionSystem&) = delete;
  CollisionSystem& operator=(const CollisionSystem&) = delete;

  CollisionSystem(CollisionSystem&&) = delete;
  CollisionSystem& operator=(CollisionSystem&&) = delete;

  ~CollisionSystem() = default;

  void Update(const std::unique_ptr<EventBus>& eventBus) {
    PROFILE_NAMED_SCOPE("Collision System Update");

    if (collisionResult_.valid() &&
        collisionResult_.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) {
      // Future is still running.
      return;
    }

    if (collisionResult_.valid()) {
      // Future is done.
      const auto [intersecting_pairs] = collisionResult_.get();
      for (const auto& [fst, snd] : intersecting_pairs) {
        eventBus->EmitEvent<CollisionEvent>(fst, snd);
      }
    }

    std::vector<BoxConstructionData> boxes;

    for (auto entity : GetEntities()) {
      const auto& transform = entity.GetComponent<TransformComponent>();
      const auto& collider = entity.GetComponent<BoxColliderComponent>();
      boxes.emplace_back(entity, entity.GetEntityMask(), collider.collisionMask, transform, collider);
    }

    if (boxes.empty()) {
      return;
    }
    collisionResult_ = start_async_collision_detection(std::move(boxes));
  }

 private:
  std::future<CollisionResult> collisionResult_;

  std::future<CollisionResult> start_async_collision_detection(std::vector<BoxConstructionData> constructionData) {
    return std::async(std::launch::async, [constructionData = std::move(constructionData), this]() -> CollisionResult {
      AGGREGATE_PROFILE_SESSION;
      PROFILE_NAMED_SCOPE("Async Box Creation");
      std::vector<Box> boxes;
      std::vector<std::pair<Entity, Entity>> intersectingPairs;

      boxes.reserve(constructionData.size());
      for (const auto& [entity, entityMask, collisionMask, transform, collider] : constructionData) {
        boxes.emplace_back(entity, entityMask, collisionMask, transform.position.x, transform.position.y,
                           transform.position.x + static_cast<float>(collider.width) * transform.scale.x,
                           transform.position.y + static_cast<float>(collider.height) * transform.scale.y);
      }

      find_intersections_recursive(boxes, 0, static_cast<int>(boxes.size()), 0, 0, intersectingPairs);
      return CollisionResult{intersectingPairs};
    });
  }

  void find_intersections_brute_force(const std::vector<Box>& boxes, const int begin, const int end,
                                      std::vector<std::pair<Entity, Entity>>& intersectingPairs) const {
    ACCUMULATE_PROFILE_SCOPE("Brute Force Intersection");

    for (size_t i = begin; i < end; ++i) {
      for (size_t j = i + 1; j < end; ++j) {
        if (boxes[i].intersects(boxes[j])) {
          intersectingPairs.emplace_back(boxes[i].entity, boxes[j].entity);
        }
      }
    }
  }

  [[nodiscard]] Partitions partition_boxes(const int begin, const int end, const int currentDimension,
                                           std::vector<Box>& boxes, const float medianValue) const {
    ACCUMULATE_PROFILE_SCOPE("Partition Boxes");
    const auto first = boxes.begin() + begin;
    const auto last = boxes.begin() + end;

    // Partition left
    const auto leftEndIt = std::partition(first, last, [&](const Box& box) {
      if (currentDimension == 0) {
        return box.maxX < medianValue;
      }

      return box.maxY < medianValue;
    });

    // Partition right
    const auto rightBeginIt = std::partition(leftEndIt, last, [&](const Box& box) {
      if (currentDimension == 0) {
        return box.minX <= medianValue;
      }

      return box.minY <= medianValue;
    });

    const int leftEnd = static_cast<int>(std::distance(boxes.begin(), leftEndIt));

    const int rightStart = static_cast<int>(std::distance(boxes.begin(), rightBeginIt));

    return Partitions{leftEnd, rightStart};
  }

  void find_intersections_brute_force_bipartite(std::vector<Box>& boxes1, const int begin1, const int end1,
                                                std::vector<Box>& boxes2, const int begin2, const int end2,
                                                std::vector<std::pair<Entity, Entity>>& pairs) const {
    ACCUMULATE_PROFILE_SCOPE("Brute Force Bipartite");
    for (int i = begin1; i < end1; ++i) {
      for (int j = begin2; j < end2; ++j) {
        if (boxes1[i].intersects(boxes2[j])) {
          pairs.emplace_back(boxes1[i].entity, boxes2[j].entity);
        }
      }
    }
  }

  void find_intersections_bipartite(std::vector<Box>& boxes1, const int begin1, const int end1,
                                    std::vector<Box>& boxes2, const int begin2, const int end2, int dimension,
                                    const int depth, std::vector<std::pair<Entity, Entity>>& pairs) const {
    const int count1 = end1 - begin1;
    const int count2 = end2 - begin2;

    if (count1 <= 0 || count2 <= 0) {
      return;
    }

    if (depth >= kMaxRecursionDepth || count1 * count2 < kBruteforceCutoff) {
      find_intersections_brute_force_bipartite(boxes1, begin1, end1, boxes2, begin2, end2, pairs);
      return;
    }

    // TODO Replace with recursive bipartite check
    find_intersections_brute_force_bipartite(boxes1, begin1, end1, boxes2, begin2, end2, pairs);
  }

  // NOLINTNEXTLINE(misc-no-recursion)
  void find_intersections_recursive(std::vector<Box>& boxes, const int begin, const int end, int currentDimension,
                                    const int currentDepth, std::vector<std::pair<Entity, Entity>>& intersectingPairs) {
    const int count = end - begin;
    if (count <= 1) {
      return;
    }

    if (count < kBruteforceCutoff || currentDepth >= kMaxRecursionDepth) {
      // Perform a simple brute-force check for remaining boxes in this subproblem
      find_intersections_brute_force(boxes, begin, end, intersectingPairs);
      return;
    }

    // Find the median value of the current dimension.
    const int mid = (begin + end) / 2;
    std::nth_element(boxes.begin() + begin, boxes.begin() + mid, boxes.begin() + end,
                     [currentDimension](const Box& a, const Box& b) {
                       if (currentDimension == 0) return a.maxX + a.minX < b.maxX + b.minX;
                       return a.maxY + a.minY < b.maxY + b.minY;
                     });
    const float medianValue = (currentDimension == 0) ? (boxes[mid].minX + boxes[mid].maxX) / 2.0f
                                                      : (boxes[mid].minY + boxes[mid].maxY) / 2.0f;

    // Divide boxes into sub-partitions: left, right, and spanning the median.
    const Partitions p = partition_boxes(begin, end, currentDimension, boxes, medianValue);

    // Recurse for intersections within left and right partitions (same dimension, narrower range)
    find_intersections_recursive(boxes, begin, p.leftEnd, currentDimension, currentDepth + 1, intersectingPairs);
    find_intersections_recursive(boxes, p.rightStart, end, currentDimension, currentDepth + 1, intersectingPairs);

    // Recurse for intersections within the spanning boxes (next dimension)
    const int nextDimension = (currentDimension + 1) % kMaxDimensions;
    find_intersections_recursive(boxes, p.leftEnd, p.rightStart, nextDimension, currentDepth + 1, intersectingPairs);

    // Find intersections between spanning boxes and non-spanning boxes at the current level.
    find_intersections_bipartite(boxes, begin, p.leftEnd, boxes, p.leftEnd, p.rightStart, nextDimension,
                                 currentDepth + 1, intersectingPairs);
    find_intersections_bipartite(boxes, p.leftEnd, p.rightStart, boxes, p.rightStart, end, nextDimension,
                                 currentDepth + 1, intersectingPairs);
  }
};