#pragma once

#include <utility>
#include <vector>

#include "ECS/Entity.h"
#include "General/ThreadPool.h"

// Shared scaffold for the collision-response systems (DamageSystem, ObstacleBounceSystem). Both
// iterate the frame's intersecting-pair vector doing per-pair tag CLASSIFICATION (registry reads
// only) and then a small number of MUTATIONS for the pairs that actually interact. The
// classification is the cost that scales (a HasTag sweep per pair); the mutations are few.
//
// The two phases are split so the expensive one can be parallelised:
//   * classify(a, b, sink) — append 0..N hit records to `sink` using ONLY registry reads
//                            (HasTag/HasComponent). Safe to run concurrently across pairs because
//                            the registry is structurally stable during the response (despawns are
//                            deferred to Registry::Update; no other system runs concurrently; the
//                            broadphase async task has already completed and never touched the
//                            registry).
//   * apply(hit)           — perform the actual registry mutation for one record. Runs SERIALLY on
//                            the calling thread, so component writes / QueueDespawnEntity never race.
//
// Above `threshold` pairs the classify pass is parallelised across the ThreadPool into per-batch
// local vectors (no shared mutable state, no atomics); apply() then replays every record serially in
// batch order. The result is identical to a fully serial classify+apply because apply order does not
// change the final state for these handlers: damage subtraction commutes and QueueDespawnEntity
// dedups; each bounce occurrence is replayed exactly once. Below the threshold it is a plain serial
// loop (avoids ThreadPool dispatch overhead at realistic density).
template <typename HitT, typename ClassifyFn, typename ApplyFn>
void RunCollisionResponse(const std::vector<std::pair<Entity, Entity>>& pairs, size_t threshold, ClassifyFn&& classify,
                          ApplyFn&& apply) {
  const size_t count = pairs.size();
  if (count == 0) return;

  if (count <= threshold) {
    std::vector<HitT> hits;
    for (const auto& [a, b] : pairs) {
      classify(a, b, hits);
    }
    for (const HitT& hit : hits) {
      apply(hit);
    }
    return;
  }

  // Per-batch local sinks — sized to the pool width so any ParallelChunks batchIndex is in range
  // regardless of how the range happens to split. No locks/atomics: each batch owns one sink.
  std::vector<std::vector<HitT>> perBatch(ThreadPool::Instance().Size());
  ThreadPool::ParallelChunks(count, [&](size_t batch, size_t begin, size_t end) {
    std::vector<HitT>& sink = perBatch[batch];
    for (size_t i = begin; i < end; ++i) {
      classify(pairs[i].first, pairs[i].second, sink);
    }
  });

  for (const std::vector<HitT>& batch : perBatch) {
    for (const HitT& hit : batch) {
      apply(hit);
    }
  }
}
