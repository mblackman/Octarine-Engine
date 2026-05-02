#include "ECS.h"

#include <algorithm>
#include <deque>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

#include "../General/Logger.h"

int IComponent::next_id_ = 0;

// Entity Implementation
int Entity::GetId() const {
    return id_;
}

void Entity::Tag(const std::string& tag) {
    registry_->TagEntity(*this, tag);
}

bool Entity::HasTag(const std::string& tag) const {
    return registry_->EntityHasTag(*this, tag);
}

void Entity::Group(const std::string& group) {
    registry_->GroupEntity(*this, group);
}

bool Entity::InGroup(const std::string& group) const {
    return registry_->EntityInGroup(*this, group);
}

void Entity::Blam() {
    registry_->BlamEntity(*this);
}

// Systems implementation
void System::AddEntity(const Entity entity) {
    entities_.push_back(entity);
}

void System::RemoveEntity(const Entity entity) {
    entities_.erase(std::remove_if(
                        entities_.begin(),
                        entities_.end(),
                        [&entity](const Entity& other) {
                            return other == entity;
                        }),
                    entities_.end());
}

// Registry implementation
Entity Registry::CreateEntity() {
    std::size_t entityId;

    if (free_ids_.empty()) {
        entityId = num_entities_++;

        if (entityId >= entity_component_signatures_.size()) {
            entity_component_signatures_.resize(entityId + 1);
        }
    } else {
        entityId = free_ids_.front();
        free_ids_.pop_front();
    }

    Entity entity(entityId, this);

    entities_to_add_.insert(entity);

    Logger::Info("Created entity: " + std::to_string(entityId));

    return entity;
}

void Registry::BlamEntity(const Entity entity) {
    entities_to_remove_.insert(entity);
}

void Registry::AddEntityToSystems(Entity entity) {
    const auto entityId = entity.GetId();

    const auto& entityComponentSignature = entity_component_signatures_[entityId];

    for (auto& system : systems_) {
        const auto& systemComponentSignature = system.second->GetComponentSignature();

        bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;

        if (isInterested) {
            system.second->AddEntity(entity);
        }
    }
}

void Registry::RemoveEntityFromSystems(const Entity entity) {
    for (auto& system : systems_) {
        system.second->RemoveEntity(entity);
    }
}

void Registry::Update() {
    for (auto entity : entities_to_add_) {
        entities_.push_back(entity);
        AddEntityToSystems(entity);
    }

    entities_to_add_.clear();

    for (auto entity : entities_to_remove_) {
        RemoveEntityFromSystems(entity);
        free_ids_.push_front(entity.GetId());
        entity_component_signatures_[entity.GetId()].reset();

        for (auto pool : component_pools_) {
            if (pool) {
                pool->Remove(entity.GetId());
            }
        }

        RemoveEntityTag(entity);
        RemoveEntityGroups(entity);

        Logger::Info("Entity destroyed: " + std::to_string(entity.GetId()));
    }

    entities_to_remove_.clear();
}

void Registry::TagEntity(Entity entity, const std::string& tag) {
    auto existingTag = entity_by_tag_.find(tag);
    if (existingTag != entity_by_tag_.end()) {
        if (existingTag->second != entity) {
            throw std::runtime_error("Entity with tag: " + tag + " already exists.");
        }
        return;
    }

    entity_by_tag_.emplace(tag, entity);
    tag_by_entity_.emplace(entity.GetId(), tag);
}

bool Registry::EntityHasTag(Entity entity, const std::string& tag) const {
    if (entity_by_tag_.find(tag) == entity_by_tag_.end()) {
        return false;
    }

    return (entity_by_tag_.at(tag) == entity);
}

Entity Registry::GetEntityByTag(const std::string& tag) const {
    return entity_by_tag_.at(tag);
}

void Registry::RemoveEntityTag(Entity entity) {
    const auto tag = tag_by_entity_.find(entity.GetId());
    if (tag != tag_by_entity_.end()) {
        entity_by_tag_.erase(tag->second);
        tag_by_entity_.erase(entity.GetId());
    }
}

void Registry::GroupEntity(Entity entity, const std::string& group) {
    entities_by_groups_.emplace(group, std::set<Entity>());
    entities_by_groups_.at(group).emplace(entity);
    groups_by_entity_.emplace(entity.GetId(), std::set<std::string>());
    groups_by_entity_.at(entity.GetId()).emplace(group);
}

bool Registry::EntityInGroup(Entity entity, const std::string& group) const {
    auto groups = groups_by_entity_.find(entity.GetId());
    if (groups == groups_by_entity_.end()) {
        return false;
    }

    return groups->second.find(group) != groups->second.end();
}

std::vector<Entity> Registry::GetEntitiesByGroup(const std::string& group) const {
    auto entities = entities_by_groups_.find(group);

    if (entities == entities_by_groups_.end()) {
        return std::vector<Entity>();
    }

    return std::vector(entities->second.begin(), entities->second.end());
}

void Registry::RemoveEntityGroup(Entity entity, const std::string& group) {
    auto groups = groups_by_entity_.find(entity.GetId());
    if (groups != groups_by_entity_.end()) {
        groups->second.erase(group);
        if (groups->second.empty()) {
            groups_by_entity_.erase(entity.GetId());
        }
    } else {
        return;
    }

    auto entities = entities_by_groups_.find(group);
    if (entities != entities_by_groups_.end()) {
        entities->second.erase(entity);
        if (entities->second.empty()) {
            entities_by_groups_.erase(group);
        }
    }
}

void Registry::RemoveEntityGroups(Entity entity) {
    auto groups = groups_by_entity_.find(entity.GetId());
    if (groups != groups_by_entity_.end()) {
        for (auto group : groups->second) {
            auto entities = entities_by_groups_.find(group);
            if (entities != entities_by_groups_.end()) {
                entities->second.erase(entity);
                if (entities->second.empty()) {
                    entities_by_groups_.erase(group);
                }
            }
        }
        groups_by_entity_.erase(entity.GetId());
    }
}