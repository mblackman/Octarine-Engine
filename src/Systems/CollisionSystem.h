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
#include "ECS/Iterable.h"
#include "General/PerfUtils.h"

constexpr int kMaxDimensions = 2;
// These values can be tuned for better performance.
constexpr int kMaxRecursionDepth = 8;
constexpr int kBruteforceCutoff = 128;

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

struct Partitions {
  int leftEnd;
  int rightStart;
};

class CollisionSystem {
 public:
  void operator()(const Iterable& iter, TransformComponent& transform, BoxColliderComponent& collider) {
    PROFILE_NAMED_SCOPE("Collision System Update");

    if (collisionResult_.valid() &&
        collisionResult_.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) {
      return;
    }

    if (collisionResult_.valid()) {
      const auto [intersecting_pairs] = collisionResult_.get();
      for (const auto& [fst, snd] : intersecting_pairs) {
        eventBus->EmitEvent<CollisionEvent>(fst, snd);
      }
    }

    const auto entities = GetEntities();
    if (entities.empty()) {
      return;
    }

    // TODO speed up box creation more!
    std::vector<Box> boxes;
    boxes.reserve(entities.size());

    for (auto entity : entities) {
      const auto& transform = entity.GetComponent<TransformComponent>();
      const auto& collider = entity.GetComponent<BoxColliderComponent>();

      boxes.emplace_back(entity, entity.GetEntityMask(), collider.collisionMask, transform.position.x,
                         transform.position.y,
                         transform.position.x + static_cast<float>(collider.width) * transform.scale.x,
                         transform.position.y + static_cast<float>(collider.height) * transform.scale.y);
    }

    collisionResult_ = start_async_collision_detection(std::move(boxes));
  }

 private:
  std::future<CollisionResult> collisionResult_;

  std::future<CollisionResult> start_async_collision_detection(std::vector<Box> boxes) {
    return std::async(std::launch::async, [boxes = std::move(boxes), this]() mutable -> CollisionResult {
      AGGREGATE_PROFILE_SESSION("Async Box Creation");
      std::vector<std::pair<Entity, Entity>> intersectingPairs;

      if (!boxes.empty()) {
        find_intersections_recursive(boxes, 0, static_cast<int>(boxes.size()), 0, 0, intersectingPairs);
      }
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

  [[nodiscard]] Partitions partition_boxes(std::vector<Box>& boxes, const int begin, const int end, const int dimension,
                                           const float medianValue) const {
    ACCUMULATE_PROFILE_SCOPE("Partition Boxes");
    const auto first = boxes.begin() + begin;
    const auto last = boxes.begin() + end;

    // Partition left
    const auto leftEndIt = std::partition(first, last, [&](const Box& box) {
      if (dimension == 0) {
        return box.maxX < medianValue;
      }

      return box.maxY < medianValue;
    });

    // Partition right
    const auto rightBeginIt = std::partition(leftEndIt, last, [&](const Box& box) {
      if (dimension == 0) {
        return box.minX <= medianValue;
      }

      return box.minY <= medianValue;
    });

    const int leftEnd = static_cast<int>(std::distance(boxes.begin(), leftEndIt));

    const int rightStart = static_cast<int>(std::distance(boxes.begin(), rightBeginIt));

    return Partitions{leftEnd, rightStart};
  }

  void find_intersections_brute_force_bipartite(std::vector<Box>& boxes, const int begin1, const int end1,
                                                const int begin2, const int end2,
                                                std::vector<std::pair<Entity, Entity>>& pairs) const {
    ACCUMULATE_PROFILE_SCOPE("Brute Force Bipartite");
    for (int i = begin1; i < end1; ++i) {
      for (int j = begin2; j < end2; ++j) {
        if (boxes[i].intersects(boxes[j])) {
          pairs.emplace_back(boxes[i].entity, boxes[j].entity);
        }
      }
    }
  }

  // NOLINTNEXTLINE(misc-no-recursion)
  void find_intersections_recursive(std::vector<Box>& boxes, const int begin, const int end, int dimension,
                                    const int depth, std::vector<std::pair<Entity, Entity>>& intersectingPairs) {
    const int count = end - begin;
    if (count <= 1) {
      return;
    }

    if (count < kBruteforceCutoff || depth >= kMaxRecursionDepth) {
      // Perform a simple brute-force check for remaining boxes in this subproblem
      find_intersections_brute_force(boxes, begin, end, intersectingPairs);
      return;
    }

    // Find the median value of the current dimension.
    const int mid = (begin + end) / 2;
    std::nth_element(boxes.begin() + begin, boxes.begin() + mid, boxes.begin() + end,
                     [dimension](const Box& a, const Box& b) {
                       if (dimension == 0) return a.maxX + a.minX < b.maxX + b.minX;
                       return a.maxY + a.minY < b.maxY + b.minY;
                     });
    const float medianValue =
        (dimension == 0) ? (boxes[mid].minX + boxes[mid].maxX) / 2.0f : (boxes[mid].minY + boxes[mid].maxY) / 2.0f;

    // Divide boxes into sub-partitions: left, right, and spanning the median.
    const auto [leftEnd, rightStart] = partition_boxes(boxes, begin, end, dimension, medianValue);

    // Recurse for intersections within left and right partitions (same dimension, narrower range)
    find_intersections_recursive(boxes, begin, leftEnd, dimension, depth + 1, intersectingPairs);
    find_intersections_recursive(boxes, rightStart, end, dimension, depth + 1, intersectingPairs);

    // Recurse for intersections within the spanning boxes (next dimension)
    const int nextDimension = (dimension + 1) % kMaxDimensions;
    find_intersections_recursive(boxes, leftEnd, rightStart, nextDimension, depth + 1, intersectingPairs);

    // Find intersections between spanning boxes and non-spanning boxes at the current level.
    find_intersections_brute_force_bipartite(boxes, begin, leftEnd, leftEnd, rightStart, intersectingPairs);
    find_intersections_brute_force_bipartite(boxes, leftEnd, rightStart, rightStart, end, intersectingPairs);
  }
};