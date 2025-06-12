#pragma once
#include <functional>
#include <stack>

#include "ComponentLuaFactory.h"
#include "Components/BoxColliderComponent.h"
#include "Components/CameraFollowComponent.h"
#include "Components/HealthComponent.h"
#include "Components/KeyboardControlComponent.h"
#include "Components/ProjectileEmitterComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/ScriptComponent.h"
#include "Components/TransformComponent.h"
#include "Components/UIButtonComponent.h"
#include "ECS/ECS.h"

class LuaEntityLoader {
 public:
  using ComponentCreationFunction = std::function<void(Entity, const sol::table&)>;

  /**
   * @brief Loads entities from a Lua table definition, creating them in the registry.
   *
   * This function iteratively processes a tree-like structure of entities from a Lua
   * table. It uses a stack to manage parent-child relationships and avoid deep recursion.
   *
   * @param registry A pointer to the game's entity-component-system registry.
   * @param entityData The top-level sol::table containing the entity definition.
   */
  static void LoadEntityFromLua(Registry* registry, const sol::table& entityData) {
    if (!entityData.valid()) {
      std::cerr << "LoadEntityFromLua: Invalid root entity data table." << std::endl;
      return;
    }

    std::stack<std::pair<sol::table, std::optional<Entity>>> nodesToProcess;
    nodesToProcess.emplace(entityData, std::nullopt);

    while (!nodesToProcess.empty()) {
      auto [currentData, parentEntity] = nodesToProcess.top();
      nodesToProcess.pop();

      // Create the entity and establish parentage
      const Entity entity = registry->CreateEntity();
      if (parentEntity) {
        entity.AddParent(*parentEntity);
      }

      // An entity must have a component table to be valid.
      sol::optional<sol::table> componentsTableOpt = currentData["components"];
      if (!componentsTableOpt || !componentsTableOpt.value().valid()) {
        std::cout << "LoadEntityFromLua: Entity has no 'components' table. Skipping children." << std::endl;
        continue;
      }

      // Add tag and groups

      const sol::optional<std::string> tag = currentData["tag"];
      const sol::optional<std::string> group = currentData["group"];

      if (tag.has_value() && !tag.value().empty()) {
        entity.Tag(tag.value());
      }

      if (group.has_value() && !group.value().empty()) {
        entity.Group(group.value());
      }

      // Add all defined components to the entity
      const sol::table& componentsTable = componentsTableOpt.value();
      for (const auto& [name, data] : componentsTable) {
        auto componentName = name.as<std::string>();
        auto componentDataTable = data.as<sol::table>();

        if (!componentDataTable.valid()) {
          std::cerr << "LoadEntityFromLua: Invalid data table for component: " << componentName << std::endl;
          continue;
        }

        if (auto it = GetComponentFactoryMap().find(componentName); it != GetComponentFactoryMap().end()) {
          it->second(entity, componentDataTable);
        } else {
          std::cerr << "LoadEntityFromLua: Unknown component type '" << componentName << "' in Lua table." << std::endl;
        }
      }

      // Add any child entities to the stack to be processed.
      sol::optional<sol::table> childEntitiesOpt = currentData["entities"];
      if (childEntitiesOpt && childEntitiesOpt.value().valid()) {
        const sol::table& childEntitiesTable = childEntitiesOpt.value();

        std::vector<sol::table> children;
        children.reserve(childEntitiesTable.size());
        for (const auto& [key, value] : childEntitiesTable) {
          children.push_back(value.as<sol::table>());
        }

        for (auto it = children.rbegin(); it != children.rend(); ++it) {
          nodesToProcess.emplace(*it, entity);
        }
      }
    }
  }

 private:
  static const std::unordered_map<std::string, ComponentCreationFunction>& GetComponentFactoryMap() {
    static std::unordered_map<std::string, ComponentCreationFunction> componentFactories;
    if (componentFactories.empty()) {
      componentFactories = InitializeFactories();
    }
    return componentFactories;
  }

  static std::unordered_map<std::string, ComponentCreationFunction> InitializeFactories() {
    std::unordered_map<std::string, ComponentCreationFunction> factories;

    factories["transform"] = [](Entity ent, const sol::table& data) {
      ent.AddComponent<TransformComponent>(ComponentLuaFactory::CreateTransformComponent(data));
    };
    factories["rigidbody"] = [](Entity ent, const sol::table& data) {
      ent.AddComponent<RigidBodyComponent>(ComponentLuaFactory::CreateRigidBodyComponent(data));
    };
    factories["sprite"] = [](Entity ent, const sol::table& data) {
      ent.AddComponent<SpriteComponent>(ComponentLuaFactory::CreateSpriteComponent(data));
    };
    factories["square"] = [](Entity ent, const sol::table& data) {
      ent.AddComponent<SquarePrimitiveComponent>(ComponentLuaFactory::CreateSquarePrimitiveComponent(data));
    };
    factories["animation"] = [](Entity ent, const sol::table& data) {
      ent.AddComponent<AnimationComponent>(ComponentLuaFactory::CreateAnimationComponent(data));
    };
    factories["box_collider"] = [](Entity ent, const sol::table& data) {
      ent.AddComponent<BoxColliderComponent>(ComponentLuaFactory::CreateBoxColliderComponent(data));
    };
    factories["health"] = [](Entity ent, const sol::table& data) {
      ent.AddComponent<HealthComponent>(ComponentLuaFactory::CreateHealthComponent(data));
    };
    factories["projectile_emitter"] = [](Entity ent, const sol::table& data) {
      ent.AddComponent<ProjectileEmitterComponent>(ComponentLuaFactory::CreateProjectileEmitterComponent(data));
    };
    factories["camera_follow"] = [](Entity ent, const sol::table& data) {
      ent.AddComponent<CameraFollowComponent>(ComponentLuaFactory::CreateCameraFollowComponent(data));
    };
    factories["keyboard_controller"] = [](Entity ent, const sol::table& data) {
      ent.AddComponent<KeyboardControlComponent>(ComponentLuaFactory::CreateKeyboardControlledComponent(data));
    };
    factories["script"] = [](Entity ent, const sol::table& data) {
      ent.AddComponent<ScriptComponent>(ComponentLuaFactory::CreateScriptComponent(data));
    };
    factories["ui_button"] = [](Entity ent, const sol::table& data) {
      ent.AddComponent<UIButtonComponent>(ComponentLuaFactory::CreateUIButtonComponent(data));
    };
    factories["text_label"] = [](Entity ent, const sol::table& data) {
      ent.AddComponent<TextLabelComponent>(ComponentLuaFactory::CreateTextLabelComponent(data));
    };

    return factories;
  }
};
