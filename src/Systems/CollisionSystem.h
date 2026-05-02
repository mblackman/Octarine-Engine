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
constexpr int kMinScanThreshold = 10;

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
    return !(collisionMask & other.entityMask).none() && intersectsInDimension(other, 0) &&
           intersectsInDimension(other, 1);
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
  std::vector<const Box*> left;
  std::vector<const Box*> right;
  std::vector<const Box*> spanning;
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
      PROFILE_NAMED_SCOPE("Async Box Creation");
      std::vector<Box> boxes;
      boxes.reserve(constructionData.size());
      for (const auto& [entity, entityMask, collisionMask, transform, collider] : constructionData) {
        const float minX = transform.position.x;
        const float minY = transform.position.y;
        const float maxX = transform.position.x + static_cast<float>(collider.width) * transform.scale.x;
        const float maxY = transform.position.y + static_cast<float>(collider.height) * transform.scale.y;
        boxes.emplace_back(entity, entityMask, collisionMask, minX, minY, maxX, maxY);
      }

      std::vector<const Box*> boxPointers;
      boxPointers.reserve(boxes.size());
      for (const auto& box : boxes) {
        boxPointers.push_back(&box);
      }

      return find_intersections_recursive(boxPointers, 0, 0);
    });
  }

  void find_intersections_brute_force(const std::vector<const Box*>& boxes, CollisionResult& result) const {
    for (size_t i = 0; i < boxes.size(); ++i) {
      for (size_t j = i + 1; j < boxes.size(); ++j) {
        if (boxes[i]->intersects(*boxes[j])) {
          result.intersectingPairs.emplace_back(boxes[i]->entity, boxes[j]->entity);
        }
      }
    }
  }

  [[nodiscard]] Partitions partition_boxes(const int currentDimension, const std::vector<const Box*>& sortedBoxes,
                                           const float medianValue) const {
    Partitions p;

    for (const auto* box : sortedBoxes) {
      bool touchesLeftSide = false;
      bool touchesRightSide = false;

      if (currentDimension == 0) {
        touchesLeftSide = box->minX <= medianValue;
        touchesRightSide = box->maxX >= medianValue;
      } else {
        touchesLeftSide = box->minY <= medianValue;
        touchesRightSide = box->maxY >= medianValue;
      }

      if (touchesLeftSide && touchesRightSide) {
        p.spanning.push_back(box);
      } else if (touchesLeftSide) {
        p.left.push_back(box);
      } else {
        p.right.push_back(box);
      }
    }

    return p;
  }

  void check_spanning_vs_partitions(CollisionResult& result, const Partitions& p) const {
    for (const auto* spanBox : p.spanning) {
      for (const auto* otherBox : p.left) {
        if (spanBox->intersects(*otherBox)) {
          result.intersectingPairs.emplace_back(spanBox->entity, otherBox->entity);
        }
      }
      for (const auto* otherBox : p.right) {
        if (spanBox->intersects(*otherBox)) {
          result.intersectingPairs.emplace_back(spanBox->entity, otherBox->entity);
        }
      }
    }
  }

  // NOLINTNEXTLINE(misc-no-recursion)
  CollisionResult find_intersections_recursive(const std::vector<const Box*>& boxes, int currentDimension,
                                               const int currentDepth) {
    CollisionResult result;

    if (boxes.size() < kMinScanThreshold || currentDepth >= kMaxRecursionDepth) {
      // Perform a simple brute-force check for remaining boxes in this subproblems
      find_intersections_brute_force(boxes, result);
      return result;
    }

    // Sort boxes based on the min coordinate of the current dimension.
    std::vector<const Box*> sortedBoxes = boxes;
    std::ranges::sort(sortedBoxes, [currentDimension](const Box* a, const Box* b) {
      if (currentDimension == 0) return a->minX < b->minX;
      return a->minY < b->minY;
    });

    if (sortedBoxes.empty()) {
      return result;
    }

    // Find a "median" point for partitioning, akin to ApproxMedian.
    const Box* median_box = sortedBoxes[sortedBoxes.size() / 2];
    const float medianValue = currentDimension == 0 ? median_box->minX : median_box->minY;

    // Divide boxes into sub-partitions: left, right, and spanning the median.
    const Partitions p = partition_boxes(currentDimension, sortedBoxes, medianValue);

    // Recurse for intersections within the spanning boxes (next dimension)
    CollisionResult spanningResults =
        find_intersections_recursive(p.spanning, (currentDimension + 1) % kMaxDimensions, currentDepth + 1);
    result.intersectingPairs.insert(result.intersectingPairs.end(), spanningResults.intersectingPairs.begin(),
                                    spanningResults.intersectingPairs.end());

    // Recurse for intersections within left and right partitions (same dimension, narrower range)
    CollisionResult leftResults = find_intersections_recursive(p.left, currentDimension, currentDepth + 1);
    result.intersectingPairs.insert(result.intersectingPairs.end(), leftResults.intersectingPairs.begin(),
                                    leftResults.intersectingPairs.end());

    CollisionResult rightResults = find_intersections_recursive(p.right, currentDimension, currentDepth + 1);
    result.intersectingPairs.insert(result.intersectingPairs.end(), rightResults.intersectingPairs.begin(),
                                    rightResults.intersectingPairs.end());

    // Find intersections between spanning boxes and non-spanning boxes at the current level.
    check_spanning_vs_partitions(result, p);

    return result;
  }
};