#pragma once

#include <map>

#include "Component.h"
#include "Entity.h"

class Query {};

template <typename... TComponents>
class ComponentQuery;

class Registry {
 public:
  Registry() {
    entity_manager_ = std::make_unique<EntityManager>();
    component_registry_ = std::make_unique<ComponentRegistry>();
  }

  // Entity management
  Entity CreateEntity();

  void BlamEntity(Entity entity);

  // Component management
  template <typename T>
  void AddComponent(const Entity entity, T component) {
    component_registry_->RegisterComponent<T>();
    const auto& oldLocation = entity_locations_[entity.id];
    Signature newSignature = oldLocation.archetype->GetSignature();
    const auto componentId = GetComponentTypeID<T>();
    newSignature.set(componentId);

    Archetype* newArchetype = nullptr;

    auto it = oldLocation.archetype->edges.find(componentId);
    if (it != oldLocation.archetype->edges.end() && it->second.add != nullptr) {
      newArchetype = it->second.add;
    } else {
      newArchetype = FindOrCreateArchetype(newSignature);
      if (it != oldLocation.archetype->edges.end()) {
        it->second.add = newArchetype;
      } else {
        oldLocation.archetype->edges.insert(std::make_pair(componentId, ArchetypeEdge{newArchetype, nullptr}));
      }
    }

    const EntityLocation newLocation = newArchetype->AddEntity(entity);
    newArchetype->AddComponent(newLocation, component);

    newArchetype->CopyComponents(oldLocation, newLocation);

    oldLocation.archetype->RemoveEntity(oldLocation);

    entity_locations_[entity.id] = newLocation;
  }

  // template <typename T>
  // void RemoveComponent(Entity entity);

  template <typename T>
  T& GetComponent(const Entity entity) {
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
  bool HasComponent(const Entity entity) {
    const auto it = entity_locations_.find(entity.id);
    if (it == entity_locations_.end()) {
      Logger::Warn("Could not find entity with ID: " + std::to_string(entity.id) + " to check for component");
      return false;
    }
    const auto componentId = GetComponentTypeID<T>();
    return it->second.archetype->GetSignature().test(componentId);
  }

  // Archetype management
  [[nodiscard]] std::vector<Archetype*> GetMatchingArchetypes(Signature signature) const;

  // Queries
  template <typename... TComponents>
  ComponentQuery<TComponents...>& CreateQuery() {
    const auto signature = GetSignature<TComponents...>();
    auto it = queries_.find(signature);

    if (it == queries_.end()) {
      auto newQuery = std::make_unique<ComponentQuery<TComponents...>>(this);

      it = queries_.emplace(signature, std::move(newQuery)).first;
    }
    Query* base_ptr = it->second.get();
    return *static_cast<ComponentQuery<TComponents...>*>(base_ptr);
  }

 private:
  Archetype* FindOrCreateArchetype(Signature signature);

  std::unique_ptr<EntityManager> entity_manager_;
  std::unique_ptr<ComponentRegistry> component_registry_;
  std::unordered_map<EntityID, EntityLocation> entity_locations_;
  std::unordered_map<Signature, std::unique_ptr<Archetype>> archetypes_;
  Archetype* root_archetype_ = nullptr;  // The root of the archetype graph. Empty signature.
  std::unordered_map<Signature, std::unique_ptr<Query>> queries_;
};

template <typename... TComponents>
class ComponentQuery : public Query {
 public:
  explicit ComponentQuery(Registry* registry) : registry_(registry), signature_(GetSignature<TComponents...>()) {}

  template <typename Func>
  void ForEach(Func func) {
    const std::vector<Archetype*> archetypes = registry_->GetMatchingArchetypes(signature_);

    for (auto* archetype : archetypes) {
      archetype->ForEach<Func, TComponents...>(func);
    }
  }

 private:
  Registry* registry_;
  Signature signature_;
};
