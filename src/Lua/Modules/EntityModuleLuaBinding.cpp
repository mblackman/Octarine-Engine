#include "Lua/Modules/EntityModuleLuaBinding.h"

#include <glm/glm.hpp>
#include <string>

#include "Components/NameComponent.h"
#include "Components/PositionComponent.h"
#include "Components/SpriteComponent.h"
#include "ECS/Registry.h"
#include "Game/Game.h"
#include "General/Logger.h"
#include "Lua/Bindings/LuaComponentRegistry.h"

namespace
{
    glm::vec2 GetEntityPosition(Registry* registry, const Entity entity)
    {
        if (!registry->HasComponent<PositionComponent>(entity))
        {
            Logger::Error("Entity does not have PositionComponent.");
            return {0, 0};
        }
        return registry->GetComponent<PositionComponent>(entity).value;
    }

    std::string GetEntityName(Registry* registry, const Entity entity)
    {
        if (!registry->HasComponent<NameComponent>(entity)) return {};
        return registry->GetComponent<NameComponent>(entity).name;
    }

    void SetEntityName(Registry* registry, const Entity entity, const std::string& name)
    {
        registry->AddComponent(entity, NameComponent(name));
    }

    sol::object FindEntityByName(sol::this_state state, Registry* registry, const std::string& name)
    {
        for (const auto& entity : registry->GetUserEntities())
        {
            if (!registry->HasComponent<NameComponent>(entity)) continue;
            if (registry->GetComponent<NameComponent>(entity).name == name)
            {
                return sol::make_object(state, entity);
            }
        }
        return sol::lua_nil;
    }

    void SetEntityPosition(Registry* registry, const Entity entity, const double x, const double y)
    {
        if (!registry->HasComponent<PositionComponent>(entity))
        {
            Logger::Error("Entity does not have PositionComponent.");
            return;
        }
        registry->GetComponent<PositionComponent>(entity).value =
            glm::vec2(static_cast<float>(x), static_cast<float>(y));
    }

    void SetEntitySpriteSrcRect(Registry* registry, const Entity entity, const float srcRectX,
                                const float srcRectY)
    {
        if (!registry->HasComponent<SpriteComponent>(entity))
        {
            Logger::Error("Entity does not have SpriteComponent.");
            return;
        }
        auto& sprite = registry->GetComponent<SpriteComponent>(entity);
        sprite.srcRect.x = srcRectX;
        sprite.srcRect.y = srcRectY;
    }
} // namespace

void LuaModuleBinding<EntityModule>::install(sol::state& lua, Game& game)
{
    lua.set_function("blam", [&game](const Entity entity) { game.GetRegistry()->BlamEntity(entity); });
    lua.set_function("get_name", [&game](const Entity entity)
    {
        return GetEntityName(game.GetRegistry(), entity);
    });
    lua.set_function("set_name", [&game](const Entity entity, const std::string& name)
    {
        SetEntityName(game.GetRegistry(), entity, name);
    });
    lua.set_function("find_entity_by_name", [&game](sol::this_state state, const std::string& name)
    {
        return FindEntityByName(state, game.GetRegistry(), name);
    });
    lua.set_function("get_position",
                     [&game](const Entity entity) { return GetEntityPosition(game.GetRegistry(), entity); });
    lua.set_function("set_position", [&game](const Entity entity, const double x, const double y)
    {
        SetEntityPosition(game.GetRegistry(), entity, x, y);
    });
    lua.set_function("set_sprite_src_rect", [&game](const Entity entity, const float x, const float y)
    {
        SetEntitySpriteSrcRect(game.GetRegistry(), entity, x, y);
    });

    lua["registry"] = lua.create_table();
    sol::table reg = lua["registry"];

    reg.set_function("get_parent", [&game](sol::this_state state, const Entity entity) -> sol::object
    {
        const auto parent = game.GetRegistry()->GetParent(entity);
        if (!parent.has_value()) return sol::lua_nil;
        return sol::make_object(state, parent.value());
    });

    // Component has_/get_ accessors — driven by LuaComponentRegistry. Adding a new
    // component automatically exposes `registry.has_<key>` and `registry.get_<key>`.
    for (const auto& entry : LuaComponentRegistry::all())
    {
        entry.bindRegistryAccessors(reg, game.GetRegistry());
    }
}
