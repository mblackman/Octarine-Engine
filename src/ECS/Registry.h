#pragma once

#include "Component.h"
#include "Entity.h"
#include "Iter.h"
#include "System.h"

class Query {
 public:
  virtual ~Query() = default;
  virtual void Update() = 0;
};

template <typename... TComponents>
class ComponentQuery;

class Registry {
 public:
  Registry() {
    entity_manager_ = std::make_unique<EntityManager>();
    component_registry_ = std::make_unique<ComponentRegistry>();
  }

  void Update(float deltaTime) const;

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

      void Update(const Registry& registry, float deltaTime) override { query_.ForEach(this->func_, deltaTime); }

     private:
      Func func_;
      ComponentQuery<TArgs...>& query_;
    };

    systems_.push_back(std::make_unique<SystemWrapper>(this, std::forward<Func>(func)));
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
};

template <typename... TComponents>
class ComponentQuery final : public Query {
 public:
  explicit ComponentQuery(Registry* registry) : registry_(registry), signature_(GetSignature<TComponents...>()) {}

  void Update() override { cached_archetypes_ = registry_->GetMatchingArchetypes(signature_); }

  template <typename Func>
  void ForEach(Func func, const float deltaTime = 0) {
    if constexpr (std::is_invocable_v<Func, Iter&, TComponents&...>) {
      Iter iter{registry_, deltaTime};

      for (auto* archetype : cached_archetypes_) {
        archetype->ForEach<Func, TComponents...>(std::forward<Func>(func), iter);
      }

    } else if constexpr (std::is_invocable_v<Func, TComponents&...>) {
      for (auto* archetype : cached_archetypes_) {
        archetype->ForEach<Func, TComponents...>(std::forward<Func>(func));
      }

    } else {
      static_assert(!std::is_same_v<Func, Func>,
                    "The function passed to ForEach does not match the required signatures. "
                    "Expected either: void(Iter&, T*...) or void(Entity, T&...).");
    }
  }

 private:
  Registry* registry_;
  Signature signature_;
  std::vector<Archetype*> cached_archetypes_;
};
