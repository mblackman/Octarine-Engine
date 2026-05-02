#include "Registry.h"

#include "../General/Logger.h"

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

std::unique_ptr<Archetype> Registry::FindOrCreateArchetype(Signature signature) {
  if (const auto it = archetypes_.find(signature); it != archetypes_.end()) {
    return std::move(it->second);
  }

  archetypes_.emplace(signature, std::make_unique<Archetype>(signature, *component_registry_));
  return std::move(archetypes_.at(signature));
}

template <typename T>
void Registry::AddComponent(const Entity entity, T component) {
  const auto& oldLocation = entity_locations_[entity.id];
  Signature newSignature = oldLocation.archetype->GetSignature();
  const auto componentId = GetComponentTypeID<T>();
  newSignature.set(componentId);

  Archetype* newArchetype = nullptr;

  auto it = oldLocation.archetype->edges.find(componentId);
  if (it != oldLocation.archetype->edges.end() && it->second.add) {
    newArchetype = it->second.add;
  } else {
    newArchetype = FindOrCreateArchetype(newSignature).get();
    if (it != oldLocation.archetype->edges.end()) {
      it->second.add = newArchetype;
    } else {
      oldLocation.archetype->edges.emplace(componentId, {newArchetype, nullptr});
    }
  }

  const EntityLocation newLocation = newArchetype->AddEntity(entity);
  newArchetype->AddComponent(newLocation, component);

  newArchetype->CopyComponents(oldLocation, newLocation);

  oldLocation.archetype->RemoveEntity(oldLocation);

  entity_locations_[entity.id] = newLocation;
}
template <typename T>
T& Registry::GetComponent(const Entity entity) {
  const auto it = entity_locations_.find(entity.id);
  if (it == entity_locations_.end()) {
    Logger::Warn("Could not find entity with ID: " + std::to_string(entity.id) + " to get component");
    return static_cast<T&>(nullptr);
  }
  const auto componentId = GetComponentTypeID<T>();
  const auto [archetype, chunkIndex, indexInChunk] = it->second;

  if (!archetype->GetSignature().test(componentId)) {
    return static_cast<T&>(nullptr);
  }
  const auto componentArray = archetype->GetComponentArray<T>(chunkIndex);

  if (componentArray == nullptr) {
    return static_cast<T&>(nullptr);
  }

  return componentArray[indexInChunk];
}

template <typename T>
bool Registry::HasComponent(const Entity entity) {
  const auto it = entity_locations_.find(entity.id);
  if (it == entity_locations_.end()) {
    Logger::Warn("Could not find entity with ID: " + std::to_string(entity.id) + " to check for component");
    return false;
  }
  const auto componentId = GetComponentTypeID<T>();
  return it->second.archetype->GetSignature().test(componentId);
}
