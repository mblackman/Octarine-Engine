#pragma once

#include <any>
#include <memory>
#include <stdexcept>
#include <string>

#include "Component.h"
#include "Entity.h"
#include "General/Logger.h"
#include "System.h"

class Query;

template <typename... TComponents>
class ComponentQuery;

namespace Internal {
inline ComponentID GetNextComponentID() {
  // This provides a simple incrementing counter for component types.
  static ComponentID lastID = 0;
  return lastID++;
}

template <typename T>
inline ComponentID GetComponentID() {
  // For each component type T, this will be instantiated once.
  // The static id will be initialized with a unique ID from the counter
  // on its first call, and that same ID will be returned on subsequent calls.
  static const ComponentID id = GetNextComponentID();
  return id;
}

}  // namespace Internal

class ComponentRegistry {
 public:
  template <typename T>
  void RegisterComponent(const Entity& componentEntity) {
    const EntityID id = componentEntity.id;

    if (id >= component_infos_.size()) {
      component_infos_.resize(id + 1);
    } else if (component_infos_[id].id != 0) {
      return;
    }

    component_infos_[id] = {.id = id, .name = "TODO", .size = sizeof(T), .alignment = alignof(T)};
  }

  [[nodiscard]] const ComponentInfo& GetInfo(const EntityID id) const { return component_infos_[id]; }

 private:
  std::vector<ComponentInfo> component_infos_;
};

class Registry {
 public:
  template <typename... TComponents>
  friend class ComponentQuery;

  Registry() : root_archetype_(new Archetype({})) {
    entity_manager_ = std::make_unique<EntityManager>();
    component_registry_ = std::make_unique<ComponentRegistry>();
  }

  void Update(float deltaTime);

  // Entity management
  Entity CreateEntity();

  void BlamEntity(Entity entity);

  // Component management
  template <typename T>
  Entity Component() {
    static ComponentID id = Internal::GetComponentID<T>();
    if (!component_to_entity_.contains(id)) {
      const auto entity = CreateEntity();
      component_to_entity_[id] = entity;
      component_registry_->RegisterComponent<T>(entity);
      return entity;
    }
    return component_to_entity_.at(id);
  }

  template <typename T>
  Entity Component() const {
    static ComponentID id = Internal::GetComponentID<T>();
    if (!component_to_entity_.contains(id)) {
      throw std::runtime_error("Component type " + std::string(typeid(T).name()) +
                               " has not been used yet. Cannot get component.");
    }
    return component_to_entity_.at(id);
  }

  template <typename T>
  void AddComponent(const Entity entity, T component) {
    const auto& oldLocation = entity_locations_[entity.id];
    const auto componentEntity = Component<T>();

    Archetype* newArchetype = nullptr;

    auto it = oldLocation.archetype->edges.find(componentEntity.id);
    if (it != oldLocation.archetype->edges.end() && it->second.add != nullptr) {
      newArchetype = it->second.add;
    } else {
      std::vector<ComponentInfo> componentInfos;
      componentInfos.reserve(oldLocation.archetype->type().size() + 1);
      componentInfos.push_back(component_registry_->GetInfo(componentEntity.id));

      for (const auto& id : oldLocation.archetype->type()) {
        componentInfos.push_back(component_registry_->GetInfo(id));
      }

      newArchetype = new Archetype(componentInfos);
      newArchetype->edges.insert(std::make_pair(componentEntity.id, ArchetypeEdge{nullptr, oldLocation.archetype}));

      if (it != oldLocation.archetype->edges.end()) {
        it->second.add = newArchetype;
      } else {
        oldLocation.archetype->edges.insert(std::make_pair(componentEntity.id, ArchetypeEdge{newArchetype, nullptr}));
      }
    }

    const EntityLocation newLocation = newArchetype->AddEntity(entity);
    newArchetype->AddComponent(newLocation, componentEntity, component);

    newArchetype->CopyComponents(oldLocation, newLocation);

    oldLocation.archetype->RemoveEntity(oldLocation);

    entity_locations_[entity.id] = newLocation;
  }

  // template <typename T>
  // void RemoveComponent(Entity entity);

  template <typename T>
  T& GetComponent(const Entity entity) const {
    const auto it = entity_locations_.find(entity.id);
    if (it == entity_locations_.end()) {
      throw std::runtime_error("Failed to get required component " + std::string(typeid(T).name()) + " for entity " +
                               std::to_string(entity.id));
    }
    const auto componentEntity = Component<T>();
    const auto [archetype, chunkIndex, indexInChunk] = it->second;

    if (!archetype->HasComponent(componentEntity.id)) {
      throw std::runtime_error("Failed to get required component " + std::string(typeid(T).name()) + " for entity " +
                               std::to_string(entity.id));
    }
    const auto componentArray = archetype->GetComponentArray<T>(chunkIndex, componentEntity.id);

    if (componentArray == nullptr) {
      throw std::runtime_error("Failed to get required component " + std::string(typeid(T).name()) + " for entity " +
                               std::to_string(entity.id));
    }

    return componentArray[indexInChunk];
  }

  template <typename T>
  [[nodiscard]] bool HasComponent(const Entity entity) const {
    const auto it = entity_locations_.find(entity.id);
    if (it == entity_locations_.end()) {
      Logger::Warn("Could not find entity with ID: " + std::to_string(entity.id) + " to check for component");
      return false;
    }
    const auto componentEntity = Component<T>();
    return it->second.archetype->HasComponent(componentEntity.id);
  }

  [[nodiscard]] Archetype* GetMatchingArchetype(const ArchetypeType& type) const;
  [[nodiscard]] std::vector<Archetype*> GetMatchingArchetypes(const ArchetypeType& type) const;

  // Queries
  template <typename... TComponents>
  std::unique_ptr<ComponentQuery<TComponents...>> CreateQuery() {
    auto query = std::make_unique<ComponentQuery<TComponents...>>(this);
    query->Update();
    return query;
  }

  // System Management
  template <typename... TArgs, typename Func>
  void RegisterSystem(Func&& func) {
    class SystemWrapper final : public ISystem {
     public:
      explicit SystemWrapper(Registry* registry, Func&& f)
          : func_(std::forward<Func>(f)), query_(registry->CreateQuery<TArgs...>()) {}

      void Update(const Registry& registry) override {
        query_->Update();
        query_->ForEach(this->func_);
      }

     private:
      Func func_;
      std::unique_ptr<ComponentQuery<TArgs...>> query_;
    };

    systems_.push_back(std::make_unique<SystemWrapper>(this, std::forward<Func>(func)));
  }

  // Singleton components
  template <typename T>
  T& Set(T value) {
    const auto entityComponent = Component<T>();
    singleton_components_[entityComponent.id] = std::move(value);
    return std::any_cast<T&>(singleton_components_.at(entityComponent.id));
  }

  template <typename T>
  const T& Get() const {
    const auto entityComponent = Component<T>();
    try {
      return std::any_cast<const T&>(singleton_components_.at(entityComponent.id));

    } catch (const std::out_of_range& e) {
      throw std::runtime_error("Attempted to Get a singleton component that has not been Set.");
    } catch (const std::bad_any_cast& e) {
      throw std::runtime_error("Type mismatch in Get. This indicates a logic error.");
    }
  }

  template <typename T>
  T& Get() {
    return const_cast<T&>(static_cast<const std::decay_t<decltype(*this)>&>(*this).Get<T>());
  }

  // Relationships
  void AddPair(Entity entity, Entity relationship, Entity target);

  bool HasPair(Entity entity, Entity relationship, Entity target);

 private:
  std::unique_ptr<EntityManager> entity_manager_;
  std::unique_ptr<ComponentRegistry> component_registry_;
  std::unordered_map<EntityID, EntityLocation> entity_locations_;
  std::unordered_map<ArchetypeID, std::unique_ptr<Archetype>> archetypes_;
  Archetype* root_archetype_ = nullptr;  // The root of the archetype graph. Empty signature.
  std::vector<std::unique_ptr<ISystem>> systems_;
  std::unordered_map<EntityID, std::any> singleton_components_;
  std::unordered_map<ComponentID, Entity> component_to_entity_;
  float delta_time_{};
};
