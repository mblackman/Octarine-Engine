#pragma once

#include <any>
#include <memory>
#include <stdexcept>
#include <string>

#include "ArchetypeQuery.h"
#include "Component.h"
#include "Entity.h"
#include "General/Logger.h"
#include "Iterable.h"
#include "System.h"

class Query {
 public:
  Query() = default;
  virtual ~Query() = default;

  Query(Query&&) = default;
  Query(Query& query) = default;

  Query& operator=(Query&&) = default;
  Query& operator=(const Query& query) = default;

  virtual void Update() = 0;
};

template <typename... TComponents>
class ComponentQuery;

class Registry {
 public:
  template <typename... TComponents>
  friend class ComponentQuery;

  Registry() {
    entity_manager_ = std::make_unique<EntityManager>();
    component_registry_ = std::make_unique<ComponentRegistry>();
  }

  void Update(float deltaTime);

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
  T& GetComponent(const Entity entity) const {
    const auto it = entity_locations_.find(entity.id);
    if (it == entity_locations_.end()) {
      throw std::runtime_error("Failed to get required component " + std::string(typeid(T).name()) + " for entity " +
                               std::to_string(entity.id));
    }
    const auto componentId = GetComponentTypeID<T>();
    const auto [archetype, chunkIndex, indexInChunk] = it->second;

    if (!archetype->GetSignature().test(componentId)) {
      throw std::runtime_error("Failed to get required component " + std::string(typeid(T).name()) + " for entity " +
                               std::to_string(entity.id));
    }
    const auto componentArray = archetype->GetComponentArray<T>(chunkIndex);

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
      newQuery->Update();
      it = queries_.emplace(signature, std::move(newQuery)).first;
    }
    Query* base_ptr = it->second.get();
    return *static_cast<ComponentQuery<TComponents...>*>(base_ptr);
  }

  void UpdateQueries();

  // System Management
  template <typename... TArgs, typename Func>
  void RegisterSystem(Func&& func) {
    class SystemWrapper final : public ISystem {
     public:
      explicit SystemWrapper(Registry* registry, Func&& f)
          : func_(std::forward<Func>(f)), query_(registry->CreateQuery<TArgs...>()) {}

      void Update(const Registry& registry) override { query_.ForEach(this->func_); }

     private:
      Func func_;
      ComponentQuery<TArgs...>& query_;
    };

    systems_.push_back(std::make_unique<SystemWrapper>(this, std::forward<Func>(func)));
  }

  // Misc
  template <typename T>
  T& Set(T value) {
    const auto id = GetComponentTypeID<T>();
    singleton_components_[id] = std::move(value);
    return std::any_cast<T&>(singleton_components_.at(id));
  }

  template <typename T>
  const T& Get() const {
    const auto id = GetComponentTypeID<T>();
    try {
      return std::any_cast<const T&>(singleton_components_.at(id));

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

 private:
  Archetype* FindOrCreateArchetype(Signature signature);

  std::unique_ptr<EntityManager> entity_manager_;
  std::unique_ptr<ComponentRegistry> component_registry_;
  std::unordered_map<EntityID, EntityLocation> entity_locations_;
  std::unordered_map<Signature, std::unique_ptr<Archetype>> archetypes_;
  Archetype* root_archetype_ = nullptr;  // The root of the archetype graph. Empty signature.
  std::unordered_map<Signature, std::unique_ptr<Query>> queries_;
  std::vector<std::unique_ptr<ISystem>> systems_;
  std::unordered_map<ComponentTypeID, std::any> singleton_components_;
  float delta_time_{};
};

template <typename... TComponents>
class ComponentQuery final : public Query {
 public:
  explicit ComponentQuery(Registry* registry)
      : registry_(registry),
        signature_(GetSignature<TComponents...>()),
        archetype_query_(registry_->GetMatchingArchetypes(signature_)) {}

  void Update() override {
    archetype_query_ = ArchetypeQuery<TComponents...>(registry_->GetMatchingArchetypes(signature_));
  }

  template <typename Func>
  void ForEach(Func func) {
    for (const auto iterable = CreateIterable(); auto&& context : iterable) {
      if constexpr (std::is_invocable_v<Func, ContextFacade&, TComponents&...>) {
        func(context, context.Component<TComponents>()...);
      } else if constexpr (std::is_invocable_v<Func, Entity, TComponents&...>) {
        func(context.Entity(), context.Entity(), context.Component<TComponents>()...);
      } else if constexpr (std::is_invocable_v<Func, TComponents&...>) {
        func(context.Component<TComponents>()...);
      } else {
        static_assert(!std::is_same_v<Func, Func>,
                      "The function passed to ForEach does not match the required signatures. "
                      "Expected one of: void(Iter&, T&...), void(Entity, T&...), or void(T&...).");
      }
    }
  }

  template <typename Func>
  void Iterate(Func&& func) {
    func(CreateIterable());
  }

 private:
  Iterable CreateIterable() {
    auto beginFunc = [&]() {
      return AnyIterator(std::make_unique<Internal::IteratorImpl<TComponents...>>(archetype_query_.begin(), registry_,
                                                                                  registry_->delta_time_));
    };
    auto endFunc = [&]() {
      return AnyIterator(std::make_unique<Internal::IteratorImpl<TComponents...>>(archetype_query_.end(), registry_,
                                                                                  registry_->delta_time_));
    };

    return Iterable(beginFunc, endFunc);
  }

  Registry* registry_;
  Signature signature_;
  ArchetypeQuery<TComponents...> archetype_query_;
};
