#pragma once
#include <functional>

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

  static void LoadEntityFromLua(Registry* registry, const sol::table& entityData) {
    if (!entityData.valid()) {
      std::cerr << "LoadEntityFromLua: Invalid entity data table." << std::endl;
      return;
    }

    const Entity entity = registry->CreateEntity();

    sol::optional<sol::table> componentsTableOpt = entityData["components"];
    if (!componentsTableOpt || !componentsTableOpt.value().valid()) {
      std::cout << "LoadEntityFromLua: Entity has no 'components' table." << std::endl;
      return;
    }

    const sol::table componentsTable = componentsTableOpt.value();

    for (const auto& [fst, snd] : componentsTable) {
      auto componentName = fst.as<std::string>();
      auto componentDataTable = snd.as<sol::table>();

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
    factories["square_primitive"] = [](Entity ent, const sol::table& data) {
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

    return factories;
  }
};
