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

  explicit Entity(const EcsId id) : id(id) {}

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
    EntityID id;
    if (!available_entities_.empty()) {
      id = available_entities_.front();
      available_entities_.pop();
    } else {
      id = living_entity_count_++;
      if (id >= generations_.size()) {
        generations_.resize(id + 1);
      }
    }
    id += (generations_[id] << kEntityGenerationOffset);
    return Entity(id);
  }

  void BlamEntity(const Entity entity) {
    generations_[entity.id]++;
    available_entities_.push(entity.id);
  }

  [[nodiscard]] bool IsValid(const Entity entity) const {
    return entity.id < generations_.size() && generations_[entity.id] == entity.GetGeneration();
  }

 private:
  std::queue<EntityID> available_entities_;
  std::vector<EntityGeneration> generations_;
  EntityID living_entity_count_ = 0;
};