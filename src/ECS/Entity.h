#pragma once
#include <bitset>
#include <cstdint>
#include <optional>
#include <queue>

constexpr unsigned int kMaxEntityMasks = 32;
constexpr unsigned int kStartingEntityPoolSize = 1000;

// Used to determine which
typedef std::bitset<kMaxEntityMasks> EntityMask;
typedef std::uint32_t EntityID;
typedef std::uint16_t EntityGeneration;

struct Entity {
  EntityID id;
  EntityGeneration generation;

  [[nodiscard]] EntityID GetId() const { return id; }

  bool operator==(const Entity& other) const { return id == other.id && generation == other.generation; }

  bool operator!=(const Entity& other) const { return id != other.id || generation != other.generation; }

  bool operator<(const Entity& other) const { return id < other.id && generation < other.generation; }

  bool operator>(const Entity& other) const { return id > other.id && generation > other.generation; }

  bool operator<=(const Entity& other) const { return id <= other.id && generation <= other.generation; }

  bool operator>=(const Entity& other) const { return id >= other.id && generation >= other.generation; }

  void Tag(const std::string& tag) const;
  [[nodiscard]] bool HasTag(const std::string& tag) const;
  void Group(const std::string& group) const;
  [[nodiscard]] bool InGroup(const std::string& group) const;

  void SetEntityMask(EntityMask entityMask) const;
  [[nodiscard]] EntityMask GetEntityMask() const;

  void Blam() const;

  void AddParent(const Entity& parent) const;

  void RemoveParent() const;

  [[nodiscard]] std::optional<Entity> GetParent() const;

  [[nodiscard]] std::optional<std::vector<Entity>> GetChildren() const;
};

// TODO need to handle recycling entity generations
class EntityManager {
 public:
  EntityManager() {
    for (uint32_t i = 0; i < kStartingEntityPoolSize; ++i) {
      available_entities_.push(i);
    }
    generations_.resize(kStartingEntityPoolSize);
  }

  Entity CreateEntity() {
    uint32_t id;
    if (!available_entities_.empty()) {
      id = available_entities_.front();
      available_entities_.pop();
    } else {
      id = living_entity_count_++;
      if (id >= generations_.size()) {
        generations_.resize(id + 1);
      }
    }
    return {id, generations_[id]};
  }

  void BlamEntity(const Entity entity) {
    generations_[entity.id]++;
    available_entities_.push(entity.id);
  }

  [[nodiscard]] bool IsValid(const Entity entity) const {
    return entity.id < generations_.size() && generations_[entity.id] == entity.generation;
  }

 private:
  std::queue<EntityID> available_entities_;
  std::vector<EntityGeneration> generations_;
  EntityID living_entity_count_ = 0;
};