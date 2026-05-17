#pragma once

#include <sol/sol.hpp>

#include "Components/SquarePrimitiveComponent.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<SquarePrimitiveComponent>
{
    static constexpr const char* kLuaKey = "square";
    static constexpr const char* kUsertypeName = "square_primitive_component";

    static SquarePrimitiveComponent fromLua(const sol::object& data)
    {
        const auto t = data.as<sol::table>();
        using namespace LuaComponentHelpers;
        const glm::vec2 position = SafeGetVec2(t, "position");
        const auto layer = SafeGetOptionalValue<int>(t, "layer", 1);
        const auto width = SafeGetOptionalValue<float>(t, "width", 0.0f);
        const auto height = SafeGetOptionalValue<float>(t, "height", 0.0f);
        const SDL_Color color = SafeGetColor(t, "color");
        const auto fixed = SafeGetOptionalValue<bool>(t, "fixed", false);
        return SquarePrimitiveComponent(position, layer, width, height, color, fixed);
    }

    static void bindUsertype(sol::state& lua)
    {
        lua.new_usertype<SquarePrimitiveComponent>(kUsertypeName,
                                                   "width", &SquarePrimitiveComponent::width,
                                                   "height", &SquarePrimitiveComponent::height,
                                                   "color", &SquarePrimitiveComponent::color,
                                                   "position", &SquarePrimitiveComponent::position,
                                                   "layer", &SquarePrimitiveComponent::layer,
                                                   "is_fixed", &SquarePrimitiveComponent::isFixed);
    }
};
