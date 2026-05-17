#pragma once

#include <sol/sol.hpp>

#include "Components/UIButtonComponent.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<UIButtonComponent>
{
    static constexpr const char* kLuaKey = "ui_button";
    static constexpr const char* kUsertypeName = "ui_button_component";

    static UIButtonComponent fromLua(const sol::object& data)
    {
        const auto t = data.as<sol::table>();
        using namespace LuaComponentHelpers;
        const bool isActive = SafeGetOptionalValue<bool>(t, "is_active", true);
        const sol::protected_function clickFn = SafeGetProtectedFunction(t, "on_click");
        return UIButtonComponent(isActive, t, clickFn);
    }

    static void bindUsertype(sol::state& lua)
    {
        lua.new_usertype<UIButtonComponent>(kUsertypeName, "is_active", &UIButtonComponent::isActive);
    }
};
