#pragma once

#include <sol/sol.hpp>

#include "Components/BoxColliderComponent.h"
#include "General/Constants.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<BoxColliderComponent>
{
    static constexpr const char* kLuaKey = "box_collider";
    static constexpr const char* kUsertypeName = "box_collider_component";

    static BoxColliderComponent fromLua(const sol::object& data)
    {
        const auto t = data.as<sol::table>();
        using namespace LuaComponentHelpers;
        const int width = SafeGetOptionalValue<int>(t, "width", 1);
        const int height = SafeGetOptionalValue<int>(t, "height", 1);
        const glm::vec2 offset = SafeGetVec2(t, "offset");
        const bool isFixed = SafeGetOptionalValue<bool>(t, "is_fixed", false);
        const auto collisionMask = SafeGetOptionalValue<int>(t, "collision_mask", Constants::kDefaultEntityMask);
        const auto collisionMaskBits = EntityMask(static_cast<unsigned long long>(collisionMask));
        return BoxColliderComponent(width, height, offset, isFixed, collisionMaskBits);
    }

    static void bindUsertype(sol::state& lua)
    {
        lua.new_usertype<BoxColliderComponent>(kUsertypeName,
                                               "width", &BoxColliderComponent::width,
                                               "height", &BoxColliderComponent::height,
                                               "is_fixed", &BoxColliderComponent::isFixed,
                                               "offset", &BoxColliderComponent::offset);
    }
};
