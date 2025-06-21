#pragma once

#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <set>
#include <unordered_map>
#include <vector>

#include "Entity.h"
#include "General/Logger.h"
#include "Iter.h"

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

template <typename... TComponents>
Signature GetSignature() {
  Signature signature;
  (signature.set(GetComponentTypeID<TComponents>()), ...);
  return signature;
}

class Archetype;

struct ChunkHeader {
  uint32_t entity_count;
};

struct ComponentInfo {
  ComponentTypeID id{};
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

class ComponentRegistry {
 public:
  template <typename T>
  void RegisterComponent() {
    const ComponentTypeID id = GetComponentTypeID<T>();

    if (id >= component_infos_.size()) {
      component_infos_.resize(id + 1);
    } else if (component_infos_[id].id != 0) {
      return;
    }

    component_infos_[id] = {.id = id, .name = "TODO", .size = sizeof(T), .alignment = alignof(T)};
  }

  [[nodiscard]] const ComponentInfo& GetInfo(const ComponentTypeID id) const { return component_infos_[id]; }

 private:
  std::vector<ComponentInfo> component_infos_;
};

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
  explicit Archetype(const Signature signature, const ComponentRegistry& componentRegistry)
      : edges(), signature_(signature), chunk_capacity_(0) {
    component_infos_.reserve(signature.count());

    for (size_t i = 0; i < signature.size(); ++i) {
      if (signature.test(i)) {
        const auto& componentInfo = componentRegistry.GetInfo(i);
        component_infos_.emplace_back(componentInfo);
      }
    }

    std::ranges::sort(component_infos_, [](const auto& a, const auto& b) { return a.alignment > b.alignment; });
    for (int i = 0; i < component_infos_.size(); ++i) {
      component_type_to_index_[component_infos_[i].id] = i;
    }
    CalculateLayout();
  }

  [[nodiscard]] const Signature& GetSignature() const { return signature_; }

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
    const auto& source_sig = sourceLocation.archetype->GetSignature();
    assert((this->signature_ & source_sig) == source_sig);

    const auto& sourceChunk = sourceLocation.archetype->chunks_[sourceLocation.chunkIndex];
    auto& destChunk = chunks_[destinationLocation.chunkIndex];  // Note: destChunk should not be const

    for (size_t id = 0; id < source_sig.size(); ++id) {
      if (source_sig.test(id)) {
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
  }

  template <typename Func, typename... TComponents>
  void ForEach(Func func) {
    for (size_t chunk_idx = 0; chunk_idx < chunks_.size(); ++chunk_idx) {
      auto& chunk = chunks_[chunk_idx];
      const size_t entity_count = chunk.GetEntityCount();
      if (entity_count == 0) {
        continue;
      }

      auto component_arrays = std::make_tuple(GetComponentArray<TComponents>(chunk_idx)...);

      for (size_t i = 0; i < entity_count; ++i) {
        std::apply([&](auto*... arrays) { func(arrays[i]...); }, component_arrays);
      }
    }
  }

  template <typename Func, typename... TComponents>
  void ForEach(Func func, Iter& iter) {
    for (size_t chunk_idx = 0; chunk_idx < chunks_.size(); ++chunk_idx) {
      auto& chunk = chunks_[chunk_idx];
      const size_t entity_count = chunk.GetEntityCount();
      if (entity_count == 0) {
        continue;
      }

      auto component_arrays = std::make_tuple(GetComponentArray<TComponents>(chunk_idx)...);
      const auto entityArray = chunk.GetEntityArray();

      for (size_t i = 0; i < entity_count; ++i) {
        iter.entity = entityArray[i];
        std::apply([&](auto*... arrays) { func(iter, arrays[i]...); }, component_arrays);
      }
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