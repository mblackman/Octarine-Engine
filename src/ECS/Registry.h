#pragma once

#include <any>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "Component.h"
#include "Entity.h"
#include "General/Logger.h"
#include "System.h"

class Query;

template <typename... TComponents>
class ComponentQuery;

class ComponentRegistry {
 public:
  template <typename T>
  void RegisterComponent(const Entity& componentEntity) {
    const ComponentID id = componentEntity.GetId();
    ComponentInfo info{id, typeid(T).name(), sizeof(T), alignof(T)};
    if constexpr (std::is_move_constructible_v<T>) {
      info.move_construct = [](void* dst, void* src) { ::new (dst) T(std::move(*static_cast<T*>(src))); };
    } else {
      info.move_construct = [](void* dst, void* src) { std::memcpy(dst, src, sizeof(T)); };
    }
    info.destroy = [](void* ptr) { static_cast<T*>(ptr)->~T(); };
    component_infos_.try_emplace(id, std::move(info));
  }

  [[nodiscard]] const ComponentInfo& GetInfo(const ComponentID id) const { return component_infos_.at(id); }

 private:
  std::unordered_map<ComponentID, ComponentInfo> component_infos_;
};

class Registry {
 public:
  template <typename... TComponents>
  friend class ComponentQuery;

  using ArchetypeList = std::vector<ArchetypeID>;

  Registry() : root_archetype_(std::make_unique<Archetype>(std::vector<ComponentInfo>{})) {
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
    const std::type_index key(typeid(T));
    if (const auto it = component_to_entity_.find(key); it != component_to_entity_.end()) {
      return it->second;
    }
    const auto entity = CreateEntity();
    component_to_entity_.emplace(key, entity);
    component_registry_->RegisterComponent<T>(entity);
    return entity;
  }

  template <typename T>
  Entity Component() const {
    const std::type_index key(typeid(T));
    const auto it = component_to_entity_.find(key);
    if (it == component_to_entity_.end()) {
      throw std::runtime_error("Component type " + std::string(typeid(T).name()) +
                               " has not been used yet. Cannot get component.");
    }
    return it->second;
  }

  template <typename T>
  void AddComponent(const Entity entity, T component) {
    const EntityLocation oldLocation = entity_locations_[entity.id];
    const auto componentEntity = Component<T>();
    const ComponentID componentId = componentEntity.GetId();

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
    newArchetype->AddComponent(newLocation, componentEntity, component);
    newArchetype->CopyComponents(oldLocation, newLocation);

    const auto swapped = oldLocation.archetype->RemoveEntity(oldLocation);
    entity_locations_[entity.id] = newLocation;
    if (swapped) {
      entity_locations_[swapped->id] = oldLocation;
    }
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

    if (!archetype->HasComponent(componentEntity.GetId())) {
      throw std::runtime_error("Failed to get required component " + std::string(typeid(T).name()) + " for entity " +
                               std::to_string(entity.id));
    }
    const auto componentArray = archetype->GetComponentArray<T>(chunkIndex, componentEntity.GetId());

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
    return it->second.archetype->HasComponent(componentEntity.GetId());
  }

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
  std::decay_t<Func>& RegisterSystem(Func&& func) {
    using StoredFunc = std::decay_t<Func>;
    class SystemWrapper final : public ISystem {
     public:
      SystemWrapper(Registry* registry, Func&& f)
          : func_(std::forward<Func>(f)), query_(registry->CreateQuery<TArgs...>()) {}

      void Update(const Registry& registry) override {
        query_->Update();
        // Pass by reference so captured state in the system lambda persists across invocations.
        query_->ForEach(func_);
      }

      StoredFunc& GetFunc() { return func_; }

     private:
      StoredFunc func_;
      std::unique_ptr<ComponentQuery<TArgs...>> query_;
    };

    auto wrapper = std::make_unique<SystemWrapper>(this, std::forward<Func>(func));
    StoredFunc& ref = wrapper->GetFunc();
    systems_.push_back(std::move(wrapper));
    return ref;
  }

  // Singleton components
  template <typename T>
  T& Set(T value) {
    const auto entityComponent = Component<T>();
    singleton_components_[entityComponent.GetId()] = std::move(value);
    return std::any_cast<T&>(singleton_components_.at(entityComponent.GetId()));
  }

  template <typename T>
  const T& Get() const {
    const auto entityComponent = Component<T>();
    try {
      return std::any_cast<const T&>(singleton_components_.at(entityComponent.GetId()));

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
  Archetype* GetOrCreateArchetype(std::vector<ComponentID> componentIDs, ComponentID newComponentId);

  std::unique_ptr<EntityManager> entity_manager_;
  std::unique_ptr<ComponentRegistry> component_registry_;
  std::unordered_map<EntityID, EntityLocation> entity_locations_;
  std::unordered_map<ArchetypeID, std::unique_ptr<Archetype>> archetypes_;
  std::unique_ptr<Archetype> root_archetype_;  // The root of the archetype graph. Empty signature.
  std::vector<std::unique_ptr<ISystem>> systems_;
  std::unordered_map<ComponentID, std::any> singleton_components_;
  std::unordered_map<std::type_index, Entity> component_to_entity_;
  // Per-component-id list of archetypes containing it. Authoritative source for query lookup.
  std::unordered_map<ComponentID, ArchetypeList> component_index_;
  std::unordered_map<EntityID, std::unordered_set<EcsId>> pairs_;
  float delta_time_{};
};
