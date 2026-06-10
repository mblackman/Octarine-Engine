#pragma once

#include <cstdint>
#include <sol/sol.hpp>
#include <string>
#include <string_view>

#include "Components/SpriteComponent.h"
#include "General/BlendMode.h"
#include "General/Constants.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<SpriteComponent> {
  static constexpr const char* kLuaKey = "sprite";
  static constexpr const char* kUsertypeName = "sprite_component";

  static SpriteComponent fromLua(const sol::object& data) {
    const auto t = data.as<sol::table>();
    using namespace LuaComponentHelpers;
    const std::string textureAssetId = t["texture_asset_id"].get<std::string>();
    const auto width = t["width"].get<float>();
    const auto height = t["height"].get<float>();
    const auto layer = SafeGetOptionalValue<int>(t, "layer", 1);
    const auto fixed = SafeGetOptionalValue<bool>(t, "fixed", false);
    const auto srcRectX = SafeGetOptionalValue<float>(t, "src_rect_x", 0);
    const auto srcRectY = SafeGetOptionalValue<float>(t, "src_rect_y", 0);
    SpriteComponent sprite(textureAssetId, width, height, layer, fixed, srcRectX, srcRectY);
    sprite.colorMod = SafeGetColor(t, "color_mod", Constants::kUint8Max, Constants::kUint8Max, Constants::kUint8Max,
                                   Constants::kUint8Max);
    // "alpha" is a convenience alias for color_mod.a; when both are given, alpha wins.
    sprite.colorMod.a =
        static_cast<std::uint8_t>(SafeGetOptionalValue<int>(t, "alpha", static_cast<int>(sprite.colorMod.a)));
    sprite.blendMode = octarine::BlendModeFromString(SafeGetOptionalValue<std::string>(t, "blend_mode", "blend"));
    return sprite;
  }

  static void bindUsertype(sol::state& lua) {
    lua.new_usertype<SpriteComponent>(
        kUsertypeName, "width", sol::readonly(&SpriteComponent::width), "height",
        sol::readonly(&SpriteComponent::height), "layer", &SpriteComponent::layer, "asset_id",
        sol::readonly(&SpriteComponent::assetId), "color_mod", &SpriteComponent::colorMod, "alpha",
        sol::property([](const SpriteComponent& s) { return s.colorMod.a; },
                      [](SpriteComponent& s, const std::uint8_t alpha) { s.colorMod.a = alpha; }),
        "blend_mode",
        sol::property([](const SpriteComponent& s) { return octarine::ToString(s.blendMode); },
                      [](SpriteComponent& s, const std::string_view mode) {
                        s.blendMode = octarine::BlendModeFromString(mode, s.blendMode);
                      }));
  }
};
