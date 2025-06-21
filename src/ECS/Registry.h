#pragma once

#include <map>

#include "Component.h"
#include "Entity.h"

class Registry {
 public:
  Registry() = default;

  // Entity management
  Entity CreateEntity();

  void BlamEntity(Entity entity);

  // Component management
  template <typename T>
  void AddComponent(Entity entity, T component);

  template <typename T>
  void RemoveComponent(Entity entity);

  template <typename T>
  T& GetComponent(Entity entity);

  template <typename T>
  bool HasComponent(Entity entity);

 private:
  std::unique_ptr<Archetype> FindOrCreateArchetype(Signature signature);

  std::unique_ptr<EntityManager> entity_manager_;
  std::unique_ptr<ComponentRegistry> component_registry_;
  std::unordered_map<EntityID, EntityLocation> entity_locations_;
  std::map<Signature, std::unique_ptr<Archetype>> archetypes_;
  std::unique_ptr<Archetype> root_archetype_;  // The root of the archetype graph. Empty signature.
};