#pragma once
#include <iterator>
#include <tuple>
#include <utility>

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
          entity_idx_(0) {
      assert(sizeof...(TComponents) == type_.size());
      AdvanceToValid();
    }

    Reference operator*() const {
      assert(sizeof...(TComponents) == type_.size());
      Archetype* current_archetype = *archetype_it_;
      auto component_arrays = [&]<std::size_t... Is>(std::index_sequence<Is...>) {
        return std::make_tuple(current_archetype->GetComponentArray<TComponents>(chunk_idx_, type_[Is])...);
      }(std::index_sequence_for<TComponents...>{});

      auto* entity_array = current_archetype->chunks_[chunk_idx_].GetEntityArray();

      return std::apply([&](auto*... arrays) { return std::tie(entity_array[entity_idx_], arrays[entity_idx_]...); },
                        component_arrays);
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
    void AdvanceToValid() {
      while (archetype_it_ != archetype_end_it_) {
        const Archetype* current_archetype = *archetype_it_;
        while (chunk_idx_ < current_archetype->chunks_.size()) {
          if (entity_idx_ < current_archetype->chunks_[chunk_idx_].GetEntityCount()) {
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
  };

  ArchetypeQuery() = default;

  explicit ArchetypeQuery(const ArchetypeType& type, std::vector<Archetype*> matching_archetypes)
      : type_(type), matching_archetypes_(std::move(matching_archetypes)) {
    assert(sizeof...(TComponents) == type_.size());
  }

  Iterator begin() { return Iterator(type_, matching_archetypes_.begin(), matching_archetypes_.end()); }
  Iterator end() { return Iterator(type_, matching_archetypes_.end(), matching_archetypes_.end()); }

 private:
  ArchetypeType type_;
  std::vector<Archetype*> matching_archetypes_;
};