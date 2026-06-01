#pragma once

#include <sol/sol.hpp>

#include "Components/CameraFollowComponent.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<CameraFollowComponent> {
  static constexpr const char* kLuaKey = "camera_follow";
  static constexpr const char* kUsertypeName = "camera_follow_component";

  static CameraFollowComponent fromLua(const sol::object& /*data*/) { return {}; }

  static void bindUsertype(sol::state& lua) { lua.new_usertype<CameraFollowComponent>(kUsertypeName); }
};
