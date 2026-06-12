#pragma once

#include <sol/sol.hpp>

#include "Components/UIZIndexComponent.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<UIZIndexComponent> {
  static constexpr const char* kLuaKey = "ui_z_index";
  static constexpr const char* kUsertypeName = "ui_z_index_component";

  static UIZIndexComponent fromLua(const sol::object& data) {
    using namespace LuaComponentHelpers;
    if (data.get_type() == sol::type::number) {
      return UIZIndexComponent(data.as<int>());
    }
    const auto t = data.as<sol::table>();
    return UIZIndexComponent(SafeGetOptionalValue<int>(t, "z", 0));
  }

  static void bindUsertype(sol::state& lua) {
    lua.new_usertype<UIZIndexComponent>(kUsertypeName, "z", &UIZIndexComponent::z);
  }
};
