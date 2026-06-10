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

  // Zero-size component used as a tag/label. Move/destroy/swap are no-ops so RemoveEntity,
  // CopyComponents, and Chunk::Swap can call them unconditionally.
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

  // Entity management
  Entity CreateEntity();

  // Create an entity already populated with a fixed bundle of components, landing in the
  // destination archetype in a single transition. Avoids the per-component archetype churn
  // that AddComponent does. Tags / pairs still need to be added separately.
  template <typename... TComponents>
  Entity CreateEntityWithBundle(TComponents... components) {
    static_assert(sizeof...(TComponents) > 0, "CreateEntityWithBundle requires at least one component");
    // Resolve component-type entities up front so each T is registered before archetype lookup.
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

    // Place each component in its archetype slot. Order in `componentEntities` mirrors the
    // template parameter pack — Archetype::AddComponent looks up the in-archetype index by id.
    PlaceBundle(archetype, location, componentEntities, std::forward_as_tuple(components...),
                std::index_sequence_for<TComponents...>{});
    // New entities land active. Promote the slot we just filled into the active prefix; if
    // the chunk had an inactive tail, this swaps the new entity past it.
    PromoteToActive(location);
    return entity;
  }

  void BlamEntity(Entity entity);

  // Count of user-visible entities (excludes internal component-type / tag entities).
  [[nodiscard]] std::uint64_t GetUserEntityCount() const { return user_entity_count_; }

  // Defer destroy until end of Registry::Update — safe to call during system iteration.
  // Deduplicates within a frame: same projectile hitting two targets only blams once.
  void QueueBlamEntity(const Entity entity) {
    if (pending_blam_ids_.insert(entity.id).second) {
      pending_blams_.push_back(entity);
    }
  }

  // Defer despawn until end of Registry::Update
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

  // Resolve an entity to its (archetype, chunk, index) slot. Returns a null-archetype location
  // when the entity is out of range or has no archetype assigned. Used by systems that need
  // direct chunk access (e.g. tree walks that can't piggyback on a query's per-chunk caching).
  [[nodiscard]] EntityLocation GetEntityLocation(const Entity entity) const {
    const std::uint32_t id = entity.GetId();
    if (id >= entity_locations_.size()) return EntityLocation{nullptr, 0, 0};
    return entity_locations_[id];
  }

  // Blams all user-visible entities (those not marked as internal).
  void ClearUserEntities();

  [[nodiscard]] std::uint64_t GetEntityCount() const { return entity_locations_.size(); }

  [[nodiscard]] std::vector<Entity> GetUserEntities() const;

  // Returns the entity's current archetype id, including any tags currently on it. Used by the
  // pool to bucket parked entities by shape so that Spawn pulls back like-shaped entities.
  [[nodiscard]] ArchetypeID GetArchetypeID(const Entity entity) const {
    const std::uint32_t id = entity.GetId();
    if (id >= entity_locations_.size() || !entity_manager_->IsValid(entity) || !entity_locations_[id].archetype) {
      throw std::runtime_error("GetArchetypeID called on a destroyed, stale, or never-allocated entity.");
    }
    return entity_locations_[id].archetype->GetID();
  }

  // Activate / Deactivate move an entity across the active/inactive partition of its archetype's
  // chunk without ever crossing archetypes. Default queries see only the active prefix, so a
  // deactivated entity becomes invisible to gameplay systems while its components stay in place
  // and ready to reuse. Used by EntityPoolManager for park/unpark.
  void Activate(Entity entity);
  void Deactivate(Entity entity);

  // True iff the entity is alive AND in the active prefix of its chunk.
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

  // Parallel per-entity system. Same shape as RegisterSystem but the per-entity callback runs
  // across chunks on the ThreadPool. Func signature: void (Entity, TComponents&...), void (Entity, float,
  // TComponents&...), void (TComponents&...) or void (float, TComponents&...) — the ContextFacade signatures supported
  // by RegisterSystem are not available here because the parallel path does not build a ContextImpl. Caller is
  // responsible for thread-safety: expose an EntityCommandBuffer on your system to handle requests for registry state
  // changes so they resolve on the calling thread all at once.
  template <typename... TArgs, typename Func>
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

  // Bulk system: wrapper invokes Func once per Update with the matching Iterable.
  // Func signature: void(const ContextFacade&, const Iterable&). The ContextFacade
  // exposes Registry/DeltaTime and a sentinel Entity (Entity{}); per-entity component
  // access goes through iterating the Iterable.
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

  // Execution-order constraints between registered systems, declared after registration:
  //   registry.Order(collision).After(transform);
  //   registry.Order(producer).Before(consumer);
  // Update topo-sorts (Kahn's algorithm) with registration order breaking ties, so systems
  // without constraints keep exact registration order. A constraint cycle throws
  // std::runtime_error naming the systems involved.
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

  // Singleton components. Stored as std::shared_ptr<T> inside std::any so that
  // move-only resource owners (e.g. AssetManager) can be stashed — std::any itself
  // requires CopyConstructible.
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

  // Non-throwing singleton lookup. Returns nullptr if T has never been Set.
  template <typename T>
  [[nodiscard]] T* TryGet() {
    const auto it = singleton_components_.find(typeid(T).name());
    if (it == singleton_components_.end()) return nullptr;
    if (const auto* ptr = std::any_cast<std::shared_ptr<T>>(&it->second)) return ptr->get();
    return nullptr;
  }

  // Tags / labels — zero-size component-entities. Each unique name maps to one Entity registered
  // as a zero-size component, so AddTag/HasTag/RemoveTag are archetype transitions / membership
  // checks. Lazy: TagId creates the Entity on first call.
  Entity TagId(const std::string& name) {
    if (const auto it = tag_to_entity_.find(name); it != tag_to_entity_.end()) {
      return it->second;
    }
    const Entity tagEntity = CreateInternalEntity();
    component_registry_->RegisterTag(tagEntity.GetId(), name);
    tag_to_entity_.emplace(name, tagEntity);
    return tagEntity;
  }

  // Typed tag — empty struct routed through fast_component_to_entity_, so resolution is one array
  // lookup instead of a hash-table find on a string. Storage is zero bytes per entity (registered
  // via RegisterTag). Use this for engine-internal tags; reserve string tags for data-driven ones.
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

  [[nodiscard]] float DeltaTime() const { return delta_time_; }

  // Returns true if any entity has relationship pairs (e.g. ChildOf hierarchy).
  // Used by TransformSystem to skip hierarchy resolution when no parents exist.
  [[nodiscard]] bool HasAnyPairs() const { return !pairs_.empty(); }

  // True iff at least one ChildOf relationship is live. Distinct from HasAnyPairs so the
  // TransformSystem fast path stays enabled when unrelated relationships exist.
  [[nodiscard]] bool HasAnyChildPairs() const { return !child_to_parent_.empty(); }

  // Bumped whenever the ChildOf hierarchy mutates (SetParent / detach / BlamEntity).
  // Lets TransformSystem cache root sets and rebuild only when membership changes.
  [[nodiscard]] uint64_t HierarchyGeneration() const { return hierarchy_generation_; }

  // Incremented when a new archetype is created. Queries use this to skip re-matching
  // when the archetype set hasn't changed since their last Update.
  [[nodiscard]] uint64_t ArchetypeGeneration() const { return archetype_generation_; }

 private:
  // Internal entities back component-type and tag registrations. They live in
  // entity_locations_ but should not show up in GetUserEntityCount.
  Entity CreateInternalEntity();

  EntityLocation TransitionAddComponent(Entity entity, ComponentID componentId);
  EntityLocation TransitionRemoveComponent(Entity entity, ComponentID componentId);
  // Move a freshly-added entity (whose components are placed but is sitting at entity_count - 1
  // in its chunk's inactive tail) into the active prefix. Idempotent for archetypes that have
  // no inactive tail — just bumps active_count.
  void PromoteToActive(const EntityLocation& location);
  Archetype* GetOrCreateArchetype(std::vector<ComponentID> componentIDs, ComponentID newComponentId);
  Archetype* GetOrCreateArchetypeRemove(std::vector<ComponentID> componentIDs, ComponentID removeComponentId);
  Archetype* GetOrCreateArchetypeFromSet(std::vector<ComponentID> componentIDs);

  // System-ordering graph (fed by Order().After()/.Before()). RebuildExecutionOrder runs Kahn's
  // algorithm over the edges; the ready set is kept ordered by SystemId so unconstrained systems
  // execute in registration order.
  void AddOrderEdge(SystemId before, SystemId after);
  void RebuildExecutionOrder();

  // Helper for CreateEntityWithBundle: index-pack expansion to dispatch each component to
  // Archetype::AddComponent at its corresponding location slot.
  template <typename Tuple, size_t N, size_t... Is>
  void PlaceBundle(Archetype* archetype, const EntityLocation& location, const std::array<Entity, N>& componentEntities,
                   Tuple&& comps, std::index_sequence<Is...>) {
    (archetype->AddComponent(location, componentEntities[Is], std::get<Is>(comps)), ...);
  }

  std::unique_ptr<EntityManager> entity_manager_;
  std::unique_ptr<ComponentRegistry> component_registry_;
  std::vector<EntityLocation> entity_locations_;
  std::unordered_map<ArchetypeID, std::unique_ptr<Archetype>> archetypes_;
  std::unique_ptr<Archetype> root_archetype_;  // The root of the archetype graph. Empty signature.
  std::vector<std::unique_ptr<ISystem>> systems_;
  // Ordering edges (before, after) between systems_ indices; consumed by RebuildExecutionOrder.
  std::vector<std::pair<SystemId, SystemId>> system_order_edges_;
  std::vector<SystemId> system_execution_order_;
  bool system_order_dirty_ = false;
  // Singleton storage keyed by typeid(T).name() (string_view into static storage). Was
  // ComponentID-keyed via Component<T>(), but on Android NDK with -fvisibility=hidden, opaque-type
  // pointers like MIX_Mixer* get TU-local typeinfo; std::type_index in type_to_entity_ produces
  // different keys per TU and Set/TryGet mismatch silently. typeid(T).name() returns the mangled
  // name, whose CONTENTS are identical across TUs even when the pointer addresses differ — so
  // a string_view comparison hashes/compares by contents and matches across TUs.
  std::unordered_map<std::string_view, std::any> singleton_components_;
  std::unordered_map<std::type_index, Entity> type_to_entity_;
  std::unordered_map<std::string, Entity> tag_to_entity_;
  // Per-component-id list of archetypes containing it. Authoritative source for query lookup.
  std::unordered_map<ComponentID, ArchetypeList> component_index_;
  std::unordered_map<EntityID, std::unordered_set<EcsId>> pairs_;
  // Reverse index for pairs: maps a target ID (32-bit) to all author EntityIDs (64-bit) pointing to it.
  std::unordered_map<std::uint32_t, std::unordered_set<EntityID>> target_to_pair_authors_;
  // Hierarchy authoritative storage. Keys/values are full EntityIDs (generation in high 32),
  // so recycled ids cannot alias old children — the legacy `pairs_` map drops generation.
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
