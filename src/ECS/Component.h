#pragma once

#include <algorithm>
#include <cassert>
#include <cstring>
#include <optional>
#include <unordered_map>
#include <vector>

#include "Entity.h"

constexpr size_t kChunkSize = 16 * 1024;  // 16KB

class Archetype;

struct ChunkHeader {
  uint32_t entity_count;
  // Active prefix of the chunk: slots [0, active_count) are visible to default queries; slots
  // [active_count, entity_count) hold parked/inactive entities that share storage with the active
  // ones. Invariant: active_count <= entity_count.
  uint32_t active_count;
};

// Outcome of Chunk::RemoveEntity: up to two entities may have been moved into different slots
// (one when crossing the active/inactive boundary, one for the standard swap-with-end). The
// caller patches entity_locations_ for each.
struct ChunkRemoveSwap {
  Entity entity;
  size_t indexInChunk;
};

struct ComponentInfo {
  ComponentID id{};
  std::string name;
  size_t size{};
  size_t alignment{};
  // Lifecycle ops — needed so non-trivially-copyable components survive archetype transitions.
  void (*move_construct)(void* dst, void* src) = nullptr;
  void (*destroy)(void* ptr) = nullptr;
  // In-chunk swap of two slots. Used by Archetype::Activate/Deactivate to shuffle entities across
  // the active/inactive partition without crossing chunks. Tags (size 0) leave this as a no-op.
  void (*swap)(void* a, void* b) = nullptr;
};

struct EntityLocation {
  Archetype* archetype;
  size_t chunkIndex;
  size_t indexInChunk;
};

struct ArchetypeEdge {
  Archetype* add;
  Archetype* remove;
};

namespace Internal {
inline ArchetypeID GetNextArchetypeID() {
  static ArchetypeID lastID = 0;
  return lastID++;
}
}  // namespace Internal

constexpr size_t kUsableSpace = kChunkSize;

class Chunk {
 public:
  explicit Chunk() : header_(), buffer_(new unsigned char[kUsableSpace]) {
    header_.entity_count = 0;
    header_.active_count = 0;
  }

  ~Chunk() { delete[] buffer_; }

  Chunk(Chunk&& other) noexcept : header_(other.header_), buffer_(other.buffer_) {
    other.buffer_ = nullptr;
    other.header_.entity_count = 0;
    other.header_.active_count = 0;
  }

  Chunk& operator=(Chunk&& other) noexcept {
    if (this != &other) {
      delete[] buffer_;
      header_ = other.header_;
      buffer_ = other.buffer_;
      other.buffer_ = nullptr;
      other.header_.entity_count = 0;
      other.header_.active_count = 0;
    }
    return *this;
  }

  Chunk(const Chunk&) = delete;
  Chunk& operator=(const Chunk&) = delete;

  [[nodiscard]] void* GetComponentArray(const size_t offset) const {
    assert(offset < kUsableSpace);
    return buffer_ + offset;
  }

  [[nodiscard]] size_t GetEntityCount() const { return header_.entity_count; }
  [[nodiscard]] size_t GetActiveCount() const { return header_.active_count; }

  [[nodiscard]] const Entity* GetEntityArray() const { return reinterpret_cast<const Entity*>(buffer_); }

  template <typename T>
  void AddComponent(const T& component, const size_t offset, const size_t index) {
    assert(offset < kUsableSpace);
    assert(index < header_.entity_count);
    ::new (buffer_ + offset + index * sizeof(T)) T(component);
  }

  // Append the entity record. Components are not yet placed; the caller does that next, and
  // then must call Archetype::Activate (or Archetype::FinalizeAdd, same thing) to move the new
  // entity into the active region.
  void AddEntity(const Entity entity, [[maybe_unused]] const size_t capacity) {
    assert(header_.entity_count < capacity);
    auto* entity_array = reinterpret_cast<Entity*>(buffer_);
    entity_array[header_.entity_count] = entity;
    header_.entity_count++;
  }

  void IncrementActive() {
    assert(header_.active_count < header_.entity_count);
    ++header_.active_count;
  }

  void DecrementActive() {
    assert(header_.active_count > 0);
    --header_.active_count;
  }

  // Swap two slots (entity record + every component array). Both slots must be constructed.
  // Caller is responsible for patching entity_locations_ for the two swapped entities.
  void Swap(const size_t i, const size_t j, const std::vector<size_t>& componentOffsets,
            const std::vector<ComponentInfo>& componentInfos) {
    assert(componentOffsets.size() == componentInfos.size());
    assert(i < header_.entity_count && j < header_.entity_count);
    if (i == j) return;
    auto* entity_array = reinterpret_cast<Entity*>(buffer_);
    std::swap(entity_array[i], entity_array[j]);
    for (size_t c = 0; c < componentOffsets.size(); ++c) {
      const auto& info = componentInfos[c];
      if (info.size == 0 || !info.swap) continue;
      unsigned char* a = buffer_ + componentOffsets[c] + i * info.size;
      unsigned char* b = buffer_ + componentOffsets[c] + j * info.size;
      info.swap(a, b);
    }
  }

  // Remove the entity at `index`. Handles entries in either the active region [0, active_count)
  // or the inactive tail [active_count, entity_count). Returns up to two relocations: the first
  // for the active-boundary collapse (if the removed entity was active and not at the boundary),
  // the second for the standard swap-with-last fill. Caller patches entity_locations_ for each.
  std::vector<ChunkRemoveSwap> RemoveEntity(const size_t index, const std::vector<size_t>& componentOffsets,
                                            const std::vector<ComponentInfo>& componentInfos) {
    assert(componentOffsets.size() == componentInfos.size());
    std::vector<ChunkRemoveSwap> swaps;
    if (index >= header_.entity_count) return swaps;

    auto* entity_array = reinterpret_cast<Entity*>(buffer_);
    const bool wasActive = index < header_.active_count;

    // Destroy components at the removed slot.
    for (size_t c = 0; c < componentOffsets.size(); ++c) {
      const auto& info = componentInfos[c];
      unsigned char* slot_ptr = buffer_ + componentOffsets[c] + index * info.size;
      if (info.destroy) info.destroy(slot_ptr);
    }

    // Active-region collapse: pull the last active entity into the freed slot so the active
    // prefix stays dense. Skip when the removed entity was already at the boundary.
    if (wasActive && index + 1 < header_.active_count) {
      const size_t srcIdx = header_.active_count - 1;
      for (size_t c = 0; c < componentOffsets.size(); ++c) {
        const auto& info = componentInfos[c];
        unsigned char* dest_ptr = buffer_ + componentOffsets[c] + index * info.size;
        unsigned char* source_ptr = buffer_ + componentOffsets[c] + srcIdx * info.size;
        if (info.move_construct) info.move_construct(dest_ptr, source_ptr);
        if (info.destroy) info.destroy(source_ptr);
      }
      entity_array[index] = entity_array[srcIdx];
      swaps.push_back({entity_array[index], index});
    }

    if (wasActive) --header_.active_count;

    // The freed slot after the active collapse is either `index` (inactive removal) or the
    // post-decrement active_count (active removal — the slot that was the active boundary).
    const size_t freedSlot = wasActive ? header_.active_count : index;
    const size_t lastSlot = header_.entity_count - 1;

    // Standard swap-with-end: pull the last entity (always inactive at this point) into the
    // freed slot. Skip when there's nothing to pull.
    if (freedSlot != lastSlot) {
      for (size_t c = 0; c < componentOffsets.size(); ++c) {
        const auto& info = componentInfos[c];
        unsigned char* dest_ptr = buffer_ + componentOffsets[c] + freedSlot * info.size;
        unsigned char* source_ptr = buffer_ + componentOffsets[c] + lastSlot * info.size;
        if (info.move_construct) info.move_construct(dest_ptr, source_ptr);
        if (info.destroy) info.destroy(source_ptr);
      }
      entity_array[freedSlot] = entity_array[lastSlot];
      swaps.push_back({entity_array[freedSlot], freedSlot});
    }

    --header_.entity_count;
    return swaps;
  }

 private:
  ChunkHeader header_;
  unsigned char* buffer_;
};

class Archetype {
 public:
  template <typename... TComponents>
  friend class ArchetypeQuery;

  explicit Archetype(const std::vector<ComponentInfo>& componentsInfo)
      : archetype_id_(Internal::GetNextArchetypeID()), component_infos_(componentsInfo), chunk_capacity_(0) {
    archetype_type_.reserve(component_infos_.size());

    // Sort component_infos_ by id so component_type_to_index_ matches archetype_type_ ordering.
    std::ranges::sort(component_infos_, [](const ComponentInfo& a, const ComponentInfo& b) { return a.id < b.id; });

    for (size_t i = 0; i < component_infos_.size(); ++i) {
      component_type_to_index_[component_infos_[i].id] = i;
      archetype_type_.push_back(component_infos_[i].id);
    }

    CalculateLayout();
  }

  [[nodiscard]] const ArchetypeType& type() const { return archetype_type_; }
  [[nodiscard]] ArchetypeID GetID() const { return archetype_id_; }
  [[nodiscard]] const std::vector<ComponentInfo>& component_infos() const { return component_infos_; }

  [[nodiscard]] Entity GetEntity(size_t chunkIndex, size_t indexInChunk) const {
    assert(chunkIndex < chunks_.size());
    assert(indexInChunk < chunks_[chunkIndex].GetEntityCount());
    return chunks_[chunkIndex].GetEntityArray()[indexInChunk];
  }

  [[nodiscard]] size_t GetActiveCountInChunk(size_t chunkIndex) const {
    assert(chunkIndex < chunks_.size());
    return chunks_[chunkIndex].GetActiveCount();
  }

  EntityLocation AddEntity(const Entity entity) {
    // Cached first-non-full chunk index advances monotonically as earlier chunks fill.
    while (first_non_full_chunk_ < chunks_.size() &&
           chunks_[first_non_full_chunk_].GetEntityCount() >= chunk_capacity_) {
      ++first_non_full_chunk_;
    }

    if (first_non_full_chunk_ < chunks_.size()) {
      auto& chunk = chunks_[first_non_full_chunk_];
      chunk.AddEntity(entity, chunk_capacity_);
      return {this, first_non_full_chunk_, chunk.GetEntityCount() - 1};
    }

    chunks_.emplace_back();
    chunks_.back().AddEntity(entity, chunk_capacity_);
    return {this, chunks_.size() - 1, 0};
  }

  // Returns up to two relocations (active-boundary collapse + swap-with-end). Caller patches
  // entity_locations_ for each returned entity using the recorded slot.
  std::vector<ChunkRemoveSwap> RemoveEntity(const EntityLocation& location) {
    AssertLocation(location);
    auto swaps = chunks_[location.chunkIndex].RemoveEntity(location.indexInChunk, component_offsets_, component_infos_);
    if (location.chunkIndex < first_non_full_chunk_) {
      first_non_full_chunk_ = location.chunkIndex;
    }
    return swaps;
  }

  // Result of a partition transition: the entity that was being toggled is now at `newSlot` in
  // the same chunk; if a swap happened, `displaced` is the entity that previously occupied
  // `newSlot` and now sits at the caller's original slot.
  struct PartitionResult {
    size_t newSlot;
    std::optional<Entity> displaced;
  };

  // Move an entity from the inactive tail into the active prefix. The entity must currently sit
  // at or beyond `active_count`. After this call, it occupies the slot at the previous active
  // boundary and `active_count` has grown by one.
  PartitionResult Activate(const EntityLocation& location) {
    AssertLocation(location);
    auto& chunk = chunks_[location.chunkIndex];
    assert(location.indexInChunk >= chunk.GetActiveCount());
    const size_t boundary = chunk.GetActiveCount();
    PartitionResult result{boundary, std::nullopt};
    if (location.indexInChunk != boundary) {
      chunk.Swap(location.indexInChunk, boundary, component_offsets_, component_infos_);
      result.displaced = chunk.GetEntityArray()[location.indexInChunk];
    }
    chunk.IncrementActive();
    return result;
  }

  // Move an entity from the active prefix into the inactive tail. The entity must currently be
  // active. After this call, it occupies the slot at the new active boundary (active_count - 1
  // post-decrement, which is the first inactive slot).
  PartitionResult Deactivate(const EntityLocation& location) {
    AssertLocation(location);
    auto& chunk = chunks_[location.chunkIndex];
    assert(location.indexInChunk < chunk.GetActiveCount());
    chunk.DecrementActive();
    const size_t boundary = chunk.GetActiveCount();  // post-decrement: new first inactive slot
    PartitionResult result{boundary, std::nullopt};
    if (location.indexInChunk != boundary) {
      chunk.Swap(location.indexInChunk, boundary, component_offsets_, component_infos_);
      result.displaced = chunk.GetEntityArray()[location.indexInChunk];
    }
    return result;
  }

  [[nodiscard]] bool HasComponent(const ComponentID id) const { return component_type_to_index_.contains(id); }

  template <typename T>
  void AddComponent(const EntityLocation& location, const Entity& componentEntity, const T& component) {
    AssertLocation(location);
    assert(HasComponent(componentEntity.id));
    const auto index = component_type_to_index_[componentEntity.id];
    chunks_[location.chunkIndex].AddComponent(component, component_offsets_[index], location.indexInChunk);
  }

  template <typename T>
  T* GetComponentArray(const size_t chunkIndex, const ComponentID componentId) {
    assert(chunkIndex < chunks_.size());
    assert(HasComponent(componentId));
    assert(component_type_to_index_.contains(componentId));
    const auto index = component_type_to_index_[componentId];
    return static_cast<T*>(chunks_[chunkIndex].GetComponentArray(component_offsets_[index]));
  }

  // Move-constructs each shared component from source slot into the (uninitialized) destination
  // slot. Source components are left in moved-from state; the caller is expected to RemoveEntity
  // the source slot afterwards, which will destroy them. Components present in source but not
  // destination are skipped — supports both add (dest is superset) and remove (dest is subset).
  void CopyComponents(const EntityLocation& sourceLocation, const EntityLocation& destinationLocation) const {
    assert(sourceLocation.archetype != this);
    assert(destinationLocation.archetype == this);
    const auto& sourceType = sourceLocation.archetype->type();

    const auto& sourceChunk = sourceLocation.archetype->chunks_[sourceLocation.chunkIndex];
    auto& destChunk = chunks_[destinationLocation.chunkIndex];

    for (const ComponentID id : sourceType) {
      const auto destIt = this->component_type_to_index_.find(id);
      if (destIt == this->component_type_to_index_.end()) continue;
      const auto destTypeIndex = destIt->second;
      const auto sourceTypeIndex = sourceLocation.archetype->component_type_to_index_.at(id);
      const auto& info = this->component_infos_[destTypeIndex];
      if (info.size == 0) continue;  // tag — nothing to copy

      void* sourceArray = sourceChunk.GetComponentArray(sourceLocation.archetype->component_offsets_[sourceTypeIndex]);
      void* destArray = destChunk.GetComponentArray(this->component_offsets_[destTypeIndex]);

      unsigned char* source_component_ptr =
          static_cast<unsigned char*>(sourceArray) + sourceLocation.indexInChunk * info.size;
      unsigned char* dest_component_ptr =
          static_cast<unsigned char*>(destArray) + destinationLocation.indexInChunk * info.size;

      if (info.move_construct) {
        info.move_construct(dest_component_ptr, source_component_ptr);
      } else {
        memcpy(dest_component_ptr, source_component_ptr, info.size);
      }
    }
  }

  std::unordered_map<EntityID, ArchetypeEdge> edges;

 private:
  void CalculateLayout() {
    size_t entityComponentSize = sizeof(Entity);
    for (const auto& info : component_infos_) {
      entityComponentSize += info.size;
    }

    if (entityComponentSize == 0) {
      chunk_capacity_ = 0;
      return;
    }

    // Header lives outside the buffer, so layout starts at offset 0 with the entity array.
    chunk_capacity_ = kUsableSpace / entityComponentSize;

    size_t current_offset = chunk_capacity_ * sizeof(Entity);

    component_offsets_.resize(component_infos_.size());
    for (size_t i = 0; i < component_infos_.size(); ++i) {
      const auto& info = component_infos_[i];

      const size_t padding = info.alignment ? (info.alignment - (current_offset % info.alignment)) % info.alignment : 0;
      current_offset += padding;

      component_offsets_[i] = current_offset;
      current_offset += chunk_capacity_ * info.size;
    }

    // Padding may push the last component array past kUsableSpace; shrink capacity until it fits.
    while (chunk_capacity_ > 0 && current_offset > kUsableSpace) {
      --chunk_capacity_;
      current_offset = chunk_capacity_ * sizeof(Entity);
      for (size_t i = 0; i < component_infos_.size(); ++i) {
        const auto& info = component_infos_[i];
        const size_t padding =
            info.alignment ? (info.alignment - (current_offset % info.alignment)) % info.alignment : 0;
        current_offset += padding;
        component_offsets_[i] = current_offset;
        current_offset += chunk_capacity_ * info.size;
      }
    }
    assert(chunk_capacity_ > 0);
  }

  void AssertLocation([[maybe_unused]] const EntityLocation& location) const {
    assert(location.archetype == this);
    assert(location.chunkIndex < chunks_.size());
    assert(location.indexInChunk < chunks_[location.chunkIndex].GetEntityCount());
  }

  ArchetypeID archetype_id_;
  ArchetypeType archetype_type_;
  std::vector<ComponentInfo> component_infos_;
  std::vector<size_t> component_offsets_;
  std::unordered_map<ComponentID, size_t> component_type_to_index_;
  std::vector<Chunk> chunks_;
  size_t chunk_capacity_;
  size_t first_non_full_chunk_ = 0;
};