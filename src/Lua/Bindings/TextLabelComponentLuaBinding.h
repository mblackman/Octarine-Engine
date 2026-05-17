#pragma once

#include <sol/sol.hpp>

#include "Components/TextLabelComponent.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<TextLabelComponent>
{
    static constexpr const char* kLuaKey = "text_label";
    static constexpr const char* kUsertypeName = "text_label_component";

    static TextLabelComponent fromLua(const sol::object& data)
    {
        const auto t = data.as<sol::table>();
        using namespace LuaComponentHelpers;
        const glm::vec2 offsetPosition = SafeGetVec2(t, "position");
        const int layer = SafeGetOptionalValue<int>(t, "layer", 1);
        const std::string text = t["text"].get<std::string>();
        const std::string fontId = t["font_id"].get<std::string>();
        const SDL_Color color = SafeGetColor(t, "color");
        const bool isFixed = SafeGetOptionalValue<bool>(t, "is_fixed", true);
        return TextLabelComponent(offsetPosition, layer, text, fontId, color, isFixed);
    }

    static void bindUsertype(sol::state& lua)
    {
        lua.new_usertype<TextLabelComponent>(kUsertypeName,
                                             "text", &TextLabelComponent::text,
                                             "color", &TextLabelComponent::color,
                                             "layer", &TextLabelComponent::layer,
                                             "position", &TextLabelComponent::position,
                                             "is_fixed", &TextLabelComponent::isFixed);
    }
};
