#pragma once
#include <stack>

#include "../Components/BoxColliderComponent.h"
#include "../Components/EntityMaskComponent.h"
#include "../Components/GlobalTransformComponent.h"
#include "../Components/NameComponent.h"
#include "../Components/PositionComponent.h"
#include "../Components/RotationComponent.h"
#include "../Components/ScaleComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/SquarePrimitiveComponent.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/UIButtonComponent.h"
#include "../ECS/Registry.h"
#include "../General/Logger.h"
#include "Lua/Bindings/LuaBinding.h"
#include "Lua/Bindings/LuaComponentRegistry.h"

class LuaEntityLoader
{
public:
  // Reads `tag`/`tags`/`group`/`groups` (string or list of strings) and applies each as a label
  // tag on the entity. `group(s)` is a back-compat alias for `tag(s)`.
  static void TagAndGroupEntity(const sol::table& currentData, Registry* registry, const Entity& entity)
  {
    ApplyTagField(currentData, "tag", registry, entity);
    ApplyTagField(currentData, "tags", registry, entity);
  }

  // Reads top-level `name` string and attaches a NameComponent. Accepts plain strings only at
  // the top level (`components.name = "foo"` also works via the registry lookup below).
  static void ApplyName(const sol::table& currentData, Registry* registry, const Entity& entity)
  {
    const sol::object value = currentData.get<sol::object>("name");
    if (value.is<std::string>())
    {
      const std::string n = value.as<std::string>();
      if (!n.empty()) registry->AddComponent(entity, NameComponent(n));
    }
  }

  // Reads top-level `mask` int from the entity table and attaches an EntityMaskComponent.
  // Also attaches a default EntityMaskComponent when the entity declares a `box_collider`
  // (CollisionSystem queries this component, so collider entities must have one).
  static void ApplyEntityMask(const sol::table& currentData, Registry* registry, const Entity& entity)
  {
    const sol::object topLevelMask = currentData.get<sol::object>("entity_mask");
    const bool hasTopLevelMask = topLevelMask.valid() && topLevelMask.is<int>();

    sol::object componentMask = sol::lua_nil;
    bool hasComponentMask = false;
    bool hasCollider = false;
    const std::string componentsKey = "components";
    if (sol::optional<sol::table> componentsOpt = currentData[componentsKey];
      componentsOpt && componentsOpt.value().valid())
    {
      componentMask = componentsOpt.value().get<sol::object>("entity_mask");
      hasComponentMask = componentMask.valid() && componentMask.is<int>();
      const sol::object boxCollider = componentsOpt.value().get<sol::object>("box_collider");
      hasCollider = boxCollider.valid() && boxCollider.is<sol::table>();
    }

    if (hasTopLevelMask && hasComponentMask)
    {
      Logger::Error("LoadEntityFromLua: entity_mask set at both top-level and components - pick one. Skipping mask.");
      return;
    }

    if (!hasTopLevelMask && !hasComponentMask && !hasCollider) return;

    const sol::object& maskValue = hasTopLevelMask ? topLevelMask : componentMask;
    const int raw = maskValue.is<int>() ? maskValue.as<int>() : Constants::kDefaultEntityMask;
    registry->AddComponent(entity, EntityMaskComponent(EntityMask(static_cast<unsigned long long>(raw))));
  }

  static void LoadEntityComponents(const sol::table& currentData, Registry* registry, const Entity& entity)
  {
    const std::string componentsKey = "components";
    sol::optional<sol::table> componentsTableOpt = currentData[componentsKey];
    if (!componentsTableOpt || !componentsTableOpt.value().valid())
    {
      Logger::Info("LoadEntityFromLua: Entity has no 'components' table. Skipping.");
      return;
    }

    const sol::table& componentsTable = componentsTableOpt.value();
    for (const auto& [name, data] : componentsTable)
    {
      auto componentName = name.as<std::string>();

      // entity_mask handled in ApplyEntityMask (also validates top-level vs component-level conflict).
      if (componentName == "entity_mask") continue;

      // `transform` explodes into up-to-three real components. Per-field tables are optional;
      // omitted fields fall back to system defaults (identity).
      if (componentName == "transform")
      {
        if (!data.is<sol::table>())
        {
          Logger::Error("LoadEntityFromLua: 'transform' must be a table.");
          continue;
        }
        const sol::table t = data.as<sol::table>();
        using namespace LuaComponentHelpers;
        if (t["position"].valid())
        {
          registry->AddComponent(entity, PositionComponent(SafeGetVec2(t, "position")));
        }
        if (t["scale"].valid())
        {
          registry->AddComponent(entity, ScaleComponent(SafeGetVec2(t, "scale", 1.0f, 1.0f)));
        }
        if (t["rotation"].valid())
        {
          registry->AddComponent(entity, RotationComponent(SafeGetOptionalValue<double>(t, "rotation", 0.0)));
        }
        continue;
      }

      const sol::object dataObj = data;
      if (const auto* entry = LuaComponentRegistry::find(componentName))
      {
        entry->attach(registry, entity, dataObj);
      }
      else
      {
        Logger::Error("LoadEntityFromLua: Unknown component type '" + componentName + "' in Lua table.");
      }
    }

    MaybeAttachGlobalTransform(registry, entity);
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
  static void LoadEntityFromLua(Registry* registry, const sol::table& entityData)
  {
    if (!entityData.valid())
    {
      Logger::Error("LoadEntityFromLua: Invalid root entity data table.");
      return;
    }

    std::stack<std::pair<sol::table, std::optional<Entity>>> nodesToProcess;
    nodesToProcess.emplace(entityData, std::nullopt);

    while (!nodesToProcess.empty())
    {
      auto [currentData, parentEntity] = nodesToProcess.top();
      nodesToProcess.pop();

      // Create the entity and establish parentage
      const Entity entity = registry->CreateEntity();
      if (parentEntity)
      {
        registry->SetParent(entity, *parentEntity);
      }

      ApplyName(currentData, registry, entity);
      TagAndGroupEntity(currentData, registry, entity);
      ApplyEntityMask(currentData, registry, entity);
      LoadEntityComponents(currentData, registry, entity);

      // Add any child entities to the stack to be processed.
      const std::string entitiesKey = "entities";
      sol::optional<sol::table> childEntitiesOpt = currentData[entitiesKey];
      if (childEntitiesOpt && childEntitiesOpt.value().valid())
      {
        const sol::table& childEntitiesTable = childEntitiesOpt.value();

        std::vector<sol::table> children;
        children.reserve(childEntitiesTable.size());
        for (const auto& [key, value] : childEntitiesTable)
        {
          children.push_back(value.as<sol::table>());
        }

        for (auto it = children.rbegin(); it != children.rend(); ++it)
        {
          nodesToProcess.emplace(*it, entity);
        }
      }
    }
  }

private:
  // Add a GlobalTransformComponent iff (a) any local transform component is present and
  // (b) at least one world-space consumer system (sprite/primitive/collider/UI button/text label)
  // is attached. Keeps script-only or camera-anchor entities lean.
  static void MaybeAttachGlobalTransform(Registry* registry, const Entity& entity)
  {
    const bool hasAnyLocal = registry->HasComponent<PositionComponent>(entity) ||
      registry->HasComponent<ScaleComponent>(entity) ||
      registry->HasComponent<RotationComponent>(entity);
    if (!hasAnyLocal) return;

    const bool needsGlobal = registry->HasComponent<SpriteComponent>(entity) ||
      registry->HasComponent<SquarePrimitiveComponent>(entity) ||
      registry->HasComponent<BoxColliderComponent>(entity) ||
      registry->HasComponent<UIButtonComponent>(entity) ||
      registry->HasComponent<TextLabelComponent>(entity);
    if (!needsGlobal) return;

    if (registry->HasComponent<GlobalTransformComponent>(entity)) return;
    registry->AddComponent(entity, GlobalTransformComponent{});
  }

  static void ApplyTagField(const sol::table& currentData, const char* key, Registry* registry, const Entity& entity)
  {
    const sol::object value = currentData.get<sol::object>(key);
    if (!value.valid()) return;

    if (value.is<std::string>())
    {
      const std::string name = value.as<std::string>();
      if (!name.empty()) registry->AddTag(entity, name);
      return;
    }

    if (value.is<sol::table>())
    {
      for (const auto& [_, inner] : value.as<sol::table>())
      {
        if (!inner.is<std::string>()) continue;
        const std::string name = inner.as<std::string>();
        if (!name.empty()) registry->AddTag(entity, name);
      }
    }
  }
};
