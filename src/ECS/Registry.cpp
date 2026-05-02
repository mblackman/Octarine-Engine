#include "Registry.h"

#include <ranges>

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

  it->second.archetype->RemoveEntity(it->second);
  entity_manager_->BlamEntity(entity);
  entity_locations_.erase(it);
}

Archetype* Registry::GetMatchingArchetype(const ArchetypeType& type) const {
  Archetype* currentNode = root_archetype_;
  if (!type.empty()) {
    for (const ComponentID id : type) {
      const auto it = currentNode->edges.find(id);
      if (it == currentNode->edges.end() || it->second.add == nullptr) {
        return nullptr;
      }
      currentNode = it->second.add;
    }
  }

  return currentNode;
}

std::vector<Archetype*> Registry::GetMatchingArchetypes(const ArchetypeType& type) const {
  Archetype* start_node = GetMatchingArchetype(type);

  std::vector<Archetype*> matching_archetypes;
  std::queue<Archetype*> to_visit;
  std::unordered_set<Archetype*> visited;

  if (start_node) {
    to_visit.push(start_node);
    visited.insert(start_node);
  }

  while (!to_visit.empty()) {
    Archetype* current = to_visit.front();
    to_visit.pop();

    matching_archetypes.push_back(current);

    for (const auto& [add, remove] : current->edges | std::views::values) {
      if (add != nullptr && !visited.contains(add)) {
        visited.insert(add);
        to_visit.push(add);
      }
    }
  }

  return matching_archetypes;
}
