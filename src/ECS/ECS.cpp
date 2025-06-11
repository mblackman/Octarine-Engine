#include "ECS.h"

#include <algorithm>
#include <deque>
#include <memory>
#include <optional>
#include <stdexcept>
#include <vector>

#include "../General/Logger.h"
#include "General/Collections.h"

int IComponent::next_id_ = 0;

// Entity Implementation
int Entity::GetId() const { return id_; }

std::optional<Entity> Entity::GetParent() const { return registry_->GetParent(*this); }

std::optional<std::vector<Entity>> Entity::GetChildren() const { return registry_->GetChildren(*this); }

void Entity::Tag(const std::string& tag) const { registry_->TagEntity(*this, tag); }

bool Entity::HasTag(const std::string& tag) const { return registry_->EntityHasTag(*this, tag); }

void Entity::Group(const std::string& group) const { registry_->GroupEntity(*this, group); }

bool Entity::InGroup(const std::string& group) const { return registry_->EntityInGroup(*this, group); }

void Entity::Blam() const { registry_->BlamEntity(*this); }

void Entity::AddParent(const Entity& parent) const { registry_->SetParent(parent, *this); }

void Entity::RemoveParent() const {}

// Systems implementation
void System::AddEntity(const Entity& entity) { entities_.push_back(entity); }

void System::RemoveEntity(const Entity& entity) { Collections::SwapAndPop(entities_, entity); }

void System::AddRootEntity(const Entity& entity) { root_entities_.push_back(entity); }

void System::RemoveRootEntity(const Entity& entity) { Collections::SwapAndPop(root_entities_, entity); }

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

  const Entity entity(static_cast<int>(entityId), this);

  entities_to_add_.insert(entity);

  Logger::Info("Created entity: " + std::to_string(entityId));

  return entity;
}

void Registry::BlamEntity(const Entity& entity) { entities_to_remove_.insert(entity); }

void Registry::SetParent(const Entity& parent, const Entity& child) {
  // TODO Check for infinite loops
  entity_child_to_add_parent_.emplace_back(child, parent);
}

void Registry::RemoveParent(const Entity& entity) { entities_to_remove_parent_.insert(entity); }

std::optional<Entity> Registry::GetParent(const Entity& entity) {
  if (const auto it = child_to_parent_map_.find(entity.GetId()); it != child_to_parent_map_.end()) {
    return it->second;
  }
  return {};
}
std::optional<std::vector<Entity>> Registry::GetChildren(const Entity& entity) {
  if (const auto it = parent_to_children_.find(entity.GetId()); it != parent_to_children_.end()) {
    return it->second;
  }
  return {};
}

void Registry::AddEntityToSystems(const Entity& entity, const bool isRoot) const {
  UpdateInterestedSystems(entity, [entity, isRoot](System& system) {
    system.AddEntity(entity);

    if (isRoot) {
      system.AddRootEntity(entity);
    }
  });
}

void Registry::RemoveEntityFromSystems(const Entity& entity) const {
  UpdateInterestedSystems(entity, [entity](System& system) {
    system.RemoveEntity(entity);
    system.RemoveRootEntity(entity);
  });
}
void Registry::AddRootEntityToSystems(const Entity& entity) const {
  UpdateInterestedSystems(entity, [entity](System& system) { system.AddRootEntity(entity); });
}

void Registry::RemoveRootEntityFromSystems(const Entity& entity) const {
  UpdateInterestedSystems(entity, [entity](System& system) { system.RemoveRootEntity(entity); });
}

void Registry::UpdateAddEntities() {
  for (auto entity : entities_to_add_) {
    const bool isRoot = child_to_parent_map_.find(entity.GetId()) == child_to_parent_map_.end();
    entities_.push_back(entity);
    if (isRoot) {
      root_entities_.push_back(entity);
    }
    AddEntityToSystems(entity, isRoot);
  }

  entities_to_add_.clear();
}
void Registry::UpdateProcessParentAdditions() {
  for (const auto& [child, parent] : entity_child_to_add_parent_) {
    const int childId = child.GetId();
    if (child_to_parent_map_.count(childId) && child_to_parent_map_.at(childId) == parent) {
      Logger::Info("Entity " + std::to_string(childId) + " already has parent " + std::to_string(parent.GetId()));
      continue;
    }

    parent_to_children_[parent.GetId()].emplace_back(child);
    child_to_parent_map_.insert_or_assign(childId, parent);

    // Remove child from root entities if it was a root entity
    if (Collections::SwapAndPop(root_entities_, child)) {
      RemoveRootEntityFromSystems(child);
    }

    Logger::Info("Set parent of entity " + std::to_string(childId) + " to " + std::to_string(parent.GetId()));
  }
  entity_child_to_add_parent_.clear();
}

void Registry::UpdateProcessParentRemovals() {
  for (auto child : entities_to_remove_parent_) {
    if (auto it = child_to_parent_map_.find(child.GetId()); it != child_to_parent_map_.end()) {
      const Entity oldParent = it->second;

      // Remove child from old parent's children list
      if (auto parentChildrenIt = parent_to_children_.find(oldParent.GetId());
          parentChildrenIt != parent_to_children_.end()) {
        Collections::SwapAndPop(parentChildrenIt->second, child);
        if (parentChildrenIt->second.empty()) {
          parent_to_children_.erase(parentChildrenIt);
        }
      }

      // Remove parent from child
      child_to_parent_map_.erase(it);

      // Add child to root entities as it no longer has a parent
      root_entities_.push_back(child);
      AddRootEntityToSystems(child);

      Logger::Info("Removed parent from entity " + std::to_string(child.GetId()));
    } else {
      Logger::Info("Entity " + std::to_string(child.GetId()) + " has no parent to remove.");
    }
  }
  entities_to_remove_parent_.clear();
}

void Registry::UpdateProcessEntityRemovals() {
  if (entities_to_remove_.empty()) {
    return;
  }

  std::deque<Entity> queue;

  for (const auto& entity : entities_to_remove_) {
    queue.push_back(entity);
  }

  while (!queue.empty()) {
    Entity current = queue.front();
    queue.pop_front();

    auto childrenIt = parent_to_children_.find(current.GetId());
    if (childrenIt != parent_to_children_.end()) {
      for (const auto& child : childrenIt->second) {
        if (entities_to_remove_.find(child) == entities_to_remove_.end()) {
          queue.push_back(child);
          entities_to_remove_.insert(child);
        }
      }
    }
  }

  for (const auto& entity : entities_to_remove_) {
    const int entityId = entity.GetId();

    auto parentIt = child_to_parent_map_.find(entityId);
    if (parentIt != child_to_parent_map_.end()) {
      const Entity& parent = parentIt->second;
      if (entities_to_remove_.find(parent) == entities_to_remove_.end()) {
        auto parentChildrenIt = parent_to_children_.find(parent.GetId());
        if (parentChildrenIt != parent_to_children_.end()) {
          Collections::SwapAndPop(parentChildrenIt->second, entity);
          if (parentChildrenIt->second.empty()) {
            parent_to_children_.erase(parentChildrenIt);
          }
        }
      }
    }

    child_to_parent_map_.erase(entityId);
    parent_to_children_.erase(entityId);

    RemoveEntityFromSystems(entity);
    free_ids_.push_front(entityId);
    entity_component_signatures_[entityId].reset();

    for (const auto& pool : component_pools_) {
      if (pool) {
        pool->Remove(entityId);
      }
    }

    RemoveEntityTag(entity);
    RemoveEntityGroups(entity);

    Collections::SwapAndPop(root_entities_, entity);
    Collections::SwapAndPop(entities_, entity);

    Logger::Info("Entity destroyed: " + std::to_string(entityId));
  }

  entities_to_remove_.clear();
}

void Registry::Update() {
  UpdateAddEntities();
  UpdateProcessParentAdditions();
  UpdateProcessParentRemovals();
  UpdateProcessEntityRemovals();
}

void Registry::TagEntity(const Entity& entity, const std::string& tag) {
  const auto existingTag = entity_by_tag_.find(tag);
  if (existingTag != entity_by_tag_.end()) {
    if (existingTag->second != entity) {
      throw std::runtime_error("Entity with tag: " + tag + " already exists.");
    }
    return;
  }

  entity_by_tag_.emplace(tag, entity);
  tag_by_entity_.emplace(entity.GetId(), tag);
}

bool Registry::EntityHasTag(const Entity& entity, const std::string& tag) const {
  if (entity_by_tag_.find(tag) == entity_by_tag_.end()) {
    return false;
  }

  return entity_by_tag_.at(tag) == entity;
}

Entity Registry::GetEntityByTag(const std::string& tag) const { return entity_by_tag_.at(tag); }

void Registry::RemoveEntityTag(const Entity& entity) {
  const auto tag = tag_by_entity_.find(entity.GetId());
  if (tag != tag_by_entity_.end()) {
    entity_by_tag_.erase(tag->second);
    tag_by_entity_.erase(entity.GetId());
  }
}

void Registry::GroupEntity(const Entity& entity, const std::string& group) {
  entities_by_groups_.emplace(group, std::set<Entity>());
  entities_by_groups_.at(group).emplace(entity);
  groups_by_entity_.emplace(entity.GetId(), std::set<std::string>());
  groups_by_entity_.at(entity.GetId()).emplace(group);
}

bool Registry::EntityInGroup(const Entity& entity, const std::string& group) const {
  const auto groups = groups_by_entity_.find(entity.GetId());
  if (groups == groups_by_entity_.end()) {
    return false;
  }

  return groups->second.find(group) != groups->second.end();
}

std::vector<Entity> Registry::GetEntitiesByGroup(const std::string& group) const {
  const auto entities = entities_by_groups_.find(group);

  if (entities == entities_by_groups_.end()) {
    return {};
  }

  return {entities->second.begin(), entities->second.end()};
}

void Registry::RemoveEntityGroup(const Entity& entity, const std::string& group) {
  const auto groups = groups_by_entity_.find(entity.GetId());
  if (groups != groups_by_entity_.end()) {
    groups->second.erase(group);
    if (groups->second.empty()) {
      groups_by_entity_.erase(entity.GetId());
    }
  } else {
    return;
  }

  const auto entities = entities_by_groups_.find(group);
  if (entities != entities_by_groups_.end()) {
    entities->second.erase(entity);
    if (entities->second.empty()) {
      entities_by_groups_.erase(group);
    }
  }
}

void Registry::RemoveEntityGroups(const Entity& entity) {
  const auto groups = groups_by_entity_.find(entity.GetId());
  if (groups != groups_by_entity_.end()) {
    for (const auto& group : groups->second) {
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