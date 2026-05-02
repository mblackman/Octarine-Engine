#include "Registry.h"

#include <algorithm>

#include "../General/Logger.h"
#include "Query.h"

void Registry::Update(const float deltaTime) {
  delta_time_ = deltaTime;
  for (const auto& system : systems_) {
    system->Update(*this);
  }
  if (!pending_blams_.empty()) {
    auto pending = std::move(pending_blams_);
    pending_blams_.clear();
    for (const Entity entity : pending) {
      BlamEntity(entity);
    }
  }
}

Entity Registry::CreateEntity() {
  const Entity entity = entity_manager_->CreateEntity();
  const auto entityLocation = root_archetype_->AddEntity(entity);
  const std::uint32_t id = entity.GetId();
  if (id >= entity_locations_.size()) {
    entity_locations_.resize(id + 1, EntityLocation{nullptr, 0, 0});
  }
  entity_locations_[id] = entityLocation;
  return entity;
}

void Registry::BlamEntity(const Entity entity) {
  const std::uint32_t id = entity.GetId();
  if (id >= entity_locations_.size() || !entity_locations_[id].archetype || !entity_manager_->IsValid(entity)) {
    Logger::Warn("Could not find entity with ID: " + std::to_string(entity.id) + " to blam");
    return;
  }

  const EntityLocation removedLocation = entity_locations_[id];
  const auto swapped = removedLocation.archetype->RemoveEntity(removedLocation);
  entity_manager_->BlamEntity(entity);
  entity_locations_[id] = EntityLocation{nullptr, 0, 0};
  if (swapped) {
    entity_locations_[swapped->GetId()] = removedLocation;
  }

  // Drop relationship entries authored by this entity, and any pair targeting it.
  pairs_.erase(entity.id);
  const auto targetId = static_cast<std::uint32_t>(entity.GetId());
  for (auto pairIt = pairs_.begin(); pairIt != pairs_.end();) {
    auto& pairSet = pairIt->second;
    for (auto setIt = pairSet.begin(); setIt != pairSet.end();) {
      if (Pair(*setIt).GetTarget() == targetId) {
        setIt = pairSet.erase(setIt);
      } else {
        ++setIt;
      }
    }
    if (pairSet.empty()) {
      pairIt = pairs_.erase(pairIt);
    } else {
      ++pairIt;
    }
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

EntityLocation Registry::TransitionAddComponent(const Entity entity, const ComponentID componentId) {
  const std::uint32_t id = entity.GetId();
  if (id >= entity_locations_.size() || !entity_locations_[id].archetype || !entity_manager_->IsValid(entity)) {
    Logger::Warn("TransitionAddComponent called on missing entity " + std::to_string(entity.id));
    return {nullptr, 0, 0};
  }
  const EntityLocation oldLocation = entity_locations_[id];
  if (oldLocation.archetype->HasComponent(componentId)) {
    return oldLocation;
  }

  Archetype* newArchetype = nullptr;
  auto it = oldLocation.archetype->edges.find(componentId);
  if (it != oldLocation.archetype->edges.end() && it->second.add != nullptr) {
    newArchetype = it->second.add;
  } else {
    newArchetype = GetOrCreateArchetype(oldLocation.archetype->type(), componentId);

    newArchetype->edges.insert(std::make_pair(componentId, ArchetypeEdge{nullptr, oldLocation.archetype}));
    if (it != oldLocation.archetype->edges.end()) {
      it->second.add = newArchetype;
    } else {
      oldLocation.archetype->edges.insert(std::make_pair(componentId, ArchetypeEdge{newArchetype, nullptr}));
    }
  }

  const EntityLocation newLocation = newArchetype->AddEntity(entity);
  newArchetype->CopyComponents(oldLocation, newLocation);

  const auto swapped = oldLocation.archetype->RemoveEntity(oldLocation);
  entity_locations_[id] = newLocation;
  if (swapped) {
    entity_locations_[swapped->GetId()] = oldLocation;
  }
  return newLocation;
}

EntityLocation Registry::TransitionRemoveComponent(const Entity entity, const ComponentID componentId) {
  const std::uint32_t id = entity.GetId();
  if (id >= entity_locations_.size() || !entity_locations_[id].archetype || !entity_manager_->IsValid(entity)) {
    Logger::Warn("TransitionRemoveComponent called on missing entity " + std::to_string(entity.id));
    return {nullptr, 0, 0};
  }
  const EntityLocation oldLocation = entity_locations_[id];
  if (!oldLocation.archetype->HasComponent(componentId)) {
    return oldLocation;
  }

  Archetype* newArchetype = nullptr;
  auto it = oldLocation.archetype->edges.find(componentId);
  if (it != oldLocation.archetype->edges.end() && it->second.remove != nullptr) {
    newArchetype = it->second.remove;
  } else {
    newArchetype = GetOrCreateArchetypeRemove(oldLocation.archetype->type(), componentId);

    newArchetype->edges.insert(std::make_pair(componentId, ArchetypeEdge{oldLocation.archetype, nullptr}));
    if (it != oldLocation.archetype->edges.end()) {
      it->second.remove = newArchetype;
    } else {
      oldLocation.archetype->edges.insert(std::make_pair(componentId, ArchetypeEdge{nullptr, newArchetype}));
    }
  }

  const EntityLocation newLocation = newArchetype->AddEntity(entity);
  newArchetype->CopyComponents(oldLocation, newLocation);

  const auto swapped = oldLocation.archetype->RemoveEntity(oldLocation);
  entity_locations_[id] = newLocation;
  if (swapped) {
    entity_locations_[swapped->GetId()] = oldLocation;
  }
  return newLocation;
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

Archetype* Registry::GetOrCreateArchetypeRemove(std::vector<ComponentID> componentIDs,
                                                const ComponentID removeComponentId) {
  std::erase(componentIDs, removeComponentId);
  std::ranges::sort(componentIDs);

  if (componentIDs.empty()) {
    return root_archetype_.get();
  }

  if (const auto it = component_index_.find(componentIDs.front()); it != component_index_.end()) {
    for (const auto archetypeId : it->second) {
      const auto archetype = archetypes_.find(archetypeId);
      if (archetype->second->type().size() != componentIDs.size()) {
        continue;
      }

      bool match = true;
      for (size_t i = 0; i < componentIDs.size(); ++i) {
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

void Registry::SetParent(const Entity child, const Entity parent) {
  const Entity childOf = ChildOfEntity();
  AddPair(child, childOf, parent);
}

std::optional<Entity> Registry::GetParent(const Entity child) const {
  const auto it = pairs_.find(child.id);
  if (it == pairs_.end()) return std::nullopt;

  const size_t type_idx = GetComponentTypeIndex<ChildOfRelation>();
  if (type_idx >= fast_component_to_entity_.size() || !fast_component_to_entity_[type_idx].has_value()) return std::nullopt;
  const auto childOfRelationId = static_cast<std::uint32_t>(fast_component_to_entity_[type_idx].value().GetId());

  for (const EcsId rawPair : it->second) {
    const Pair p(rawPair);
    if (p.GetRelationship() == childOfRelationId) {
      return Entity(static_cast<EcsId>(p.GetTarget()));
    }
  }
  return std::nullopt;
}

std::vector<Entity> Registry::GetChildren(const Entity parent) const {
  std::vector<Entity> children;

  const size_t type_idx = GetComponentTypeIndex<ChildOfRelation>();
  if (type_idx >= fast_component_to_entity_.size() || !fast_component_to_entity_[type_idx].has_value()) return children;
  const auto childOfRelationId = static_cast<std::uint32_t>(fast_component_to_entity_[type_idx].value().GetId());
  const auto parentId = static_cast<std::uint32_t>(parent.GetId());

  for (const auto& [entityId, pairSet] : pairs_) {
    for (const EcsId rawPair : pairSet) {
      const Pair p(rawPair);
      if (p.GetRelationship() == childOfRelationId && p.GetTarget() == parentId) {
        children.emplace_back(entityId);
        break;
      }
    }
  }
  return children;
}
