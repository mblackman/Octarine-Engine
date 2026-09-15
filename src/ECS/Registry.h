#pragma once

#include <any>
#include <array>
#include <atomic>
#include <cstring>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "Component.h"
#include "Context.h"
#include "Entity.h"
#include "General/Logger.h"
#include "System.h"

class Query;

template <typename... TComponents>
class ComponentQuery;

class Iterable;

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
    if constexpr (std::is_swappable_v<T>) {
      info.swap = [](void* a, void* b) {
        using std::swap;
        swap(*static_cast<T*>(a), *static_cast<T*>(b));
      };
    }
    auto [it, inserted] = component_infos_.try_emplace(id, std::move(info));
    if (!inserted && it->second.name != typeid(T).name()) {
      throw std::runtime_error("Component ID collision: " + it->second.name + " and " + typeid(T).name());
    }
  }

  // Registers a zero-sized tag component.
  void RegisterTag(const ComponentID id, std::string name) {
    ComponentInfo info{id, name, 0, 1};
    info.move_construct = [](void*, void*) {};
    info.destroy = [](void*) {};
    info.swap = [](void*, void*) {};
    auto [it, inserted] = component_infos_.try_emplace(id, std::move(info));
    if (!inserted && it->second.name != name) {
      throw std::runtime_error("Tag ID collision: " + it->second.name + " and " + name);
    }
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

  Entity CreateEntity();

  // Creates an entity populated with components in a single archetype transition.
  template <typename... TComponents>
  Entity CreateEntityWithBundle(TComponents... components) {
    static_assert(sizeof...(TComponents) > 0, "CreateEntityWithBundle requires at least one component");
    std::array<Entity, sizeof...(TComponents)> componentEntities{Component<std::decay_t<TComponents>>()...};
    std::vector<ComponentID> ids;
    ids.reserve(sizeof...(TComponents));
    for (const auto& e : componentEntities) ids.push_back(e.GetId());

    Archetype* archetype = GetOrCreateArchetypeFromSet(ids);

    const Entity entity = entity_manager_->CreateEntity();
    const auto location = archetype->AddEntity(entity);
    const std::uint32_t id = entity.GetId();
    if (id >= entity_locations_.size()) {
      entity_locations_.resize(id + 1, EntityLocation{nullptr, 0, 0});
    }
    entity_locations_[id] = location;
    ++user_entity_count_;

    PlaceBundle(archetype, location, componentEntities, std::forward_as_tuple(components...),
                std::index_sequence_for<TComponents...>{});
    PromoteToActive(location);
    return entity;
  }

  void BlamEntity(Entity entity);

  [[nodiscard]] std::uint64_t GetUserEntityCount() const { return user_entity_count_; }

  // Defers destruction until the end of Update; safe to call during system iteration.
  void QueueBlamEntity(const Entity entity) {
    if (pending_blam_ids_.insert(entity.id).second) {
      pending_blams_.push_back(entity);
    }
  }

  // Defers despawn until the end of Update.
  void QueueDespawnEntity(const Entity entity) {
    if (pending_despawn_ids_.insert(entity.id).second) {
      pending_despawns_.push_back(entity);
    }
  }

  [[nodiscard]] bool IsAlive(const Entity entity) const {
    const std::uint32_t id = entity.GetId();
    if (id >= entity_locations_.size() || !entity_manager_->IsValid(entity)) return false;
    return entity_locations_[id].archetype != nullptr;
  }

  // Resolves an entity to its chunk slot. Returns null archetype if invalid.
  [[nodiscard]] EntityLocation GetEntityLocation(const Entity entity) const {
    const std::uint32_t id = entity.GetId();
    if (id >= entity_locations_.size()) return EntityLocation{nullptr, 0, 0};
    return entity_locations_[id];
  }

  void ClearUserEntities();

  [[nodiscard]] std::uint64_t GetEntityCount() const { return entity_locations_.size(); }

  [[nodiscard]] std::vector<Entity> GetUserEntities() const;

  // Returns the entity's current archetype ID, including active tags.
  [[nodiscard]] ArchetypeID GetArchetypeID(const Entity entity) const {
    const std::uint32_t id = entity.GetId();
    if (id >= entity_locations_.size() || !entity_manager_->IsValid(entity) || !entity_locations_[id].archetype) {
      throw std::runtime_error("GetArchetypeID called on a destroyed, stale, or never-allocated entity.");
    }
    return entity_locations_[id].archetype->GetID();
  }

  // Moves an entity between the active and inactive partitions of its chunk.
  void Activate(Entity entity);
  void Deactivate(Entity entity);

  [[nodiscard]] bool IsActive(Entity entity) const;

  // Component management
  template <typename T>
  Entity Component() {
    std::type_index type_idx(typeid(T));
    if (auto it = type_to_entity_.find(type_idx); it != type_to_entity_.end()) {
      return it->second;
    }
    const auto entity = CreateInternalEntity();
    type_to_entity_[type_idx] = entity;
    component_registry_->RegisterComponent<T>(entity);
    return entity;
  }

  template <typename T>
  Entity Component() const {
    std::type_index type_idx(typeid(T));
    if (auto it = type_to_entity_.find(type_idx); it != type_to_entity_.end()) {
      return it->second;
    }
    throw std::runtime_error("Component type " + std::string(typeid(T).name()) +
                             " has not been used yet. Cannot get component.");
  }

  // Non-throwing equivalent of Component<T>() const. Returns nullopt when the type was never registered.
  template <typename T>
  [[nodiscard]] std::optional<Entity> TryComponent() const {
    std::type_index type_idx(typeid(T));
    if (auto it = type_to_entity_.find(type_idx); it != type_to_entity_.end()) {
      return it->second;
    }
    return std::nullopt;
  }

  template <typename T>
  void AddComponent(const Entity entity, T component) {
    const Entity componentEntity = Component<T>();
    const std::uint32_t id = entity.GetId();
    // Re-add path: archetype already holds T. Assign over the live slot so the previous
    // value's destructor runs (placement-new would leak std::string / sol::table refs).
    if (id < entity_locations_.size() && entity_locations_[id].archetype != nullptr &&
        entity_manager_->IsValid(entity) && entity_locations_[id].archetype->HasComponent(componentEntity.GetId())) {
      GetComponent<T>(entity) = std::move(component);
      return;
    }
    const EntityLocation newLocation = TransitionAddComponent(entity, componentEntity.GetId());
    if (newLocation.archetype == nullptr) return;
    newLocation.archetype->AddComponent(newLocation, componentEntity, component);
  }

  template <typename T>
  void RemoveComponent(const Entity entity) {
    std::type_index type_idx(typeid(T));
    if (auto it = type_to_entity_.find(type_idx); it != type_to_entity_.end()) {
      TransitionRemoveComponent(entity, it->second.GetId());
    }
  }

  template <typename T>
  T& GetComponent(const Entity entity) const {
    const std::uint32_t id = entity.GetId();
    if (id >= entity_locations_.size() || !entity_manager_->IsValid(entity) || !entity_locations_[id].archetype) {
      throw std::runtime_error("Failed to get required component " + std::string(typeid(T).name()) + " for entity " +
                               std::to_string(entity.id));
    }
    const auto componentEntity = Component<T>();
    const auto [archetype, chunkIndex, indexInChunk] = entity_locations_[id];

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
    const std::uint32_t id = entity.GetId();
    if (id >= entity_locations_.size() || !entity_manager_->IsValid(entity) || !entity_locations_[id].archetype) {
      return false;
    }
    const auto componentEntity = TryComponent<T>();
    if (!componentEntity.has_value()) return false;
    return entity_locations_[id].archetype->HasComponent(componentEntity->GetId());
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
  SystemHandle<std::decay_t<Func>> RegisterSystem(Func&& func) {
    using StoredFunc = std::decay_t<Func>;
    class SystemWrapper final : public ISystem {
     public:
      SystemWrapper(Registry* registry, Func&& f)
          : ISystem(PrettifyTypeName(typeid(StoredFunc).name())),
            registry_(registry),
            func_(std::forward<Func>(f)),
            query_(registry->CreateQuery<TArgs...>()) {}

      void Update(const Registry& /*registry*/) override {
        query_->Update();
        // Pass by reference so captured state in the system lambda persists across invocations.
        query_->ForEach(func_);

        if constexpr (requires { func_.GetCommandBuffer(); }) {
          func_.GetCommandBuffer().Playback(registry_);
        }
      }

      StoredFunc& GetFunc() { return func_; }

     private:
      Registry* registry_;
      StoredFunc func_;
      std::unique_ptr<ComponentQuery<TArgs...>> query_;
    };

    auto wrapper = std::make_unique<SystemWrapper>(this, std::forward<Func>(func));
    StoredFunc& ref = wrapper->GetFunc();
    const SystemId id = systems_.size();
    systems_.push_back(std::move(wrapper));
    return SystemHandle<StoredFunc>(id, &ref);
  }

  // Registers a system running per-entity callbacks in parallel across chunks on ThreadPool.
  template <typename... TArgs, typename Func>
  // NOLINTNEXTLINE(readability-function-cognitive-complexity)
  SystemHandle<std::decay_t<Func>> RegisterParallelSystem(Func&& func) {
    using StoredFunc = std::decay_t<Func>;
    static_assert(std::is_invocable_v<StoredFunc, Entity, float, TArgs&...> ||
                      std::is_invocable_v<StoredFunc, Entity, TArgs&...> ||
                      std::is_invocable_v<StoredFunc, float, TArgs&...> || std::is_invocable_v<StoredFunc, TArgs&...>,
                  "RegisterParallelSystem func must match void(Entity, float, TArgs&...), void(Entity, TArgs&...), "
                  "void(float, TArgs&...), or void(TArgs&...). "
                  "ContextFacade signatures are not supported on the parallel path.");
    class ParallelSystemWrapper final : public ISystem {
     public:
      ParallelSystemWrapper(Registry* registry, Func&& f)
          : ISystem(PrettifyTypeName(typeid(StoredFunc).name())),
            registry_(registry),
            func_(std::forward<Func>(f)),
            query_(registry->CreateQuery<TArgs...>()) {}

      void Update(const Registry& registry) override {
        query_->Update();
        const float dt = registry.delta_time_;

        if constexpr (requires { func_.Prepare(registry_); }) {
          func_.Prepare(registry_);
        }

        if constexpr (std::is_invocable_v<StoredFunc, Entity, float, TArgs&...> ||
                      std::is_invocable_v<StoredFunc, Entity, TArgs&...>) {
          query_->ParallelForEach([this, dt](Entity entity, TArgs&... args) {
            (void)dt;
            if constexpr (std::is_invocable_v<StoredFunc, Entity, float, TArgs&...>) {
              func_(entity, dt, args...);
            } else if constexpr (std::is_invocable_v<StoredFunc, Entity, TArgs&...>) {
              func_(entity, args...);
            } else {
              static_assert(!std::is_same_v<Func, Func>,
                            "The function passed to ForEach does not match the required signatures. "
                            "Expected one of: void(Entity, T&...), void(Entity, float, T&...).");
            }
          });
        } else if constexpr (std::is_invocable_v<StoredFunc, float, TArgs&...> ||
                             std::is_invocable_v<StoredFunc, TArgs&...>) {
          query_->ParallelForEach([this, dt](TArgs&... args) {
            (void)dt;
            if constexpr (std::is_invocable_v<StoredFunc, float, TArgs&...>) {
              func_(dt, args...);
            } else if constexpr (std::is_invocable_v<StoredFunc, TArgs&...>) {
              func_(args...);
            } else {
              static_assert(!std::is_same_v<Func, Func>,
                            "The function passed to ForEach does not match the required signatures. "
                            "Expected one of: void(float, T&...), void(T&...).");
            }
          });
        } else {
          static_assert(
              !std::is_same_v<Func, Func>,
              "The function passed to ForEach does not match the required signatures. "
              "Expected one of: void(Entity, T&...), void(Entity, float, T&...), void(float, T&...), void(T&...).");
        }

        if constexpr (requires { func_.GetCommandBuffer(); }) {
          func_.GetCommandBuffer().Playback(registry_);
        }
      }

      StoredFunc& GetFunc() { return func_; }

     private:
      Registry* registry_;
      StoredFunc func_;
      std::unique_ptr<ComponentQuery<TArgs...>> query_;
    };

    auto wrapper = std::make_unique<ParallelSystemWrapper>(this, std::forward<Func>(func));
    StoredFunc& ref = wrapper->GetFunc();
    const SystemId id = systems_.size();
    systems_.push_back(std::move(wrapper));
    return SystemHandle<StoredFunc>(id, &ref);
  }

  // Registers a system invoked once per Update with matching Iterable.
  template <typename... TArgs, typename Func>
  SystemHandle<std::decay_t<Func>> RegisterBulkSystem(Func&& func) {
    using StoredFunc = std::decay_t<Func>;
    class BulkSystemWrapper final : public ISystem {
     public:
      BulkSystemWrapper(Registry* registry, Func&& f)
          : ISystem(PrettifyTypeName(typeid(StoredFunc).name())),
            func_(std::forward<Func>(f)),
            query_(registry->CreateQuery<TArgs...>()) {}

      void Update(const Registry& registry) override {
        query_->Update();
        auto* registryPtr = const_cast<Registry*>(&registry);
        const float dt = registry.delta_time_;
        query_->Iterate([this, registryPtr, dt](const Iterable& iter) {
          Internal::BulkContextImpl bulkCtx(registryPtr, dt);
          ContextFacade ctx(&bulkCtx);
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
    const SystemId id = systems_.size();
    systems_.push_back(std::move(wrapper));
    return SystemHandle<StoredFunc>(id, &ref);
  }

  // Builds execution-order constraints between registered systems:
  //   registry.Order(collision).After(transform);
  //   registry.Order(producer).Before(consumer);
  class OrderBuilder {
   public:
    OrderBuilder(Registry* registry, const SystemId id) : registry_(registry), id_(id) {}

    template <typename T>
    OrderBuilder& After(const SystemHandle<T>& other) {
      registry_->AddOrderEdge(other.Id(), id_);
      return *this;
    }

    template <typename T>
    OrderBuilder& Before(const SystemHandle<T>& other) {
      registry_->AddOrderEdge(id_, other.Id());
      return *this;
    }

   private:
    Registry* registry_;
    SystemId id_;
  };

  template <typename T>
  OrderBuilder Order(const SystemHandle<T>& handle) {
    return OrderBuilder(this, handle.Id());
  }

  // Stores singleton service wrapped in shared_ptr to support move-only types.
  template <typename T>
  T& Set(T value) {
    auto ptr = std::make_shared<T>(std::move(value));
    T& ref = *ptr;
    singleton_components_[typeid(T).name()] = std::move(ptr);
    return ref;
  }

  template <typename T>
  const T& Get() const {
    try {
      const auto& anyVal = singleton_components_.at(typeid(T).name());
      const auto& ptr = std::any_cast<const std::shared_ptr<T>&>(anyVal);
      return *ptr;
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

  // Returns nullptr if T has never been Set.
  template <typename T>
  [[nodiscard]] T* TryGet() {
    const auto it = singleton_components_.find(typeid(T).name());
    if (it == singleton_components_.end()) return nullptr;
    if (const auto* ptr = std::any_cast<std::shared_ptr<T>>(&it->second)) return ptr->get();
    return nullptr;
  }

  // Resolves or creates a named zero-size tag component.
  Entity TagId(const std::string& name) {
    if (const auto it = tag_to_entity_.find(name); it != tag_to_entity_.end()) {
      return it->second;
    }
    const Entity tagEntity = CreateInternalEntity();
    component_registry_->RegisterTag(tagEntity.GetId(), name);
    tag_to_entity_.emplace(name, tagEntity);
    return tagEntity;
  }

  // Resolves or creates a compile-time typed tag component.
  template <typename T>
  Entity Tag() {
    static_assert(std::is_empty_v<T>, "Tag<T>() requires an empty struct type");
    std::type_index type_idx(typeid(T));
    if (auto it = type_to_entity_.find(type_idx); it != type_to_entity_.end()) {
      return it->second;
    }
    const auto entity = CreateInternalEntity();
    type_to_entity_[type_idx] = entity;
    component_registry_->RegisterTag(entity.GetId(), typeid(T).name());
    return entity;
  }

  void AddTag(const Entity entity, const Entity tagEntity) { TransitionAddComponent(entity, tagEntity.GetId()); }

  void AddTag(const Entity entity, const std::string& name) { AddTag(entity, TagId(name)); }

  template <typename T>
  void AddTag(const Entity entity) {
    TransitionAddComponent(entity, Tag<T>().GetId());
  }

  void RemoveTag(const Entity entity, const Entity tagEntity) { TransitionRemoveComponent(entity, tagEntity.GetId()); }

  void RemoveTag(const Entity entity, const std::string& name) {
    const auto it = tag_to_entity_.find(name);
    if (it == tag_to_entity_.end()) return;
    RemoveTag(entity, it->second);
  }

  template <typename T>
  void RemoveTag(const Entity entity) {
    std::type_index type_idx(typeid(T));
    if (auto it = type_to_entity_.find(type_idx); it != type_to_entity_.end()) {
      TransitionRemoveComponent(entity, it->second.GetId());
    }
  }

  [[nodiscard]] bool HasTag(const Entity entity, const Entity tagEntity) const {
    const std::uint32_t id = entity.GetId();
    if (id >= entity_locations_.size() || !entity_manager_->IsValid(entity) || !entity_locations_[id].archetype) {
      return false;
    }
    return entity_locations_[id].archetype->HasComponent(tagEntity.GetId());
  }

  [[nodiscard]] bool HasTag(const Entity entity, const std::string& name) const {
    const auto tagIt = tag_to_entity_.find(name);
    if (tagIt == tag_to_entity_.end()) return false;
    return HasTag(entity, tagIt->second);
  }

  template <typename T>
  [[nodiscard]] bool HasTag(const Entity entity) const {
    std::type_index type_idx(typeid(T));
    if (auto it = type_to_entity_.find(type_idx); it != type_to_entity_.end()) {
      return HasTag(entity, it->second);
    }
    return false;
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

  // Allocation-free traversal of a parent's children — the per-frame alternative to
  // GetChildren's vector-by-value. func receives each child as Entity.
  template <typename F>
  void ForEachChild(const Entity parent, F&& func) const {
    const auto it = parent_to_children_.find(parent.id);
    if (it == parent_to_children_.end()) return;
    for (const EntityID id : it->second) {
      func(Entity{id});
    }
  }

  // Visit every hierarchy root: an entity that has children but no parent of its own.
  // O(number of parents), independent of total entity count — lets TransformSystem seed its
  // descent without scanning the whole world for parentless entities.
  template <typename F>
  void ForEachHierarchyRoot(F&& func) const {
    for (const auto& [parentId, children] : parent_to_children_) {
      if (!child_to_parent_.contains(parentId)) {
        func(Entity{parentId});
      }
    }
  }

  [[nodiscard]] float DeltaTime() const { return delta_time_; }

  [[nodiscard]] bool HasAnyPairs() const { return !pairs_.empty(); }

  [[nodiscard]] bool HasAnyChildPairs() const { return !child_to_parent_.empty(); }

  // Incremented on hierarchy mutation to invalidate cached root sets.
  [[nodiscard]] uint64_t HierarchyGeneration() const { return hierarchy_generation_; }

  // Incremented when a new archetype is registered.
  [[nodiscard]] uint64_t ArchetypeGeneration() const { return archetype_generation_; }

  // Creation-ordered log of archetypes for incremental query matching.
  [[nodiscard]] const std::vector<Archetype*>& ArchetypeLog() const { return archetype_log_; }

  // True if archetype contains every component ID in type.
  [[nodiscard]] static bool MatchesType(const Archetype& archetype, const ArchetypeType& type);

 private:
  Entity CreateInternalEntity();

  EntityLocation TransitionAddComponent(Entity entity, ComponentID componentId);
  EntityLocation TransitionRemoveComponent(Entity entity, ComponentID componentId);
  void PromoteToActive(const EntityLocation& location);
  Archetype* GetOrCreateArchetype(std::vector<ComponentID> componentIDs, ComponentID newComponentId);
  Archetype* GetOrCreateArchetypeRemove(std::vector<ComponentID> componentIDs, ComponentID removeComponentId);
  Archetype* GetOrCreateArchetypeFromSet(std::vector<ComponentID> componentIDs);
  [[nodiscard]] Archetype* FindExactArchetype(const std::vector<ComponentID>& componentIDs) const;
  Archetype* RegisterNewArchetype(const std::vector<ComponentID>& componentIDs);

  void AddOrderEdge(SystemId before, SystemId after);
  void RebuildExecutionOrder();

  void FlushPendingDestruction();
  void FlushDespawns(const std::vector<Entity>& despawns);

  template <typename Tuple, size_t N, size_t... Is>
  void PlaceBundle(Archetype* archetype, const EntityLocation& location, const std::array<Entity, N>& componentEntities,
                   Tuple&& comps, std::index_sequence<Is...>) {
    (archetype->AddComponent(location, componentEntities[Is], std::get<Is>(comps)), ...);
  }

  std::unique_ptr<EntityManager> entity_manager_;
  std::unique_ptr<ComponentRegistry> component_registry_;
  std::vector<EntityLocation> entity_locations_;
  std::unordered_map<ArchetypeID, std::unique_ptr<Archetype>> archetypes_;
  std::unique_ptr<Archetype> root_archetype_;
  std::vector<std::unique_ptr<ISystem>> systems_;
  std::vector<std::pair<SystemId, SystemId>> system_order_edges_;
  std::vector<SystemId> system_execution_order_;
  bool system_order_dirty_ = false;
  // Keyed by type name rather than type_index to safely compare across translation units with hidden visibility.
  std::unordered_map<std::string_view, std::any> singleton_components_;
  std::unordered_map<std::type_index, Entity> type_to_entity_;
  std::unordered_map<std::string, Entity> tag_to_entity_;
  std::unordered_map<ComponentID, ArchetypeList> component_index_;
  std::vector<Archetype*> archetype_log_;
  std::unordered_map<EntityID, std::unordered_set<EcsId>> pairs_;
  std::unordered_map<std::uint32_t, std::unordered_set<EntityID>> target_to_pair_authors_;
  std::unordered_map<EntityID, std::unordered_set<EntityID>> parent_to_children_;
  std::unordered_map<EntityID, Entity> child_to_parent_;
  std::vector<Entity> pending_blams_;
  std::unordered_set<EcsId> pending_blam_ids_;
  std::vector<Entity> pending_despawns_;
  std::unordered_set<EcsId> pending_despawn_ids_;
  float delta_time_{};
  uint64_t archetype_generation_{0};
  uint64_t hierarchy_generation_{0};
  std::uint64_t user_entity_count_{0};
  std::unordered_set<EcsId> internal_entity_ids_;
};

template <typename T>
inline T& ContextFacade::Component() const {
  const auto componentEntity = impl_->GetRegistry()->Component<T>();
  void* ptr = impl_->GetComponentPtr(componentEntity.GetId());
  return *static_cast<T*>(ptr);
}
