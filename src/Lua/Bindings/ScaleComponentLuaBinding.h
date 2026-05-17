#pragma once

#include <sol/sol.hpp>

#include "Components/ScaleComponent.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<ScaleComponent>
{
    static constexpr const char* kLuaKey = "scale";
    static constexpr const char* kUsertypeName = "scale_component";

    static ScaleComponent fromLua(const sol::object& data)
    {
        const auto t = data.as<sol::table>();
        return ScaleComponent(LuaComponentHelpers::SafeGetVec2(t, "value", 1.0f, 1.0f));
    }

    static void bindUsertype(sol::state& lua)
    {
        lua.new_usertype<ScaleComponent>(kUsertypeName, "value", &ScaleComponent::value);
    }
};
