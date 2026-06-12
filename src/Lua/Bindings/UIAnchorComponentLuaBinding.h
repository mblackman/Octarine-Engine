#pragma once

#include <sol/sol.hpp>

#include "Components/UIAnchorComponent.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<UIAnchorComponent> {
  static constexpr const char* kLuaKey = "ui_anchor";
  static constexpr const char* kUsertypeName = "ui_anchor_component";

  static UIAnchorComponent fromLua(const sol::object& data) {
    const auto t = data.as<sol::table>();
    using namespace LuaComponentHelpers;
    return UIAnchorComponent(
        SafeGetOptionalValue<float>(t, "left", 0.f), SafeGetOptionalValue<float>(t, "top", 0.f),
        SafeGetOptionalValue<float>(t, "right", 1.f), SafeGetOptionalValue<float>(t, "bottom", 1.f),
        SafeGetOptionalValue<float>(t, "offset_left", 0.f), SafeGetOptionalValue<float>(t, "offset_top", 0.f),
        SafeGetOptionalValue<float>(t, "offset_right", 0.f), SafeGetOptionalValue<float>(t, "offset_bottom", 0.f));
  }

  static void bindUsertype(sol::state& lua) {
    lua.new_usertype<UIAnchorComponent>(
        kUsertypeName, "anchor_left", &UIAnchorComponent::anchorLeft, "anchor_top", &UIAnchorComponent::anchorTop,
        "anchor_right", &UIAnchorComponent::anchorRight, "anchor_bottom", &UIAnchorComponent::anchorBottom,
        "offset_left", &UIAnchorComponent::offsetLeft, "offset_top", &UIAnchorComponent::offsetTop, "offset_right",
        &UIAnchorComponent::offsetRight, "offset_bottom", &UIAnchorComponent::offsetBottom);
  }
};
