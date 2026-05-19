#pragma once

#include <algorithm>
#include <chrono>
#include <cmath>
#include <future>
#include <utility>
#include <vector>

#include "../Components/BoxColliderComponent.h"
#include "../Components/EntityMaskComponent.h"
#include "../Components/GlobalTransformComponent.h"
#include "../ECS/Entity.h"
#include "../ECS/Iterable.h"
#include "../ECS/Registry.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"
#include "General/PerfUtils.h"

constexpr int kMaxDimensions = 2;
// These values can be tuned for better performance.
constexpr int kMaxRecursionDepth = 64;
constexpr int kBruteforceCutoff = 32;

struct Box {
  Entity entity;
  EntityMask entityMask;
  EntityMask collisionMask;
  // AABB enclosing the (possibly rotated) OBB — used by the median-cut broadphase.
  float minX, minY;
  float maxX, maxY;
  // OBB narrowphase state.
  float cx, cy;
  float hx, hy;
  float rotCos, rotSin;
  bool rotated;

  [[nodiscard]] bool intersectsInDimension(const Box& other, const int dim) const {
    if (dim == 0) return !(maxX < other.minX || minX > other.maxX);
    if (dim == 1) return !(maxY < other.minY || minY > other.maxY);
    return false;  // Invalid dimension
  }

  // SAT on the 4 face normals of two OBBs. Only invoked when at least one box is rotated;
  // axis-aligned pairs short-circuit on the AABB check above.
  [[nodiscard]] bool obbIntersects(const Box& other) const {
    const float ax0 = rotCos, ay0 = rotSin;
    const float ax1 = -rotSin, ay1 = rotCos;
    const float bx0 = other.rotCos, by0 = other.rotSin;
    const float bx1 = -other.rotSin, by1 = other.rotCos;
    const float dx = other.cx - cx;
    const float dy = other.cy - cy;

    const float axes[4][2] = {{ax0, ay0}, {ax1, ay1}, {bx0, by0}, {bx1, by1}};
    for (const auto& axis : axes) {
      const float ux = axis[0];
      const float uy = axis[1];
      const float aProj = hx * std::abs(ax0 * ux + ay0 * uy) + hy * std::abs(ax1 * ux + ay1 * uy);
      const float bProj =
          other.hx * std::abs(bx0 * ux + by0 * uy) + other.hy * std::abs(bx1 * ux + by1 * uy);
      const float distProj = std::abs(dx * ux + dy * uy);
      if (distProj > aProj + bProj) return false;
    }
    return true;
  }

  [[nodiscard]] bool intersects(const Box& other) const {
    const bool canInteract = !(collisionMask & other.entityMask).none() || !(other.collisionMask & entityMask).none();
    if (!canInteract) {
      return false;
    }
    if (!intersectsInDimension(other, 0) || !intersectsInDimension(other, 1)) return false;
    if (!rotated && !other.rotated) return true;
    return obbIntersects(other);
  }
};

struct CollisionResult {
  std::vector<std::pair<Entity, Entity>> intersectingPairs;
  std::vector<Box> boxes;
};

struct Partitions {
  int leftEnd;
  int rightStart;
};

class CollisionSystem {
 public:
  void operator()(const ContextFacade& ctx, const Iterable& /*iter*/) {
    PROFILE_NAMED_SCOPE("Collision System Update");

    auto* registry = ctx.GetRegistry();
    auto* eventBus = registry->Get<EventBus*>();

    if (collisionResult_.valid() &&
        collisionResult_.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) {
      return;
    }

    if (collisionResult_.valid()) {
      PROFILE_NAMED_SCOPE("Emit Events");
      CollisionResult result = collisionResult_.get();
      PROFILE_COUNTER_SET("Collision: Intersecting pairs", static_cast<long long>(result.intersectingPairs.size()));
      for (const auto& [fst, snd] : result.intersectingPairs) {
        eventBus->EmitEvent<CollisionEvent>(fst, snd);
      }
      cachedBoxes_ = std::move(result.boxes);
      cachedBoxes_.clear();
    }

    std::vector<Box> boxes = std::move(cachedBoxes_);
    {
      PROFILE_NAMED_SCOPE("Gather Boxes");

      if (!query_) {
        query_ = ctx.GetRegistry()->CreateQuery<GlobalTransformComponent, BoxColliderComponent, EntityMaskComponent>();
      }
      query_->Update();

      const size_t count = query_->GetCount();
      boxes.resize(count);
      std::atomic<size_t> nextIndex{0};

      query_->ParallelForEach([&](Entity entity, const GlobalTransformComponent& transform,
                                  const BoxColliderComponent& collider, const EntityMaskComponent& entityMask) {
        const size_t idx = nextIndex.fetch_add(1, std::memory_order_relaxed);
        const float w = static_cast<float>(collider.width) * transform.scale.x;
        const float h = static_cast<float>(collider.height) * transform.scale.y;
        const float hx = w * 0.5f;
        const float hy = h * 0.5f;
        const float cx = transform.position.x + hx;
        const float cy = transform.position.y + hy;
        const float rot = static_cast<float>(transform.rotation);
        const float rc = std::cos(rot);
        const float rs = std::sin(rot);
        const bool rotated = transform.rotation != 0.0;
        const float aabbHx = hx * std::abs(rc) + hy * std::abs(rs);
        const float aabbHy = hx * std::abs(rs) + hy * std::abs(rc);
        boxes[idx] = {entity, entityMask.mask, collider.collisionMask,
                      cx - aabbHx, cy - aabbHy, cx + aabbHx, cy + aabbHy,
                      cx, cy, hx, hy, rc, rs, rotated};
      });
    }

    PROFILE_COUNTER_SET("Collision: Box count", static_cast<long long>(boxes.size()));

    if (boxes.empty()) {
      return;
    }

    collisionResult_ = start_async_collision_detection(std::move(boxes));
  }

 private:
  std::vector<Box> cachedBoxes_;
  std::future<CollisionResult> collisionResult_;
  std::unique_ptr<ComponentQuery<GlobalTransformComponent, BoxColliderComponent, EntityMaskComponent>> query_;

  std::future<CollisionResult> start_async_collision_detection(std::vector<Box> boxes) {
    return std::async(std::launch::async, [boxes = std::move(boxes), this]() mutable -> CollisionResult {
      AGGREGATE_PROFILE_SESSION("Async Box Creation");
      std::vector<std::pair<Entity, Entity>> intersectingPairs;

      if (!boxes.empty()) {
        find_intersections_recursive(boxes, 0, static_cast<int>(boxes.size()), 0, 0, intersectingPairs);
      }
      return CollisionResult{std::move(intersectingPairs), std::move(boxes)};
    });
  }

  void find_intersections_brute_force(const std::vector<Box>& boxes, const int begin, const int end,
                                      std::vector<std::pair<Entity, Entity>>& intersectingPairs) const {
    ACCUMULATE_PROFILE_SCOPE("Brute Force Intersection");

    for (int i = begin; i < end; ++i) {
      for (int j = i + 1; j < end; ++j) {
        const auto& bi = boxes[static_cast<size_t>(i)];
        const auto& bj = boxes[static_cast<size_t>(j)];
        if (bi.intersects(bj)) {
          intersectingPairs.emplace_back(bi.entity, bj.entity);
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
        const auto& bi = boxes[static_cast<size_t>(i)];
        const auto& bj = boxes[static_cast<size_t>(j)];
        if (bi.intersects(bj)) {
          pairs.emplace_back(bi.entity, bj.entity);
        }
      }
    }
  }

  // NOLINTNEXTLINE(misc-no-recursion)
  void find_intersections_sweep_bipartite(std::vector<Box>& boxes, const int begin1, const int end1, const int begin2,
                                          const int end2, const int dimension,
                                          std::vector<std::pair<Entity, Entity>>& pairs) {
    ACCUMULATE_PROFILE_SCOPE("Sweep Bipartite");

    if (end1 - begin1 == 0 || end2 - begin2 == 0) return;

    if ((end1 - begin1) * (end2 - begin2) <= kBruteforceCutoff * kBruteforceCutoff) {
      find_intersections_brute_force_bipartite(boxes, begin1, end1, begin2, end2, pairs);
      return;
    }

    if (dimension == 0) {
      std::sort(boxes.begin() + begin1, boxes.begin() + end1,
                [](const Box& a, const Box& b) { return a.minX < b.minX; });
      std::sort(boxes.begin() + begin2, boxes.begin() + end2,
                [](const Box& a, const Box& b) { return a.minX < b.minX; });
      int start_j = begin2;
      for (int i = begin1; i < end1; ++i) {
        const Box& a = boxes[static_cast<size_t>(i)];
        while (start_j < end2 && boxes[static_cast<size_t>(start_j)].maxX < a.minX) ++start_j;
        for (int j = start_j; j < end2; ++j) {
          const Box& b = boxes[static_cast<size_t>(j)];
          if (b.minX > a.maxX) break;
          if (a.intersects(b)) pairs.emplace_back(a.entity, b.entity);
        }
      }
    } else {
      std::sort(boxes.begin() + begin1, boxes.begin() + end1,
                [](const Box& a, const Box& b) { return a.minY < b.minY; });
      std::sort(boxes.begin() + begin2, boxes.begin() + end2,
                [](const Box& a, const Box& b) { return a.minY < b.minY; });
      int start_j = begin2;
      for (int i = begin1; i < end1; ++i) {
        const Box& a = boxes[static_cast<size_t>(i)];
        while (start_j < end2 && boxes[static_cast<size_t>(start_j)].maxY < a.minY) ++start_j;
        for (int j = start_j; j < end2; ++j) {
          const Box& b = boxes[static_cast<size_t>(j)];
          if (b.minY > a.maxY) break;
          if (a.intersects(b)) pairs.emplace_back(a.entity, b.entity);
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
        (dimension == 0) ? (boxes[static_cast<size_t>(mid)].minX + boxes[static_cast<size_t>(mid)].maxX) / 2.0f
                         : (boxes[static_cast<size_t>(mid)].minY + boxes[static_cast<size_t>(mid)].maxY) / 2.0f;

    // Divide boxes into sub-partitions: left, right, and spanning the median.
    const auto [leftEnd, rightStart] = partition_boxes(boxes, begin, end, dimension, medianValue);

    // Recurse for intersections within left and right partitions (same dimension, narrower range)
    find_intersections_recursive(boxes, begin, leftEnd, dimension, depth + 1, intersectingPairs);
    find_intersections_recursive(boxes, rightStart, end, dimension, depth + 1, intersectingPairs);

    // Recurse for intersections within the spanning boxes (next dimension)
    const int nextDimension = (dimension + 1) % kMaxDimensions;
    find_intersections_recursive(boxes, leftEnd, rightStart, nextDimension, depth + 1, intersectingPairs);

    // Find intersections between spanning boxes and non-spanning boxes at the current level.
    find_intersections_sweep_bipartite(boxes, begin, leftEnd, leftEnd, rightStart, dimension, intersectingPairs);
    find_intersections_sweep_bipartite(boxes, leftEnd, rightStart, rightStart, end, dimension, intersectingPairs);
  }
};
