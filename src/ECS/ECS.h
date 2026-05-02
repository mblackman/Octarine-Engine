#pragma once

#include <bitset>
#include <deque>
#include <memory>
#include <set>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "../General/Logger.h"
#include "../General/Pool.h"

const unsigned int kMaxComponents = 32;

// This used to track which components are present in an entity and which
// entities a system is interested in.
typedef std::bitset<kMaxComponents> Signature;

struct IComponent {
 protected:
  static int next_id_;
};

template <typename T>
class Component : public IComponent {
 public:
  static int GetId() {
    static auto id = next_id_++;
    return id;
  }
};

class Entity {
  int id_;
  class Registry* registry_;

 public:
  Entity(const int id, Registry* registry) : id_(id), registry_(registry) {}

  [[nodiscard]] int GetId() const;

  bool operator==(const Entity& other) const { return id_ == other.id_; }

  bool operator!=(const Entity& other) const { return id_ != other.id_; }

  bool operator<(const Entity& other) const { return id_ < other.id_; }

  bool operator>(const Entity& other) const { return id_ > other.id_; }

  bool operator<=(const Entity& other) const { return id_ <= other.id_; }

  bool operator>=(const Entity& other) const { return id_ >= other.id_; }

  template <class ComponentTypeToPass>
  void AddComponent(ComponentTypeToPass&& component);

  template <typename T, typename... TArgs>
  void AddComponent(TArgs&&... args);

  template <typename T>
  void RemoveComponent() const;

  template <typename T>
  [[nodiscard]] bool HasComponent() const;

  template <typename T>
  T& GetComponent() const;

  void Tag(const std::string& tag);
  [[nodiscard]] bool HasTag(const std::string& tag) const;
  void Group(const std::string& group);
  [[nodiscard]] bool InGroup(const std::string& group) const;

  void Blam();
};

class System {
  Signature component_signature_;
  std::vector<Entity> entities_;

 public:
  System() = default;

  System(const System&) = delete;
  System& operator=(const System&) = delete;

  System(System&&) = delete;
  System& operator=(System&&) = delete;

  ~System() = default;

  [[nodiscard]] const Signature& GetComponentSignature() const { return component_signature_; }

  [[nodiscard]] std::vector<Entity> GetEntities() const { return entities_; }

  void AddEntity(Entity entity);
  void RemoveEntity(Entity entity);

  template <typename T>
  void RequireComponent();
};

/**
 * Manages the creation and destruction of entities, systems, and components.
 */
class Registry {
  int num_entities_{};
  std::vector<Entity> entities_;

  std::set<Entity> entities_to_add_;
  std::set<Entity> entities_to_remove_;

  // Keeps track of the tags for each entity in both directions.
  std::unordered_map<std::string, Entity> entity_by_tag_;
  std::unordered_map<int, std::string> tag_by_entity_;

  // Keeps track of the groups for each entity in both directions.
  std::unordered_map<std::string, std::set<Entity>> entities_by_groups_;
  std::unordered_map<int, std::set<std::string>> groups_by_entity_;

  // Each pool at an index corresponds to a component type.
  // [Pool index = entity id]
  std::vector<std::shared_ptr<IPool>> component_pools_;

  // Component signatures are used to track which components are present in
  // an entity and which entities a system is interested in.
  std::vector<Signature> entity_component_signatures_;

  // A map of the systems that are registered with the registry.
  std::unordered_map<std::type_index, std::shared_ptr<System>> systems_;

  // A queue of ids that have been freed from destroyed entities.
  std::deque<int> free_ids_;

 public:
  Registry() = default;

  Registry(const Registry&) = delete;
  Registry& operator=(const Registry&) = delete;

  Registry(Registry&&) = delete;
  Registry& operator=(Registry&&) = delete;

  ~Registry() = default;

  void Update();

  // Entity management
  Entity CreateEntity();

  void BlamEntity(Entity entity);

  // Tag management
  void TagEntity(Entity entity, const std::string& tag);
  bool EntityHasTag(Entity entity, const std::string& tag) const;
  Entity GetEntityByTag(const std::string& tag) const;
  void RemoveEntityTag(Entity entity);

  // Group management
  void GroupEntity(Entity entity, const std::string& group);
  bool EntityInGroup(Entity entity, const std::string& group) const;
  std::vector<Entity> GetEntitiesByGroup(const std::string& group) const;
  void RemoveEntityGroup(Entity entity, const std::string& group);
  void RemoveEntityGroups(Entity entity);

  // System management
  template <typename T, typename... TArgs>
  void AddSystem(TArgs&&... args);

  template <typename T>
  void RemoveSystem();

  template <typename T>
  bool HasSystem() const;

  template <typename T>
  T& GetSystem() const;

  void AddEntityToSystems(Entity entity);

  void RemoveEntityFromSystems(Entity entity);

  // Component management
  template <typename ComponentArg>
  void AddComponent(Entity entity, ComponentArg&& component);

  template <typename T, typename... TArgs>
  void AddComponent(Entity entity, TArgs&&... args);

  template <typename T>
  void RemoveComponent(Entity entity);

  template <typename T>
  bool HasComponent(Entity entity) const;

  template <typename T>
  T& GetComponent(Entity entity) const;
};

// Entity implementations
template <typename ComponentTypeToPass>
void Entity::AddComponent(ComponentTypeToPass&& component) {
  using ActualComponentType = std::decay_t<ComponentTypeToPass>;
  registry_->AddComponent<ActualComponentType>(*this, std::forward<ComponentTypeToPass>(component));
}

template <typename T, typename... TArgs>
void Entity::AddComponent(TArgs&&... args) {
  registry_->AddComponent<T>(*this, std::forward<TArgs>(args)...);
}

template <typename T>
void Entity::RemoveComponent() const {
  registry_->RemoveComponent<T>(*this);
}

template <typename T>
bool Entity::HasComponent() const {
  return registry_->HasComponent<T>(*this);
}

template <typename T>
T& Entity::GetComponent() const {
  return registry_->GetComponent<T>(*this);
}

// System Implementations
template <typename T>
void System::RequireComponent() {
  const auto componentId = Component<T>::GetId();
  component_signature_.set(componentId);
}

// Registry implementations
template <typename T, typename... TArgs>
void Registry::AddSystem(TArgs&&... args) {
  auto newSystem = std::make_shared<T>(std::forward<TArgs>(args)...);
  systems_.insert(std::make_pair(std::type_index(typeid(T)), newSystem));
}

template <typename T>
void Registry::RemoveSystem() {
  const auto it = systems_.find(std::type_index(typeid(T)));

  if (it != systems_.end()) {
    systems_.erase(it);
  }
}

template <typename T>
bool Registry::HasSystem() const {
  return systems_.find(std::type_index(typeid(T))) != systems_.end();
}

template <typename T>
T& Registry::GetSystem() const {
  const auto it = systems_.find(std::type_index(typeid(T)));
  return *std::static_pointer_cast<T>(it->second);
}

template <typename ComponentArg>
void Registry::AddComponent(const Entity entity, ComponentArg&& component) {
  using ActualComponentType = std::decay_t<ComponentArg>;

  const auto componentId = Component<ActualComponentType>::GetId();
  const auto entityId = entity.GetId();

  if (componentId >= static_cast<int>(component_pools_.size())) {
    component_pools_.resize(componentId + 1, nullptr);
  }

  if (!component_pools_[componentId]) {
    auto newComponentPool = std::make_shared<Pool<ActualComponentType>>();
    component_pools_[componentId] = newComponentPool;
  }

  auto componentPool = std::static_pointer_cast<Pool<ActualComponentType>>(component_pools_[componentId]);

  componentPool->Set(entityId, component);

  entity_component_signatures_[entityId].set(componentId);

  Logger::Info("Added component: " + std::to_string(componentId) + " to entity: " + std::to_string(entityId));
}

template <typename T, typename... TArgs>
void Registry::AddComponent(const Entity entity, TArgs&&... args) {
  T newComponent(std::forward<TArgs>(args)...);
  AddComponent(entity, std::move(newComponent));
}

template <typename T>
void Registry::RemoveComponent(const Entity entity) {
  const auto componentId = Component<T>::GetId();
  const auto entityId = entity.GetId();

  entity_component_signatures_[entityId].set(componentId, false);

  auto componentPool = std::static_pointer_cast<Pool<T>>(component_pools_[componentId]);
  componentPool->Remove(entityId);

  Logger::Info("Removed component: " + std::to_string(entityId) + " from entity: " + std::to_string(entityId));
}

template <typename T>
bool Registry::HasComponent(const Entity entity) const {
  return entity_component_signatures_[entity.GetId()].test(Component<T>::GetId());
}

template <typename T>
T& Registry::GetComponent(const Entity entity) const {
  const auto componentId = Component<T>::GetId();
  const auto entityId = entity.GetId();

  if (!component_pools_[componentId]) {
    throw std::runtime_error("Component pool not found for component: " + std::to_string(componentId));
  }

  auto componentPool = std::static_pointer_cast<Pool<T>>(component_pools_[componentId]);

  return componentPool->Get(entityId);
}