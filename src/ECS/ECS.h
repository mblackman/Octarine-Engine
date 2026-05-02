#pragma once
#include <vector>

typedef std::uint64_t EcsId;

typedef EcsId EntityID;

typedef EcsId ComponentID;

typedef EcsId ArchetypeID;

typedef EcsId RelationshipID;

constexpr unsigned int kPairRelationshipOffset = 32;

typedef std::vector<ComponentID> ArchetypeType;

struct Pair {
  EcsId id;

  explicit Pair(const EcsId id) : id(id) {}

  [[nodiscard]] std::uint32_t GetTarget() const { return static_cast<std::uint32_t>(id); }
  [[nodiscard]] std::uint32_t GetRelationship() const {
    return static_cast<std::uint32_t>(id >> kPairRelationshipOffset);
  }
};
