#pragma once

#include <sol/sol.hpp>

#include "Components/SpriteComponent.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<SpriteComponent>
{
    static constexpr const char* kLuaKey = "sprite";
    static constexpr const char* kUsertypeName = "sprite_component";

    static SpriteComponent fromLua(const sol::object& data)
    {
        const auto t = data.as<sol::table>();
        using namespace LuaComponentHelpers;
        const std::string textureAssetId = t["texture_asset_id"].get<std::string>();
        const auto width = t["width"].get<float>();
        const auto height = t["height"].get<float>();
        const auto layer = SafeGetOptionalValue<int>(t, "layer", 1);
        const auto fixed = SafeGetOptionalValue<bool>(t, "fixed", false);
        const auto srcRectX = SafeGetOptionalValue<float>(t, "src_rect_x", 0);
        const auto srcRectY = SafeGetOptionalValue<float>(t, "src_rect_y", 0);
        return SpriteComponent(textureAssetId, width, height, layer, fixed, srcRectX, srcRectY);
    }

    static void bindUsertype(sol::state& lua)
    {
        lua.new_usertype<SpriteComponent>(kUsertypeName,
                                          "width", sol::readonly(&SpriteComponent::width),
                                          "height", sol::readonly(&SpriteComponent::height),
                                          "layer", &SpriteComponent::layer,
                                          "asset_id", sol::readonly(&SpriteComponent::assetId));
    }
};
