#pragma once

#include <algorithm>
#include <cassert>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Entity.h"

constexpr size_t kChunkSize = 16 * 1024;  // 16KB

class Archetype;

struct ChunkHeader {
  uint32_t entity_count;
};

struct ComponentInfo {
  ComponentID id{};
  std::string name;
  size_t size{};
  size_t alignment{};
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
inline ComponentID GetNextComponentID() {
  // TODO This should limit to writing to only the first 32 bits, but given that requires over 2B components, we're ok.
  static ComponentID lastID = 0;
  return lastID++;
}

inline ArchetypeID GetNextArchetypeID() {
  static ArchetypeID lastID = 0;
  return lastID++;
}
}  // namespace Internal

template <typename T>
ComponentID GetComponentID() {
  static ComponentID id = Internal::GetNextComponentID();
  return id;
}

struct Type {
  std::vector<ComponentID> ids;

  [[nodiscard]] auto begin() const { return ids.begin(); }
  [[nodiscard]] auto end() const { return ids.end(); }
  [[nodiscard]] auto size() const { return ids.size(); }

  bool operator==(const Type& other) const = default;

  [[nodiscard]] bool Contains(const Type& other) const { return std::ranges::includes(ids, other.ids); }
};

template <typename... TComponents>
Type CreateType() {
  Type type;
  type.ids.reserve(sizeof...(TComponents));
  (type.ids.push_back(GetComponentID<TComponents>()), ...);
  std::ranges::sort(type.ids);
  return type;
}

inline Type CreateType(const std::vector<ComponentID>& components) {
  Type type;
  type.ids = components;
  std::ranges::sort(type.ids);
  return type;
}

inline Type CreateType(const std::vector<ComponentInfo>& components) {
  Type type;
  type.ids.reserve(components.size());
  for (const auto& info : components) {
    type.ids.push_back(info.id);
  }
  std::ranges::sort(type.ids);
  return type;
}

namespace std {
template <>
struct hash<Type> {
  size_t operator()(const Type& type) const noexcept {
    size_t seed = 0;
    for (const auto& id : type.ids) {
      seed ^= 0x9e3779b9 + id + (seed << 6) + (seed >> 2);
    }
    return seed;
  }
};
}  // namespace std

inline size_t usableSpace = kChunkSize - sizeof(ChunkHeader);

class Chunk {
 public:
  explicit Chunk() : header_(), buffer_(new unsigned char[usableSpace]) { header_.entity_count = 0; }

  ~Chunk() { delete[] buffer_; }

  Chunk(Chunk&& other) noexcept : header_(other.header_), buffer_(other.buffer_) {
    other.buffer_ = nullptr;
    other.header_.entity_count = 0;
  }

  Chunk& operator=(Chunk&& other) noexcept {
    if (this != &other) {
      delete[] buffer_;
      header_ = other.header_;
      buffer_ = other.buffer_;
      other.buffer_ = nullptr;
      other.header_.entity_count = 0;
    }
    return *this;
  }

  Chunk(const Chunk&) = delete;
  Chunk& operator=(const Chunk&) = delete;

  [[nodiscard]] void* GetComponentArray(const size_t offset) const {
    assert(offset > 0 && offset < usableSpace);
    return buffer_ + offset;
  }

  [[nodiscard]] size_t GetEntityCount() const { return header_.entity_count; }

  [[nodiscard]] const Entity* GetEntityArray() const { return reinterpret_cast<const Entity*>(buffer_); }

  template <typename T>
  void AddComponent(const T& component, const size_t offset, const size_t index) {
    assert(offset > 0 && offset < usableSpace);
    assert(index < header_.entity_count);
    memcpy(buffer_ + offset + index * sizeof(T), &component, sizeof(T));
  }

  void AddEntity(const Entity entity) {
    memcpy(buffer_ + header_.entity_count * sizeof(Entity), &entity, sizeof(Entity));
    header_.entity_count++;
  }

  void RemoveEntity(const size_t index, const std::vector<size_t>& componentOffsets,
                    const std::vector<ComponentInfo>& componentInfos) {
    assert(componentOffsets.size() == componentInfos.size());
    const size_t lastIndex = header_.entity_count - 1;
    if (index >= header_.entity_count) return;  // Bounds check

    if (index != lastIndex) {
      auto* entity_array = reinterpret_cast<Entity*>(buffer_);

      entity_array[index] = entity_array[lastIndex];

      for (size_t i = 0; i < componentOffsets.size(); ++i) {
        const auto& info = componentInfos[i];
        const auto offset = componentOffsets[i];

        unsigned char* dest_ptr = buffer_ + offset + index * info.size;
        const unsigned char* source_ptr = buffer_ + offset + lastIndex * info.size;

        memcpy(dest_ptr, source_ptr, info.size);
      }
    }

    header_.entity_count--;
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
      : archetype_id_(Internal::GetNextArchetypeID()),
        archetype_type_(CreateType(componentsInfo)),
        component_infos_(componentsInfo),
        chunk_capacity_(0) {
    for (int i = 0; i < component_infos_.size(); ++i) {
      component_type_to_index_[component_infos_[i].id] = i;
    }
    CalculateLayout();
  }

  [[nodiscard]] const Type& type() const { return archetype_type_; }

  EntityLocation AddEntity(const Entity entity) {
    size_t index = 0;
    for (auto& chunk : chunks_) {
      if (chunk.GetEntityCount() < chunk_capacity_) {
        chunk.AddEntity(entity);
        return {this, index, chunk.GetEntityCount() - 1};
      }
      ++index;
    }

    Chunk newChunk;
    chunks_.emplace_back(std::move(newChunk));
    chunks_.back().AddEntity(entity);
    return {this, index, 0};
  }

  void RemoveEntity(const EntityLocation& location) {
    AssertLocation(location);
    chunks_[location.chunkIndex].RemoveEntity(location.indexInChunk, component_offsets_, component_infos_);
  }

  [[nodiscard]] bool HasComponent(const ComponentID id) const {
    return std::ranges::find(archetype_type_.begin(), archetype_type_.end(), id) != archetype_type_.end();
  }

  template <typename T>
  void AddComponent(const EntityLocation& location, const T& component) {
    AssertLocation(location);
    const ComponentID componentId = GetComponentID<T>();
    assert(HasComponent(componentId));
    const auto index = component_type_to_index_[componentId];
    chunks_[location.chunkIndex].AddComponent(component, component_offsets_[index], location.indexInChunk);
  }

  template <typename T>
  T* GetComponentArray(const size_t chunkIndex) {
    assert(chunkIndex < chunks_.size());
    const ComponentID componentId = GetComponentID<T>();
    assert(HasComponent(componentId));
    assert(component_type_to_index_.contains(componentId));
    const auto index = component_type_to_index_[componentId];
    return static_cast<T*>(chunks_[chunkIndex].GetComponentArray(component_offsets_[index]));
  }

  void CopyComponents(const EntityLocation& sourceLocation, const EntityLocation& destinationLocation) const {
    assert(sourceLocation.archetype != this);
    assert(destinationLocation.archetype == this);
    const auto& sourceType = sourceLocation.archetype->type();
    assert(archetype_type_.Contains(sourceType));

    const auto& sourceChunk = sourceLocation.archetype->chunks_[sourceLocation.chunkIndex];
    auto& destChunk = chunks_[destinationLocation.chunkIndex];  // Note: destChunk should not be const

    for (ComponentID id : sourceType) {
      const auto sourceTypeIndex = sourceLocation.archetype->component_type_to_index_.at(id);
      const auto destTypeIndex = this->component_type_to_index_.at(id);
      const auto& info = this->component_infos_[destTypeIndex];

      const void* sourceArray =
          sourceChunk.GetComponentArray(sourceLocation.archetype->component_offsets_[sourceTypeIndex]);
      void* destArray = destChunk.GetComponentArray(this->component_offsets_[destTypeIndex]);

      const unsigned char* source_component_ptr =
          static_cast<const unsigned char*>(sourceArray) + sourceLocation.indexInChunk * info.size;
      unsigned char* dest_component_ptr =
          static_cast<unsigned char*>(destArray) + destinationLocation.indexInChunk * info.size;

      memcpy(dest_component_ptr, source_component_ptr, info.size);
    }
  }

  std::unordered_map<ComponentID, ArchetypeEdge> edges;

 private:
  void CalculateLayout() {
    size_t entityComponentSize = sizeof(Entity);
    for (const auto& info : component_infos_) {
      entityComponentSize += info.size;
    }

    chunk_capacity_ = usableSpace / entityComponentSize;

    // Calculate the starting offset of each component array
    size_t current_offset = sizeof(ChunkHeader);
    current_offset += chunk_capacity_ * sizeof(Entity);

    // Then component arrays
    component_offsets_.resize(component_infos_.size());
    for (size_t i = 0; i < component_infos_.size(); ++i) {
      const auto& info = component_infos_[i];

      // Add padding to meet alignment requirements
      const size_t padding = (info.alignment - (current_offset % info.alignment)) % info.alignment;
      current_offset += padding;

      component_offsets_[i] = current_offset;
      current_offset += chunk_capacity_ * info.size;
    }
  }

  void AssertLocation(const EntityLocation& location) const {
    assert(location.archetype == this);
    assert(location.chunkIndex < chunks_.size());
    assert(location.indexInChunk < chunks_[location.chunkIndex].GetEntityCount());
  }

  ArchetypeID archetype_id_;
  Type archetype_type_;
  std::vector<ComponentInfo> component_infos_;
  std::vector<size_t> component_offsets_;
  std::unordered_map<ComponentID, size_t> component_type_to_index_;
  std::vector<Chunk> chunks_;
  int chunk_capacity_;
};