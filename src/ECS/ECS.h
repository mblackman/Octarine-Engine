#pragma once

#include <bitset>
#include <deque>
#include <memory>
#include <optional>
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

  void Tag(const std::string& tag) const;
  [[nodiscard]] bool HasTag(const std::string& tag) const;
  void Group(const std::string& group) const;
  [[nodiscard]] bool InGroup(const std::string& group) const;

  void Blam() const;

  void AddParent(const Entity& parent) const;

  void RemoveParent() const;

  [[nodiscard]] std::optional<Entity> GetParent() const;

  [[nodiscard]] std::optional<std::vector<Entity>> GetChildren() const;
};

class System {
  Signature component_signature_;
  std::vector<Entity> entities_;
  std::vector<Entity> root_entities_;

 public:
  System() = default;

  System(const System&) = delete;
  System& operator=(const System&) = delete;

  System(System&&) = delete;
  System& operator=(System&&) = delete;

  ~System() = default;

  [[nodiscard]] const Signature& GetComponentSignature() const { return component_signature_; }

  [[nodiscard]] std::vector<Entity> GetEntities() const { return entities_; }

  [[nodiscard]] std::vector<Entity> GetRootEntities() const { return root_entities_; }

  void AddEntity(const Entity& entity);
  void RemoveEntity(const Entity& entity);

  void AddRootEntity(const Entity& entity);
  void RemoveRootEntity(const Entity& entity);

  template <typename T>
  void RequireComponent();
};

/**
 * Manages the creation and destruction of entities, systems, and components.
 */
class Registry {
  int num_entities_{};
  std::vector<Entity> entities_;
  // Entities without a parent
  std::vector<Entity> root_entities_;

  std::set<Entity> entities_to_add_;
  std::set<Entity> entities_to_remove_;
  std::vector<std::pair<Entity, Entity>> entity_child_to_add_parent_;
  std::set<Entity> entities_to_remove_parent_;

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

  // A map of parent-child relationships
  std::unordered_map<int, std::vector<Entity>> parent_to_children_;
  std::unordered_map<int, Entity> child_to_parent_map_;

  template <typename Func>
  void UpdateInterestedSystems(const Entity& entity, Func&& update) const;

  void AddEntityToSystems(const Entity& entity, bool isRoot) const;

  void RemoveEntityFromSystems(const Entity& entity) const;

  void AddRootEntityToSystems(const Entity& entity) const;

  void RemoveRootEntityFromSystems(const Entity& entity) const;

  void UpdateAddEntities();
  void UpdateProcessParentAdditions();
  void UpdateProcessParentRemovals();
  void UpdateProcessEntityRemovals();

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

  void BlamEntity(const Entity& entity);

  void SetParent(const Entity& parent, const Entity& child);

  void RemoveParent(const Entity& entity);

  std::optional<Entity> GetParent(const Entity& entity);

  std::optional<std::vector<Entity>> GetChildren(const Entity& entity);

  // Tag management
  void TagEntity(const Entity& entity, const std::string& tag);
  [[nodiscard]] bool EntityHasTag(const Entity& entity, const std::string& tag) const;
  [[nodiscard]] Entity GetEntityByTag(const std::string& tag) const;
  void RemoveEntityTag(const Entity& entity);

  // Group management
  void GroupEntity(const Entity& entity, const std::string& group);
  [[nodiscard]] bool EntityInGroup(const Entity& entity, const std::string& group) const;
  [[nodiscard]] std::vector<Entity> GetEntitiesByGroup(const std::string& group) const;
  void RemoveEntityGroup(const Entity& entity, const std::string& group);
  void RemoveEntityGroups(const Entity& entity);

  // System management
  template <typename T, typename... TArgs>
  void AddSystem(TArgs&&... args);

  template <typename T>
  void RemoveSystem();

  template <typename T>
  [[nodiscard]] bool HasSystem() const;

  template <typename T>
  T& GetSystem() const;

  // Component management
  template <typename ComponentArg>
  void AddComponent(const Entity& entity, ComponentArg&& component);

  template <typename T, typename... TArgs>
  void AddComponent(const Entity& entity, TArgs&&... args);

  template <typename T>
  void RemoveComponent(const Entity& entity);

  template <typename T>
  [[nodiscard]] bool HasComponent(const Entity& entity) const;

  template <typename T>
  T& GetComponent(const Entity& entity) const;
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

template <typename Func>
void Registry::UpdateInterestedSystems(const Entity& entity, Func&& update) const {
  const auto entityId = entity.GetId();
  const auto& entityComponentSignature = entity_component_signatures_[entityId];

  for (const auto& [typeIndex, system] : systems_) {
    const auto& systemComponentSignature = system->GetComponentSignature();

    if ((entityComponentSignature & systemComponentSignature) == systemComponentSignature) {
      update(*system);
    }
  }
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
void Registry::AddComponent(const Entity& entity, ComponentArg&& component) {
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
void Registry::AddComponent(const Entity& entity, TArgs&&... args) {
  T newComponent(std::forward<TArgs>(args)...);
  AddComponent(entity, std::move(newComponent));
}

template <typename T>
void Registry::RemoveComponent(const Entity& entity) {
  const auto componentId = Component<T>::GetId();
  const auto entityId = entity.GetId();

  entity_component_signatures_[entityId].set(componentId, false);

  auto componentPool = std::static_pointer_cast<Pool<T>>(component_pools_[componentId]);
  componentPool->Remove(entityId);

  Logger::Info("Removed component: " + std::to_string(entityId) + " from entity: " + std::to_string(entityId));
}

template <typename T>
bool Registry::HasComponent(const Entity& entity) const {
  return entity_component_signatures_[entity.GetId()].test(Component<T>::GetId());
}

template <typename T>
T& Registry::GetComponent(const Entity& entity) const {
  const auto componentId = Component<T>::GetId();
  const auto entityId = entity.GetId();

  if (!component_pools_[componentId]) {
    throw std::runtime_error("Component pool not found for component: " + std::to_string(componentId));
  }

  auto componentPool = std::static_pointer_cast<Pool<T>>(component_pools_[componentId]);

  return componentPool->Get(entityId);
}