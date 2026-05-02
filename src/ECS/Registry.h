#pragma once

#include <any>
#include <cstring>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "Component.h"
#include "Entity.h"
#include "General/Logger.h"
#include "System.h"

class Query;

template <typename... TComponents>
class ComponentQuery;

class Iterable;
class ContextFacade;
namespace Internal {
class BulkContextImpl;
}

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

  // Zero-size component used as a tag/label. Move/destroy are no-ops so RemoveEntity and
  // CopyComponents can call them unconditionally.
  void RegisterTag(const ComponentID id, std::string name) {
    ComponentInfo info{id, std::move(name), 0, 1};
    info.move_construct = [](void*, void*) {};
    info.destroy = [](void*) {};
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

  // Defer destroy until end of Registry::Update — safe to call during system iteration.
  void QueueBlamEntity(const Entity entity) { pending_blams_.push_back(entity); }

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
    const Entity componentEntity = Component<T>();
    const EntityLocation newLocation = TransitionAddComponent(entity, componentEntity.GetId());
    newLocation.archetype->AddComponent(newLocation, componentEntity, component);
  }

  template <typename T>
  void RemoveComponent(const Entity entity) {
    const auto it = component_to_entity_.find(std::type_index(typeid(T)));
    if (it == component_to_entity_.end()) return;
    TransitionRemoveComponent(entity, it->second.GetId());
  }

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

  // Bulk system: wrapper invokes Func once per Update with the matching Iterable.
  // Func signature: void(const ContextFacade&, const Iterable&). The ContextFacade
  // exposes Registry/DeltaTime and a sentinel Entity (Entity{}); per-entity component
  // access goes through iterating the Iterable.
  template <typename... TArgs, typename Func>
  std::decay_t<Func>& RegisterBulkSystem(Func&& func) {
    using StoredFunc = std::decay_t<Func>;
    class BulkSystemWrapper final : public ISystem {
     public:
      BulkSystemWrapper(Registry* registry, Func&& f)
          : func_(std::forward<Func>(f)), query_(registry->CreateQuery<TArgs...>()) {}

      void Update(const Registry& registry) override {
        query_->Update();
        auto* registryPtr = const_cast<Registry*>(&registry);
        const float dt = registry.delta_time_;
        query_->Iterate([this, registryPtr, dt](const Iterable& iter) {
          ContextFacade ctx(std::make_unique<Internal::BulkContextImpl>(registryPtr, dt));
          func_(ctx, iter);
        });
      }

      StoredFunc& GetFunc() { return func_; }

     private:
      StoredFunc func_;
      std::unique_ptr<ComponentQuery<TArgs...>> query_;
    };

    auto wrapper = std::make_unique<BulkSystemWrapper>(this, std::forward<Func>(func));
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

    } catch (const std::out_of_range&) {
      throw std::runtime_error("Attempted to Get a singleton component that has not been Set.");
    } catch (const std::bad_any_cast&) {
      throw std::runtime_error("Type mismatch in Get. This indicates a logic error.");
    }
  }

  template <typename T>
  T& Get() {
    return const_cast<T&>(static_cast<const std::decay_t<decltype(*this)>&>(*this).Get<T>());
  }

  // Tags / labels — zero-size component-entities. Each unique name maps to one Entity registered
  // as a zero-size component, so AddTag/HasTag/RemoveTag are archetype transitions / membership
  // checks. Lazy: TagId creates the Entity on first call.
  Entity TagId(const std::string& name) {
    if (const auto it = tag_to_entity_.find(name); it != tag_to_entity_.end()) {
      return it->second;
    }
    const Entity tagEntity = CreateEntity();
    component_registry_->RegisterTag(tagEntity.GetId(), name);
    tag_to_entity_.emplace(name, tagEntity);
    return tagEntity;
  }

  void AddTag(const Entity entity, const Entity tagEntity) { TransitionAddComponent(entity, tagEntity.GetId()); }

  void AddTag(const Entity entity, const std::string& name) { AddTag(entity, TagId(name)); }

  void RemoveTag(const Entity entity, const Entity tagEntity) { TransitionRemoveComponent(entity, tagEntity.GetId()); }

  void RemoveTag(const Entity entity, const std::string& name) {
    const auto it = tag_to_entity_.find(name);
    if (it == tag_to_entity_.end()) return;
    RemoveTag(entity, it->second);
  }

  [[nodiscard]] bool HasTag(const Entity entity, const Entity tagEntity) const {
    const auto it = entity_locations_.find(entity.id);
    if (it == entity_locations_.end()) return false;
    return it->second.archetype->HasComponent(tagEntity.GetId());
  }

  [[nodiscard]] bool HasTag(const Entity entity, const std::string& name) const {
    const auto tagIt = tag_to_entity_.find(name);
    if (tagIt == tag_to_entity_.end()) return false;
    return HasTag(entity, tagIt->second);
  }

  // Relationships
  void AddPair(Entity entity, Entity relationship, Entity target);

  bool HasPair(Entity entity, Entity relationship, Entity target);

  // Hierarchy helpers — modeled as a Pair with the ChildOf relationship.
  struct ChildOfRelation {};

  Entity ChildOfEntity() { return Component<ChildOfRelation>(); }

  void SetParent(Entity child, Entity parent);

  [[nodiscard]] std::optional<Entity> GetParent(Entity child) const;

  [[nodiscard]] std::vector<Entity> GetChildren(Entity parent) const;

  [[nodiscard]] float DeltaTime() const { return delta_time_; }

 private:
  EntityLocation TransitionAddComponent(Entity entity, ComponentID componentId);
  EntityLocation TransitionRemoveComponent(Entity entity, ComponentID componentId);
  Archetype* GetOrCreateArchetype(std::vector<ComponentID> componentIDs, ComponentID newComponentId);
  Archetype* GetOrCreateArchetypeRemove(std::vector<ComponentID> componentIDs, ComponentID removeComponentId);

  std::unique_ptr<EntityManager> entity_manager_;
  std::unique_ptr<ComponentRegistry> component_registry_;
  std::unordered_map<EntityID, EntityLocation> entity_locations_;
  std::unordered_map<ArchetypeID, std::unique_ptr<Archetype>> archetypes_;
  std::unique_ptr<Archetype> root_archetype_;  // The root of the archetype graph. Empty signature.
  std::vector<std::unique_ptr<ISystem>> systems_;
  std::unordered_map<ComponentID, std::any> singleton_components_;
  std::unordered_map<std::type_index, Entity> component_to_entity_;
  std::unordered_map<std::string, Entity> tag_to_entity_;
  // Per-component-id list of archetypes containing it. Authoritative source for query lookup.
  std::unordered_map<ComponentID, ArchetypeList> component_index_;
  std::unordered_map<EntityID, std::unordered_set<EcsId>> pairs_;
  std::vector<Entity> pending_blams_;
  float delta_time_{};
};
