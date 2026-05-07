#include "Registry.h"

#include <algorithm>

#include "../General/Logger.h"
#include "../General/PerfUtils.h"
#include "Query.h"

void Registry::Update(const float deltaTime) {
  PROFILE_NAMED_SCOPE("Registry::Update (total)");
  delta_time_ = deltaTime;
  for (size_t i = 0; i < systems_.size(); ++i) {
#ifdef OCTARINE_PROFILING
    ACCUMULATE_PROFILE_SCOPE(systems_[i]->GetName());
#endif
    systems_[i]->Update(*this);
  }
  if (!pending_blams_.empty()) {
    auto pending = std::move(pending_blams_);
    pending_blams_.clear();
    pending_blam_ids_.clear();
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
  ++user_entity_count_;
  return entity;
}

Entity Registry::CreateInternalEntity() {
  const Entity entity = entity_manager_->CreateEntity();
  const auto entityLocation = root_archetype_->AddEntity(entity);
  const std::uint32_t id = entity.GetId();
  if (id >= entity_locations_.size()) {
    entity_locations_.resize(id + 1, EntityLocation{nullptr, 0, 0});
  }
  entity_locations_[id] = entityLocation;
  internal_entity_ids_.insert(entity.id);
  return entity;
}

void Registry::BlamEntity(const Entity entity) {
  const std::uint32_t id = entity.GetId();
  if (id >= entity_locations_.size() || !entity_locations_[id].archetype || !entity_manager_->IsValid(entity)) {
    Logger::Warn("Could not find entity with ID: " + std::to_string(entity.id) + " to blam");
    return;
  }

  bool hierarchyMutated = false;

  // Cascade-destroy children first. Take a copy so the recursive BlamEntity calls can mutate
  // parent_to_children_ without invalidating our iteration.
  if (const auto childIt = parent_to_children_.find(entity.id); childIt != parent_to_children_.end()) {
    const auto childIds = childIt->second;
    parent_to_children_.erase(childIt);
    for (const EntityID childId : childIds) {
      child_to_parent_.erase(childId);
      BlamEntity(Entity(childId));
    }
    hierarchyMutated = true;
  }

  // Detach from own parent.
  if (const auto parentIt = child_to_parent_.find(entity.id); parentIt != child_to_parent_.end()) {
    if (const auto p = parent_to_children_.find(parentIt->second.id); p != parent_to_children_.end()) {
      p->second.erase(entity.id);
      if (p->second.empty()) parent_to_children_.erase(p);
    }
    child_to_parent_.erase(parentIt);
    hierarchyMutated = true;
  }

  if (hierarchyMutated) ++hierarchy_generation_;

  const EntityLocation removedLocation = entity_locations_[id];
  const auto swapped = removedLocation.archetype->RemoveEntity(removedLocation);
  entity_manager_->BlamEntity(entity);
  entity_locations_[id] = EntityLocation{nullptr, 0, 0};
  if (swapped) {
    entity_locations_[swapped->GetId()] = removedLocation;
  }
  if (internal_entity_ids_.erase(entity.id) == 0 && user_entity_count_ > 0) {
    --user_entity_count_;
  }

  // Drop relationship entries authored by this entity, and any pair targeting it.
  pairs_.erase(entity.id);
  const auto targetId = entity.GetId();
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

std::vector<Entity> Registry::GetUserEntities() const {
  std::vector<Entity> entities;
  entities.reserve(user_entity_count_);
  for (std::uint32_t id = 0; id < entity_locations_.size(); ++id) {
    if (entity_locations_[id].archetype != nullptr && !internal_entity_ids_.contains(id)) {
      // We need the full Entity from the archetype.
      // But archetype stores a packed chunk array of Entity IDs.
      // Let's just create an entity with the correct generation by asking entity_manager_... wait.
      // EntityManager doesn't expose getting a generation by ID easily.
      // Let's get the Entity directly from the archetype.
      const auto location = entity_locations_[id];
      const Entity entity = location.archetype->GetEntity(location.chunkIndex, location.indexInChunk);
      entities.push_back(entity);
    }
  }
  return entities;
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
  ++archetype_generation_;

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
  ++archetype_generation_;

  for (const ComponentID id : newArchetypePtr->type()) {
    auto& list = component_index_[id];
    if (std::ranges::find(list, newArchetypeId) == list.end()) {
      list.push_back(newArchetypeId);
    }
  }

  return newArchetypePtr;
}

Archetype* Registry::GetOrCreateArchetypeFromSet(std::vector<ComponentID> componentIDs) {
  std::ranges::sort(componentIDs);
  componentIDs.erase(std::ranges::unique(componentIDs).begin(), componentIDs.end());

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
  for (const auto& cid : componentIDs) {
    componentInfos.push_back(component_registry_->GetInfo(cid));
  }

  auto newArchetype = std::make_unique<Archetype>(std::move(componentInfos));
  Archetype* newArchetypePtr = newArchetype.get();
  const auto newArchetypeId = newArchetype->GetID();
  archetypes_.emplace(newArchetypeId, std::move(newArchetype));
  ++archetype_generation_;

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
  // Detach previous parent, if any.
  if (const auto it = child_to_parent_.find(child.id); it != child_to_parent_.end()) {
    if (const auto p = parent_to_children_.find(it->second.id); p != parent_to_children_.end()) {
      p->second.erase(child.id);
      if (p->second.empty()) parent_to_children_.erase(p);
    }
  }
  child_to_parent_[child.id] = parent;
  parent_to_children_[parent.id].insert(child.id);
  ++hierarchy_generation_;

  // Maintain the legacy pair entry too so HasPair / generic relationship queries keep working.
  const Entity childOf = ChildOfEntity();
  AddPair(child, childOf, parent);
}

std::optional<Entity> Registry::GetParent(const Entity child) const {
  const auto it = child_to_parent_.find(child.id);
  if (it == child_to_parent_.end()) return std::nullopt;
  return it->second;
}

std::vector<Entity> Registry::GetChildren(const Entity parent) const {
  std::vector<Entity> children;
  const auto it = parent_to_children_.find(parent.id);
  if (it == parent_to_children_.end()) return children;
  children.reserve(it->second.size());
  for (const EntityID id : it->second) {
    children.emplace_back(id);
  }
  return children;
}
