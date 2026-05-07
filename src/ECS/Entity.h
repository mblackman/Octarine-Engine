#pragma once
#include <bitset>
#include <queue>

#include "ECS.h"

constexpr unsigned int kMaxEntityMasks = 32;
constexpr unsigned int kStartingEntityPoolSize = 1000;
constexpr unsigned int kEntityGenerationOffset = 32;

// Used to determine which
typedef std::bitset<kMaxEntityMasks> EntityMask;
typedef std::uint16_t EntityGeneration;

struct Entity {
  EntityID id;

  Entity() = default;

  explicit Entity(const EcsId t_id) : id(t_id) {}

  [[nodiscard]] std::uint32_t GetId() const { return static_cast<std::uint32_t>(id); }
  [[nodiscard]] std::uint16_t GetGeneration() const {
    return static_cast<std::uint16_t>(id >> kEntityGenerationOffset);
  }

  bool operator==(const Entity& other) const { return id == other.id; }

  bool operator!=(const Entity& other) const { return id != other.id; }

  bool operator<(const Entity& other) const { return id < other.id; }

  bool operator>(const Entity& other) const { return id > other.id; }

  bool operator<=(const Entity& other) const { return id <= other.id; }

  bool operator>=(const Entity& other) const { return id >= other.id; }

  explicit operator EntityID() const { return id; }
};

// TODO need to handle recycling entity generations
class EntityManager {
 public:
  EntityManager() : living_entity_count_(kStartingEntityPoolSize) {
    for (uint32_t i = 0; i < kStartingEntityPoolSize; ++i) {
      available_entities_.push(i);
    }
    generations_.resize(kStartingEntityPoolSize);
  }

  Entity CreateEntity() {
    std::uint32_t baseId;
    if (!available_entities_.empty()) {
      baseId = available_entities_.front();
      available_entities_.pop();
    } else {
      baseId = static_cast<std::uint32_t>(living_entity_count_++);
      if (baseId >= generations_.size()) {
        generations_.resize(baseId + 1);
      }
    }
    const EntityID packed =
        static_cast<EntityID>(baseId) | (static_cast<EntityID>(generations_[baseId]) << kEntityGenerationOffset);
    return Entity(packed);
  }

  void BlamEntity(const Entity entity) {
    const std::uint32_t baseId = entity.GetId();
    if (baseId >= generations_.size()) return;
    generations_[baseId]++;
    available_entities_.push(baseId);
  }

  [[nodiscard]] bool IsValid(const Entity entity) const {
    const std::uint32_t baseId = entity.GetId();
    return baseId < generations_.size() && generations_[baseId] == entity.GetGeneration();
  }

 private:
  std::queue<std::uint32_t> available_entities_;
  std::vector<EntityGeneration> generations_;
  EntityID living_entity_count_ = 0;
};