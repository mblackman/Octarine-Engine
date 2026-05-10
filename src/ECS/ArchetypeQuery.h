#pragma once
#include <atomic>
#include <condition_variable>
#include <iterator>
#include <mutex>
#include <tuple>
#include <utility>
#include <vector>

#include "../General/ThreadPool.h"
#include "Component.h"
#include "Entity.h"

template <typename... TComponents>
class ArchetypeQuery {
 public:
  class Iterator {
   public:
    using IteratorCategory = std::input_iterator_tag;
    using ValueType = std::tuple<Entity, TComponents&...>;
    using Reference = ValueType;
    using Pointer = void;
    using DifferenceType = std::ptrdiff_t;

    Iterator(ArchetypeType type, std::vector<Archetype*>::iterator archetype_it,
             std::vector<Archetype*>::iterator archetype_end_it, bool include_inactive = false)
        : type_(std::move(type)),
          archetype_it_(std::move(archetype_it)),
          archetype_end_it_(std::move(archetype_end_it)),
          chunk_idx_(0),
          entity_idx_(0),
          current_entities_(nullptr),
          include_inactive_(include_inactive) {
      assert(sizeof...(TComponents) == type_.size());
      AdvanceToValid();
    }

    Reference operator*() const {
      assert(sizeof...(TComponents) == type_.size());
      return std::apply(
          [&](auto*... arrays) { return std::tie(current_entities_[entity_idx_], arrays[entity_idx_]...); },
          current_arrays_);
    }

    Iterator& operator++() {
      entity_idx_++;
      AdvanceToValid();
      return *this;
    }

    bool operator==(const Iterator& other) const {
      return archetype_it_ == other.archetype_it_ && chunk_idx_ == other.chunk_idx_ && entity_idx_ == other.entity_idx_;
    }

    bool operator!=(const Iterator& other) const { return !(*this == other); }

   private:
    void UpdateChunkPointers() {
      Archetype* current_archetype = *archetype_it_;
      current_arrays_ = [&]<std::size_t... Is>(std::index_sequence<Is...>) {
        return std::make_tuple(current_archetype->GetComponentArray<TComponents>(chunk_idx_, type_[Is])...);
      }(std::index_sequence_for<TComponents...>{});
      current_entities_ = current_archetype->chunks_[chunk_idx_].GetEntityArray();
    }

    void AdvanceToValid() {
      while (archetype_it_ != archetype_end_it_) {
        Archetype* current_archetype = *archetype_it_;
        while (chunk_idx_ < current_archetype->chunks_.size()) {
          const size_t bound = include_inactive_ ? current_archetype->chunks_[chunk_idx_].GetEntityCount()
                                                 : current_archetype->chunks_[chunk_idx_].GetActiveCount();
          if (entity_idx_ < bound) {
            if (entity_idx_ == 0) {
              UpdateChunkPointers();
            }
            return;  // Found a valid entity
          }
          chunk_idx_++;
          entity_idx_ = 0;
        }
        ++archetype_it_;
        chunk_idx_ = 0;
      }
      if (archetype_it_ == archetype_end_it_) {
        chunk_idx_ = 0;
        entity_idx_ = 0;
      }
    }

    ArchetypeType type_;
    std::vector<Archetype*>::iterator archetype_it_;
    std::vector<Archetype*>::iterator archetype_end_it_;
    size_t chunk_idx_;
    size_t entity_idx_;
    std::tuple<TComponents*...> current_arrays_;
    const Entity* current_entities_;
    bool include_inactive_ = false;
  };

  ArchetypeQuery() = default;

  explicit ArchetypeQuery(const ArchetypeType& type, std::vector<Archetype*> matching_archetypes,
                          bool include_inactive = false)
      : type_(type), matching_archetypes_(std::move(matching_archetypes)), include_inactive_(include_inactive) {
    assert(sizeof...(TComponents) == type_.size());
  }

  Iterator begin() {
    return Iterator(type_, matching_archetypes_.begin(), matching_archetypes_.end(), include_inactive_);
  }
  Iterator end() { return Iterator(type_, matching_archetypes_.end(), matching_archetypes_.end(), include_inactive_); }

  // Process all matching entities in parallel across chunks. Each chunk is an independent
  // memory region, so concurrent processing is safe for per-entity writes.
  // Func signature: void (Entity, TComponents&...) or void (TComponents&...).
  // IMPORTANT: Func must not access a shared mutable state (e.g. no pushing to shared vectors).
  template <typename Func>
  void ParallelForEach(Func&& func) {
    const auto work = CollectChunkWork();
    if (work.empty()) return;

    const size_t num_batches = std::min(work.size(), ThreadPool::Instance().Size());
    if (num_batches <= 1) {
      ProcessChunks(work, 0, work.size(), func);
      return;
    }

    const size_t items_per_batch = (work.size() + num_batches - 1) / num_batches;
    // Dispatch (num_batches - 1) batches to the pool; caller runs the last batch inline
    // so the calling thread doesn't sit idle waiting on a worker.
    BatchBarrier barrier(num_batches - 1);
    for (size_t t = 0; t < num_batches - 1; ++t) {
      const size_t begin = t * items_per_batch;
      const size_t end = std::min(begin + items_per_batch, work.size());
      DispatchBatch(work, begin, end, barrier, func);
    }

    const size_t inline_begin = (num_batches - 1) * items_per_batch;
    const size_t inline_end = std::min(inline_begin + items_per_batch, work.size());
    if (inline_begin < inline_end) {
      ProcessChunks(work, inline_begin, inline_end, func);
    }

    barrier.Wait();
  }

 private:
  struct ChunkWork {
    Archetype* archetype;
    size_t chunkIdx;
    size_t entityCount;
  };

  // Counts down N expected completions and lets one waiter block on Wait().
  // Decrement and notify happen under the mutex so Wait() cannot return — and
  // therefore the BatchBarrier cannot be destroyed — until the last Signal()
  // has fully released the lock. Signaling outside the lock would race the
  // waiter's stack-destruction of the mutex.
  struct BatchBarrier {
    size_t remaining;
    std::mutex mutex;
    std::condition_variable cv;

    explicit BatchBarrier(size_t count) : remaining(count) {}

    void Signal() {
      std::lock_guard<std::mutex> lk(mutex);
      if (--remaining == 0) {
        cv.notify_one();
      }
    }

    void Wait() {
      std::unique_lock<std::mutex> lk(mutex);
      cv.wait(lk, [this] { return remaining == 0; });
    }
  };

  std::vector<ChunkWork> CollectChunkWork() const {
    std::vector<ChunkWork> work;
    for (auto* arch : matching_archetypes_) {
      for (size_t c = 0; c < arch->chunks_.size(); ++c) {
        const size_t count = include_inactive_ ? arch->chunks_[c].GetEntityCount() : arch->chunks_[c].GetActiveCount();
        if (count > 0) {
          work.push_back({arch, c, count});
        }
      }
    }
    return work;
  }

  template <typename Func>
  void DispatchBatch(const std::vector<ChunkWork>& work, size_t begin, size_t end, BatchBarrier& barrier, Func& func) {
    if (begin >= end) {
      barrier.Signal();
      return;
    }
    ThreadPool::Instance().Submit([&work, &func, this, begin, end, &barrier] {
      ProcessChunks(work, begin, end, func);
      barrier.Signal();
    });
  }

  template <typename Func>
  void ProcessChunks(const std::vector<ChunkWork>& work, size_t begin, size_t end, Func& func) {
    for (size_t i = begin; i < end; ++i) {
      const auto& w = work[i];

      // Get typed component arrays directly from the chunk — same as Iterator::UpdateChunkPointers.
      auto arrays = [&]<std::size_t... Is>(std::index_sequence<Is...>) {
        return std::make_tuple(w.archetype->template GetComponentArray<TComponents>(w.chunkIdx, type_[Is])...);
      }(std::index_sequence_for<TComponents...>{});

      const Entity* entities = w.archetype->chunks_[w.chunkIdx].GetEntityArray();

      for (size_t e = 0; e < w.entityCount; ++e) {
        std::apply(
            [&](auto*... compArrays) {
              if constexpr (std::is_invocable_v<Func, Entity, TComponents&...>) {
                func(entities[e], compArrays[e]...);
              } else {
                func(compArrays[e]...);
              }
            },
            arrays);
      }
    }
  }

  ArchetypeType type_;
  std::vector<Archetype*> matching_archetypes_;
  bool include_inactive_ = false;
};