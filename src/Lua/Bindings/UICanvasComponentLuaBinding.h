#pragma once

#include <sol/sol.hpp>

#include "Components/UICanvasComponent.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<UICanvasComponent> {
  static constexpr const char* kLuaKey = "ui_canvas";
  static constexpr const char* kUsertypeName = "ui_canvas_component";

  static UICanvasComponent fromLua(const sol::object& data) {
    const auto t = data.as<sol::table>();
    using namespace LuaComponentHelpers;
    return UICanvasComponent(
        SafeGetOptionalValue<bool>(t, "is_fixed", true), SafeGetOptionalValue<float>(t, "width", 0.f),
        SafeGetOptionalValue<float>(t, "height", 0.f), SafeGetOptionalValue<int>(t, "base_layer", 0));
  }

  static void bindUsertype(sol::state& lua) {
    lua.new_usertype<UICanvasComponent>(kUsertypeName, "is_fixed", &UICanvasComponent::isFixed, "width",
                                        &UICanvasComponent::width, "height", &UICanvasComponent::height, "base_layer",
                                        &UICanvasComponent::baseLayer);
  }
};
