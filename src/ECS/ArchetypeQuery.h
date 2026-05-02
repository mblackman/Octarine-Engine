#pragma once
#include <future>
#include <iterator>
#include <thread>
#include <tuple>
#include <utility>
#include <vector>

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

    Iterator(const ArchetypeType& type, std::vector<Archetype*>::iterator archetype_it,
             std::vector<Archetype*>::iterator archetype_end_it)
        : type_(type),
          archetype_it_(std::move(archetype_it)),
          archetype_end_it_(std::move(archetype_end_it)),
          chunk_idx_(0),
          entity_idx_(0),
          current_entities_(nullptr) {
      assert(sizeof...(TComponents) == type_.size());
      AdvanceToValid();
    }

    Reference operator*() const {
      assert(sizeof...(TComponents) == type_.size());
      return std::apply([&](auto*... arrays) { return std::tie(current_entities_[entity_idx_], arrays[entity_idx_]...); },
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
        return std::make_tuple(current_archetype->template GetComponentArray<TComponents>(chunk_idx_, type_[Is])...);
      }(std::index_sequence_for<TComponents...>{});
      current_entities_ = current_archetype->chunks_[chunk_idx_].GetEntityArray();
    }

    void AdvanceToValid() {
      while (archetype_it_ != archetype_end_it_) {
        Archetype* current_archetype = *archetype_it_;
        while (chunk_idx_ < current_archetype->chunks_.size()) {
          if (entity_idx_ < current_archetype->chunks_[chunk_idx_].GetEntityCount()) {
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
  };

  ArchetypeQuery() = default;

  explicit ArchetypeQuery(const ArchetypeType& type, std::vector<Archetype*> matching_archetypes)
      : type_(type), matching_archetypes_(std::move(matching_archetypes)) {
    assert(sizeof...(TComponents) == type_.size());
  }

  Iterator begin() { return Iterator(type_, matching_archetypes_.begin(), matching_archetypes_.end()); }
  Iterator end() { return Iterator(type_, matching_archetypes_.end(), matching_archetypes_.end()); }

  // Process all matching entities in parallel across chunks. Each chunk is an independent
  // memory region, so concurrent processing is safe for per-entity writes.
  // Func signature: void(Entity, TComponents&...) or void(TComponents&...).
  // IMPORTANT: Func must not access shared mutable state (e.g. no pushing to shared vectors).
  template <typename Func>
  void ParallelForEach(Func&& func) {
    // Collect work items: (archetype, chunkIdx, entityCount) for all non-empty chunks.
    struct ChunkWork {
      Archetype* archetype;
      size_t chunkIdx;
      size_t entityCount;
    };

    std::vector<ChunkWork> work;
    for (auto* arch : matching_archetypes_) {
      for (size_t c = 0; c < arch->chunks_.size(); ++c) {
        const size_t count = arch->chunks_[c].GetEntityCount();
        if (count > 0) {
          work.push_back({arch, c, count});
        }
      }
    }

    if (work.empty()) return;

    const size_t hw_threads = std::max(1u, std::thread::hardware_concurrency());
    const size_t num_threads = std::min(work.size(), hw_threads);

    if (num_threads <= 1) {
      // Not enough work to justify threading — run sequentially.
      ProcessChunks(work, 0, work.size(), func);
      return;
    }

    const size_t items_per_thread = (work.size() + num_threads - 1) / num_threads;
    std::vector<std::future<void>> futures;
    futures.reserve(num_threads);

    for (size_t t = 0; t < num_threads; ++t) {
      const size_t begin = t * items_per_thread;
      const size_t end = std::min(begin + items_per_thread, work.size());
      if (begin >= end) break;

      futures.push_back(std::async(std::launch::async, [&work, &func, this, begin, end]() {
        ProcessChunks(work, begin, end, func);
      }));
    }

    for (auto& f : futures) f.get();
  }

 private:
  template <typename ChunkWork, typename Func>
  void ProcessChunks(const std::vector<ChunkWork>& work, size_t begin, size_t end, Func& func) {
    for (size_t i = begin; i < end; ++i) {
      const auto& w = work[i];

      // Get typed component arrays directly from the chunk — same as Iterator::UpdateChunkPointers.
      auto arrays = [&]<std::size_t... Is>(std::index_sequence<Is...>) {
        return std::make_tuple(
            w.archetype->template GetComponentArray<TComponents>(w.chunkIdx, type_[Is])...);
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
};