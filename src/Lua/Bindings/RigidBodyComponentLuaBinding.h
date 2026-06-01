#pragma once

#include <sol/sol.hpp>

#include "Components/RigidBodyComponent.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<RigidBodyComponent> {
  static constexpr const char* kLuaKey = "rigidbody";
  static constexpr const char* kUsertypeName = "rigidbody_component";

  static RigidBodyComponent fromLua(const sol::object& data) {
    const auto t = data.as<sol::table>();
    return RigidBodyComponent(LuaComponentHelpers::SafeGetVec2(t, "velocity"));
  }

  static void bindUsertype(sol::state& lua) {
    lua.new_usertype<RigidBodyComponent>(kUsertypeName, "velocity", &RigidBodyComponent::velocity);
  }
};
