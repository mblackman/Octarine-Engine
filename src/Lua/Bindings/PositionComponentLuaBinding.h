#pragma once

#include <sol/sol.hpp>

#include "Components/PositionComponent.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<PositionComponent> {
  static constexpr const char* kLuaKey = "position";
  static constexpr const char* kUsertypeName = "position_component";

  static PositionComponent fromLua(const sol::object& data) {
    const auto t = data.as<sol::table>();
    return PositionComponent(LuaComponentHelpers::SafeGetVec2(t, "value", 0.0f, 0.0f));
  }

  static void bindUsertype(sol::state& lua) {
    lua.new_usertype<PositionComponent>(kUsertypeName, "value",
                                        sol::property([](const PositionComponent& c) { return c.value; },
                                                      [](PositionComponent& c, const glm::vec2 v) { c.value = v; }));
  }
};
