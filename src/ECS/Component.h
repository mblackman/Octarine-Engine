#pragma once

#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <set>
#include <unordered_map>
#include <vector>

#include "Entity.h"
#include "General/Logger.h"

constexpr unsigned int kMaxComponents = 64;
constexpr size_t kChunkSize = 16 * 1024;  // 16KB
using Signature = std::bitset<kMaxComponents>;
using ComponentTypeID = std::uint8_t;

namespace Internal {
inline ComponentTypeID GetNextComponentTypeID() {
  static ComponentTypeID lastID = 0;
  return lastID++;
}
}  // namespace Internal

template <typename T>
ComponentTypeID GetComponentTypeID() {
  static ComponentTypeID typeID = Internal::GetNextComponentTypeID();
  return typeID;
}

class Archetype;

struct ChunkHeader {
  uint32_t entity_count;
};

struct ComponentInfo {
  ComponentTypeID id;
  std::string name;
  size_t size;
  size_t alignment;
};

struct EntityLocation {
  Archetype* archetype;
  size_t chunkIndex;
  size_t indexInChunk;
};

struct ArchetypeEdge {
  Archetype& add;
  Archetype& remove;
};

class ComponentRegistry {
 public:
  template <typename T>
  void RegisterComponent() {
    const ComponentTypeID id = GetComponentTypeID<T>();

    if (id >= m_component_infos.size()) {
      m_component_infos.resize(id + 1);
    }

    m_component_infos[id] = {.id = id, .name = "TODO", .size = sizeof(T), .alignment = alignof(T)};
  }

  const ComponentInfo& GetInfo(const ComponentTypeID id) const { return m_component_infos[id]; }

 private:
  std::vector<ComponentInfo> m_component_infos;
};

inline size_t usableSpace = kChunkSize - sizeof(ChunkHeader);

class Chunk {
 public:
  explicit Chunk() : header_() { buffer_ = new unsigned char[usableSpace]; }

  ~Chunk() { delete buffer_; }

  void* GetComponentArray(const size_t offset) const {
    assert(offset < usableSpace);
    assert(offset > 0);
    return buffer_ + offset;
  }

  size_t GetEntityCount() const { return header_.entity_count; }

  template <typename T>
  void AddComponent(const T& component, const size_t offset, const size_t index) {
    assert(offset < usableSpace);
    assert(offset > 0);
    assert(index < header_.entity_count);
    memcpy(buffer_ + offset + index * sizeof(T), &component, sizeof(T));
  }

  void AddEntity(const Entity entity) {
    memcpy(buffer_ + header_.entity_count * sizeof(Entity), &entity, sizeof(Entity));
    header_.entity_count++;
  }

  void RemoveEntity(const size_t index, const std::vector<size_t>& componentOffset,
                    const std::vector<ComponentInfo>& componentInfo) {
    assert(componentOffset.size() == componentInfo.size());
    const int lastIndex = header_.entity_count - 1;
    std::swap(buffer_[index * sizeof(Entity)], buffer_[lastIndex * sizeof(Entity)]);

    for (size_t i = 0; i < componentOffset.size(); ++i) {
      const auto& info = componentInfo[i];
      const auto offset = componentOffset[i];
      std::swap(buffer_[offset + index * info.size], buffer_[offset + lastIndex * info.size]);
    }

    header_.entity_count--;
  }

 private:
  ChunkHeader header_;
  unsigned char* buffer_;
};

class Archetype {
 public:
  explicit Archetype(const Signature signature, const ComponentRegistry& componentRegistry)
      : signature_(signature), chunk_capacity_(0) {
    component_infos_.reserve(signature.count());

    for (size_t i = 0; i < signature.size(); ++i) {
      if (signature.test(i)) {
        const auto componentInfo = componentRegistry.GetInfo(i);
        component_infos_.emplace_back(componentInfo);
      }
    }

    std::ranges::sort(component_infos_, [](const auto& a, const auto& b) { return a.alignment > b.alignment; });
    for (int i = 0; i < component_infos_.size(); ++i) {
      component_type_to_index_[component_infos_[i].id] = i;
    }
    CalculateLayout();
  }

  const Signature& GetSignature() const { return signature_; }

  EntityLocation AddEntity(const Entity entity) {
    size_t index = 0;
    for (auto chunk : chunks_) {
      if (chunk.GetEntityCount() < chunk_capacity_) {
        chunk.AddEntity(entity);
        return {this, index, chunk.GetEntityCount() - 1};
      }
      ++index;
    }

    Chunk newChunk;
    newChunk.AddEntity(entity);
    chunks_.emplace_back(std::move(newChunk));
    return {this, index, 0};
  }

  void RemoveEntity(const EntityLocation& location) {
    AssetLocation(location);
    chunks_[location.chunkIndex].RemoveEntity(location.indexInChunk, component_offsets_, component_infos_);
  }

  template <typename T>
  void AddComponent(const EntityLocation& location, const T& component) {
    AssetLocation(location);
    const ComponentTypeID type_id = GetComponentTypeID<T>();
    assert(signature_.test(type_id));
    const auto index = component_type_to_index_[type_id];
    chunks_[location.chunkIndex].AddComponent(component, component_offsets_[index], location.indexInChunk);
  }

  template <typename T>
  T* GetComponentArray(const size_t chunkIndex) {
    assert(chunkIndex < chunks_.size());
    const ComponentTypeID type_id = GetComponentTypeID<T>();
    assert(signature_.test(type_id));
    assert(component_type_to_index_.contains(type_id));
    const auto index = component_type_to_index_[type_id];
    return static_cast<T*>(chunks_[chunkIndex].GetComponentArray(component_offsets_[index]));
  }

  void CopyComponents(const EntityLocation& sourceLocation, const EntityLocation& destinationLocation) const {
    assert(sourceLocation.archetype != this);
    assert((sourceLocation.archetype->signature_ & this->signature_) == sourceLocation.archetype->signature_);

    const auto sourceChunk = sourceLocation.archetype->chunks_[sourceLocation.chunkIndex];
    const auto destChunk = chunks_[destinationLocation.chunkIndex];

    for (const ComponentTypeID id : sourceLocation.archetype->GetSignature()) {
      const auto sourceTypeIndex = sourceLocation.archetype->component_type_to_index_.at(id);
      const auto destTypeIndex = component_type_to_index_.at(id);
      const auto info = component_infos_[destTypeIndex];
      const auto sourceArray =
          sourceChunk.GetComponentArray(sourceLocation.archetype->component_offsets_[sourceTypeIndex]);
      const auto destArray = destChunk.GetComponentArray(component_offsets_[destTypeIndex]);

      memcpy(destArray + destinationLocation.indexInChunk * info.size,
             sourceArray + sourceLocation.indexInChunk * info.size, info.size);
    }
  }

  std::unordered_map<ComponentTypeID, ArchetypeEdge> edges;

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

  void AssetLocation(const EntityLocation& location) const {
    assert(location.archetype == this);
    assert(location.chunkIndex < chunks_.size());
    assert(location.indexInChunk < chunks_[location.chunkIndex].GetEntityCount());
  }

  Signature signature_;
  std::vector<ComponentInfo> component_infos_;
  std::vector<size_t> component_offsets_;
  std::unordered_map<ComponentTypeID, size_t> component_type_to_index_;
  std::vector<Chunk> chunks_;
  int chunk_capacity_;
};