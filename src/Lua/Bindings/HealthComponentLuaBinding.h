#pragma once

#include <sol/sol.hpp>

#include "Components/HealthComponent.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<HealthComponent>
{
    static constexpr const char* kLuaKey = "health";
    static constexpr const char* kUsertypeName = "health_component";

    static HealthComponent fromLua(const sol::object& data)
    {
        const auto t = data.as<sol::table>();
        using namespace LuaComponentHelpers;
        const int maxHealth = SafeGetOptionalValue<int>(t, "max_health", 100);
        return HealthComponent(maxHealth);
    }

    static void bindUsertype(sol::state& lua)
    {
        lua.new_usertype<HealthComponent>(kUsertypeName,
                                          "current_health", &HealthComponent::currentHealth,
                                          "max_health", &HealthComponent::maxHealth);
    }
};
