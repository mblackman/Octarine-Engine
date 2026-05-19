#pragma once

#include <sol/sol.hpp>

#include "Components/RotationComponent.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<RotationComponent>
{
    static constexpr const char* kLuaKey = "rotation";
    static constexpr const char* kUsertypeName = "rotation_component";

    static RotationComponent fromLua(const sol::object& data)
    {
        if (data.is<double>()) return RotationComponent(data.as<double>());
        if (data.is<sol::table>())
        {
            const auto t = data.as<sol::table>();
            return RotationComponent(LuaComponentHelpers::SafeGetOptionalValue<double>(t, "value", 0.0));
        }
        return RotationComponent(0.0);
    }

    static void bindUsertype(sol::state& lua)
    {
        lua.new_usertype<RotationComponent>(kUsertypeName, "value", &RotationComponent::value);
    }
};
