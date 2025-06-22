#pragma once
#include <functional>
#include <stack>

#include "../Ecs/Registry.h"
#include "ComponentLuaFactory.h"

class LuaEntityLoader {
 public:
  using ComponentCreationFunction = std::function<void(Registry*, Entity, const sol::table&)>;

  // TODO Fix all this
  static void TagAndGroupEntity(const sol::table& currentData, const Entity& entity) {
    const sol::optional<std::string> tag = currentData["tag"];
    const sol::optional<std::string> group = currentData["group"];
    const int entityMask = currentData["mask"].get_or(Constants::kDefaultEntityMask);

    if (tag.has_value() && !tag.value().empty()) {
      // entity.Tag(tag.value());
    }

    if (group.has_value() && !group.value().empty()) {
      // entity.Group(group.value());
    }

    // entity.SetEntityMask(entityMask);
  }

  static void LoadEntityComponents(const sol::table& currentData, Registry* registry, const Entity& entity) {
    sol::optional<sol::table> componentsTableOpt = currentData["components"];
    if (!componentsTableOpt || !componentsTableOpt.value().valid()) {
      std::cout << "LoadEntityFromLua: Entity has no 'components' table. Skipping." << std::endl;
      return;
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
        it->second(registry, entity, componentDataTable);
      } else {
        std::cerr << "LoadEntityFromLua: Unknown component type '" << componentName << "' in Lua table." << std::endl;
      }
    }
  }

  /**
   * @brief Loads entities from a Lua table definition, creating them in the registry->
   *
   * This function iteratively processes a tree-like structure of entities from a Lua
   * table. It uses a stack to manage parent-child relationships and avoid deep recursion.
   *
   * @param registry A pointer to the game's entity-component-system registry->
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
        // TODO add back relationships
        // entity.AddParent(*parentEntity);
      }

      TagAndGroupEntity(currentData, entity);
      LoadEntityComponents(currentData, registry, entity);

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

    factories["transform"] = [](Registry* registry, const Entity ent, const sol::table& data) {
      registry->AddComponent(ent, ComponentLuaFactory::CreateTransformComponent(data));
    };
    factories["rigidbody"] = [](Registry* registry, const Entity ent, const sol::table& data) {
      registry->AddComponent(ent, ComponentLuaFactory::CreateRigidBodyComponent(data));
    };
    factories["sprite"] = [](Registry* registry, const Entity ent, const sol::table& data) {
      registry->AddComponent(ent, ComponentLuaFactory::CreateSpriteComponent(data));
    };
    factories["square"] = [](Registry* registry, const Entity ent, const sol::table& data) {
      registry->AddComponent(ent, ComponentLuaFactory::CreateSquarePrimitiveComponent(data));
    };
    factories["animation"] = [](Registry* registry, const Entity ent, const sol::table& data) {
      registry->AddComponent(ent, ComponentLuaFactory::CreateAnimationComponent(data));
    };
    factories["box_collider"] = [](Registry* registry, const Entity ent, const sol::table& data) {
      registry->AddComponent(ent, ComponentLuaFactory::CreateBoxColliderComponent(data));
    };
    factories["health"] = [](Registry* registry, const Entity ent, const sol::table& data) {
      registry->AddComponent(ent, ComponentLuaFactory::CreateHealthComponent(data));
    };
    factories["projectile_emitter"] = [](Registry* registry, const Entity ent, const sol::table& data) {
      registry->AddComponent(ent, ComponentLuaFactory::CreateProjectileEmitterComponent(data));
    };
    factories["camera_follow"] = [](Registry* registry, Entity ent, const sol::table& data) {
      registry->AddComponent(ent, ComponentLuaFactory::CreateCameraFollowComponent(data));
    };
    factories["keyboard_controller"] = [](Registry* registry, Entity ent, const sol::table& data) {
      registry->AddComponent(ent, ComponentLuaFactory::CreateKeyboardControlledComponent(data));
    };
    factories["script"] = [](Registry* registry, Entity ent, const sol::table& data) {
      registry->AddComponent(ent, ComponentLuaFactory::CreateScriptComponent(data));
    };
    factories["ui_button"] = [](Registry* registry, const Entity ent, const sol::table& data) {
      registry->AddComponent(ent, ComponentLuaFactory::CreateUIButtonComponent(data));
    };
    factories["text_label"] = [](Registry* registry, const Entity ent, const sol::table& data) {
      registry->AddComponent(ent, ComponentLuaFactory::CreateTextLabelComponent(data));
    };

    return factories;
  }
};
