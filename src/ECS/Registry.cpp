#include "Registry.h"

#include <ranges>

#include "../General/Logger.h"

void Registry::Update(const float deltaTime) {
  delta_time_ = deltaTime;
  for (const auto& system : systems_) {
    system->Update(*this);
  }
}

Entity Registry::CreateEntity() {
  const Entity entity = entity_manager_->CreateEntity();

  if (root_archetype_ == nullptr) {
    root_archetype_ = FindOrCreateArchetype({});
  }

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

std::vector<Archetype*> Registry::GetMatchingArchetypes(const Signature signature) const {
  std::vector<Archetype*> matchingArchetypes;
  for (const auto& [key, value] : archetypes_) {
    if ((key & signature) == signature) {
      matchingArchetypes.push_back(value.get());
    }
  }
  return matchingArchetypes;
}
void Registry::UpdateQueries() {
  for (const auto& [key, val] : queries_) {
    val->Update();
  }
}

Archetype* Registry::FindOrCreateArchetype(Signature signature) {
  if (const auto it = archetypes_.find(signature); it != archetypes_.end()) {
    return it->second.get();
  }

  auto [new_iterator, success] =
      archetypes_.emplace(signature, std::make_unique<Archetype>(signature, *component_registry_));
  UpdateQueries();
  return new_iterator->second.get();
}
