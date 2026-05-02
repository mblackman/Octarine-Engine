#include "Registry.h"

#include <algorithm>

#include "../General/Logger.h"
#include "Query.h"

void Registry::Update(const float deltaTime) {
  delta_time_ = deltaTime;
  for (const auto& system : systems_) {
    system->Update(*this);
  }
}

Entity Registry::CreateEntity() {
  const Entity entity = entity_manager_->CreateEntity();
  const auto entityLocation = root_archetype_->AddEntity(entity);
  entity_locations_[entity.id] = entityLocation;
  return entity;
}

void Registry::BlamEntity(const Entity entity) {
  const auto it = entity_locations_.find(entity.id);
  if (it == entity_locations_.end()) {
    Logger::Warn("Could not find entity with ID: " + std::to_string(entity.id) + " to blam");
    return;
  }

  const EntityLocation removedLocation = it->second;
  const auto swapped = removedLocation.archetype->RemoveEntity(removedLocation);
  entity_manager_->BlamEntity(entity);
  entity_locations_.erase(it);
  if (swapped) {
    entity_locations_[swapped->id] = removedLocation;
  }
}

std::vector<Archetype*> Registry::GetMatchingArchetypes(const ArchetypeType& type) const {
  if (type.empty()) {
    return {};
  }

  const ArchetypeList* smallest = nullptr;
  for (const ComponentID id : type) {
    const auto it = component_index_.find(id);
    if (it == component_index_.end()) {
      return {};
    }
    if (smallest == nullptr || it->second.size() < smallest->size()) {
      smallest = &it->second;
    }
  }

  std::vector<Archetype*> matching_archetypes;
  matching_archetypes.reserve(smallest->size());

  for (const ArchetypeID aid : *smallest) {
    const auto archIt = archetypes_.find(aid);
    if (archIt == archetypes_.end()) {
      continue;
    }
    const auto& archetypeType = archIt->second->type();

    // Both `type` and `archetypeType` are sorted ascending; two-pointer superset check.
    size_t i = 0;
    size_t j = 0;
    while (i < type.size() && j < archetypeType.size()) {
      if (type[i] == archetypeType[j]) {
        ++i;
        ++j;
      } else if (archetypeType[j] < type[i]) {
        ++j;
      } else {
        break;
      }
    }
    if (i == type.size()) {
      matching_archetypes.push_back(archIt->second.get());
    }
  }

  return matching_archetypes;
}

Archetype* Registry::GetOrCreateArchetype(std::vector<ComponentID> componentIDs, const ComponentID newComponentId) {
  componentIDs.push_back(newComponentId);
  std::ranges::sort(componentIDs);

  if (const auto it = component_index_.find(newComponentId); it != component_index_.end()) {
    for (const auto archetypeId : it->second) {
      const auto archetype = archetypes_.find(archetypeId);
      if (archetype->second->type().size() != componentIDs.size()) {
        continue;
      }

      bool match = true;
      for (size_t i = 0; i < componentIDs.size(); i++) {
        if (archetype->second->type()[i] != componentIDs[i]) {
          match = false;
          break;
        }
      }

      if (match) {
        return archetype->second.get();
      }
    }
  }

  std::vector<ComponentInfo> componentInfos;
  componentInfos.reserve(componentIDs.size());
  for (const auto& id : componentIDs) {
    componentInfos.push_back(component_registry_->GetInfo(id));
  }

  auto newArchetype = std::make_unique<Archetype>(std::move(componentInfos));
  Archetype* newArchetypePtr = newArchetype.get();
  const auto newArchetypeId = newArchetype->GetID();
  archetypes_.emplace(newArchetypeId, std::move(newArchetype));

  // Centralized component_index_ population — every archetype is registered here.
  for (const ComponentID id : newArchetypePtr->type()) {
    auto& list = component_index_[id];
    if (std::ranges::find(list, newArchetypeId) == list.end()) {
      list.push_back(newArchetypeId);
    }
  }

  return newArchetypePtr;
}

void Registry::AddPair(const Entity entity, const Entity relationship, const Entity target) {
  const EcsId pairId =
      (static_cast<EcsId>(relationship.GetId()) << kPairRelationshipOffset) | static_cast<EcsId>(target.GetId());
  pairs_[entity.id].insert(pairId);
}

bool Registry::HasPair(const Entity entity, const Entity relationship, const Entity target) {
  const auto it = pairs_.find(entity.id);
  if (it == pairs_.end()) return false;
  const EcsId pairId =
      (static_cast<EcsId>(relationship.GetId()) << kPairRelationshipOffset) | static_cast<EcsId>(target.GetId());
  return it->second.contains(pairId);
}
