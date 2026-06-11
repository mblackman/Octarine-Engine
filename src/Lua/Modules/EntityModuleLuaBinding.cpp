#include "Lua/Modules/EntityModuleLuaBinding.h"

#include <glm/glm.hpp>
#include <string>

#include "Components/NameComponent.h"
#include "Components/PositionComponent.h"
#include "Components/SpriteComponent.h"
#include "ECS/Registry.h"
#include "General/Logger.h"
#include "Lua/Bindings/LuaComponentRegistry.h"
#include "Lua/LuaBindingContext.h"

namespace {
glm::vec2 GetEntityPosition(Registry* registry, const Entity entity) {
  if (!registry->HasComponent<PositionComponent>(entity)) {
    Logger::Error("Entity does not have PositionComponent.");
    return {0, 0};
  }
  return registry->GetComponent<PositionComponent>(entity).value;
}

std::string GetEntityName(Registry* registry, const Entity entity) {
  if (!registry->HasComponent<NameComponent>(entity)) return {};
  return registry->GetComponent<NameComponent>(entity).name;
}

void SetEntityName(Registry* registry, const Entity entity, const std::string& name) {
  registry->AddComponent(entity, NameComponent(name));
}

sol::object FindEntityByName(sol::this_state state, Registry* registry, const std::string& name) {
  for (const auto& entity : registry->GetUserEntities()) {
    if (!registry->HasComponent<NameComponent>(entity)) continue;
    if (registry->GetComponent<NameComponent>(entity).name == name) {
      return sol::make_object(state, entity);
    }
  }
  return sol::lua_nil;
}

void SetEntityPosition(Registry* registry, const Entity entity, const double x, const double y) {
  if (!registry->HasComponent<PositionComponent>(entity)) {
    Logger::Error("Entity does not have PositionComponent.");
    return;
  }
  registry->GetComponent<PositionComponent>(entity).value = glm::vec2(static_cast<float>(x), static_cast<float>(y));
}

void SetEntitySpriteSrcRect(Registry* registry, const Entity entity, const float srcRectX, const float srcRectY) {
  if (!registry->HasComponent<SpriteComponent>(entity)) {
    Logger::Error("Entity does not have SpriteComponent.");
    return;
  }
  auto& sprite = registry->GetComponent<SpriteComponent>(entity);
  sprite.srcRect.x = srcRectX;
  sprite.srcRect.y = srcRectY;
}

void SetEntityParent(Registry* registry, const Entity child, const Entity parent) {
  if (child.id == parent.id) {
    Logger::Error("set_parent: entity cannot be its own parent.");
    return;
  }
  // Reject cycles: the new parent must not already be a descendant of the child.
  for (auto ancestor = registry->GetParent(parent); ancestor.has_value(); ancestor = registry->GetParent(*ancestor)) {
    if (ancestor->id == child.id) {
      Logger::Error("set_parent: would create a hierarchy cycle.");
      return;
    }
  }
  registry->SetParent(child, parent);
}

void BlamEntities(Registry* registry, const sol::table& entities) {
  for (const auto& [key, value] : entities) {
    if (!value.is<Entity>()) {
      Logger::Error("blam: table entry is not an entity; skipping.");
      continue;
    }
    registry->QueueBlamEntity(value.as<Entity>());
  }
}
}  // namespace

void LuaModuleBinding<EntityModule>::install(sol::state& lua, LuaBindingContext& ctx) {
  // Destruction is deferred: blam queues the entity and Registry::Update destroys the whole
  // batch after all systems have run, so a script can never pull a chunk out from under an
  // iterating system mid-frame.
  lua.set_function("blam",
                   sol::overload([&ctx](const Entity entity) { ctx.GetRegistry()->QueueBlamEntity(entity); },
                                 [&ctx](const sol::table& entities) { BlamEntities(ctx.GetRegistry(), entities); }));
  lua.set_function("get_name", [&ctx](const Entity entity) { return GetEntityName(ctx.GetRegistry(), entity); });
  lua.set_function("set_name", [&ctx](const Entity entity, const std::string& name) {
    SetEntityName(ctx.GetRegistry(), entity, name);
  });
  lua.set_function("find_entity_by_name", [&ctx](sol::this_state state, const std::string& name) {
    return FindEntityByName(state, ctx.GetRegistry(), name);
  });
  lua.set_function("get_position",
                   [&ctx](const Entity entity) { return GetEntityPosition(ctx.GetRegistry(), entity); });
  lua.set_function("set_position", [&ctx](const Entity entity, const double x, const double y) {
    SetEntityPosition(ctx.GetRegistry(), entity, x, y);
  });
  lua.set_function("set_sprite_src_rect", [&ctx](const Entity entity, const float x, const float y) {
    SetEntitySpriteSrcRect(ctx.GetRegistry(), entity, x, y);
  });

  lua["registry"] = lua.create_table();
  sol::table reg = lua["registry"];

  reg.set_function("get_parent", [&ctx](sol::this_state state, const Entity entity) -> sol::object {
    const auto parent = ctx.GetRegistry()->GetParent(entity);
    if (!parent.has_value()) return sol::lua_nil;
    return sol::make_object(state, parent.value());
  });

  reg.set_function("set_parent", [&ctx](const Entity child, const Entity parent) {
    SetEntityParent(ctx.GetRegistry(), child, parent);
  });

  // Component has_/get_ accessors — driven by LuaComponentRegistry. Adding a new
  // component automatically exposes `registry.has_<key>` and `registry.get_<key>`.
  for (const auto& entry : LuaComponentRegistry::all()) {
    entry.bindRegistryAccessors(reg, ctx.GetRegistry());
  }
}
