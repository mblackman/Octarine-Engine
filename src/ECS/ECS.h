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
   private:
    int id_;
    class Registry* registry_;

   public:
    Entity(int id, Registry* registry) : id_(id), registry_(registry) {}

    int GetId() const;

    bool operator==(const Entity& other) const {
        return id_ == other.id_;
    }

    bool operator!=(const Entity& other) const {
        return id_ != other.id_;
    }

    bool operator<(const Entity& other) const {
        return id_ < other.id_;
    }

    bool operator>(const Entity& other) const {
        return id_ > other.id_;
    }

    bool operator<=(const Entity& other) const {
        return id_ <= other.id_;
    }

    bool operator>=(const Entity& other) const {
        return id_ >= other.id_;
    }

    template <typename T, typename... TArgs>
    void AddComponent(TArgs&&... args);

    template <typename T>
    void RemoveComponent();

    template <typename T>
    bool HasComponent() const;

    template <typename T>
    T& GetComponent() const;

    void Tag(const std::string& tag);
    bool HasTag(const std::string& tag) const;
    void Group(const std::string& group);
    bool InGroup(const std::string& group) const;

    void Blam();
};

class System {
   private:
    Signature component_signature_;
    std::vector<Entity> entities_;

   public:
    System() = default;
    ~System() = default;

    const Signature& GetComponentSignature() const {
        return component_signature_;
    }

    std::vector<Entity> GetEntities() const {
        return entities_;
    }

    void AddEntity(const Entity entity);
    void RemoveEntity(const Entity entity);

    template <typename T>
    void RequireComponent();
};

/**
 * Manages the creation and destruction of entities, systems, and components.
 */
class Registry {
   private:
    int num_entities_;
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
    ~Registry() = default;

    void Update();

    // Entity management
    Entity CreateEntity();

    void BlamEntity(const Entity entity);

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

    void AddEntityToSystems(const Entity entity);

    void RemoveEntityFromSystems(const Entity entity);

    // Component management
    template <typename T, typename... TArgs>
    void AddComponent(const Entity entity, TArgs&&... args);

    template <typename T>
    void RemoveComponent(const Entity entity);

    template <typename T>
    bool HasComponent(const Entity entity) const;

    template <typename T>
    T& GetComponent(const Entity entity) const;
};

// Entity implementations
template <typename T, typename... TArgs>
void Entity::AddComponent(TArgs&&... args) {
    registry_->AddComponent<T>(*this, std::forward<TArgs>(args)...);
}

template <typename T>
void Entity::RemoveComponent() {
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
    auto it = systems_.find(std::type_index(typeid(T)));

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
    auto it = systems_.find(std::type_index(typeid(T)));
    return *(std::static_pointer_cast<T>(it->second));
}

template <typename T, typename... TArgs>
void Registry::AddComponent(const Entity entity, TArgs&&... args) {
    T newComponent(std::forward<TArgs>(args)...);
    const auto componentId = Component<T>::GetId();
    const auto entityId = entity.GetId();

    if (componentId >= static_cast<int>(component_pools_.size())) {
        component_pools_.resize(componentId + 1, nullptr);
    }

    if (!component_pools_[componentId]) {
        auto newComponentPool = std::make_shared<Pool<T>>();
        component_pools_[componentId] = newComponentPool;
    }

    auto componentPool = std::static_pointer_cast<Pool<T>>(component_pools_[componentId]);

    componentPool->Set(entityId, newComponent);

    entity_component_signatures_[entityId].set(componentId);

    Logger::Info("Added component: " + std::to_string(componentId) + " to entity: " + std::to_string(entityId));
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