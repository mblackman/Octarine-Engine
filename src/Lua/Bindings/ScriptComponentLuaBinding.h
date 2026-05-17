#pragma once

#include <sol/sol.hpp>

#include "Components/ScriptComponent.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<ScriptComponent>
{
    static constexpr const char* kLuaKey = "script";
    static constexpr const char* kUsertypeName = "script_component";

    static ScriptComponent fromLua(const sol::object& data)
    {
        const auto t = data.as<sol::table>();
        using namespace LuaComponentHelpers;
        const sol::protected_function updateFn = SafeGetProtectedFunction(t, "on_update");
        const sol::protected_function onDebugGuiFn = SafeGetProtectedFunction(t, "on_debug_gui");
        return ScriptComponent(t, updateFn, onDebugGuiFn);
    }

    static void bindUsertype(sol::state& lua)
    {
        lua.new_usertype<ScriptComponent>(kUsertypeName, "data", &ScriptComponent::scriptTable);
    }
};
