#pragma once

#include <sol/sol.hpp>

#include "Components/ScaleComponent.h"
#include "General/Logger.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<ScaleComponent> {
  static constexpr const char* kLuaKey = "scale";
  static constexpr const char* kUsertypeName = "scale_component";

  static ScaleComponent fromLua(const sol::object& data) {
    float x = 1.0f;
    float y = 1.0f;

    if (data.is<sol::table>()) {
      const auto t = data.as<sol::table>();
      const auto vec = LuaComponentHelpers::SafeGetVec2(t, "value", 1.0f, 1.0f);
      x = vec.x;
      y = vec.y;
    } else if (data.is<float>()) {
      const auto val = data.as<float>();
      x = val;
      y = val;
    } else {
      // Silently defaulting to (1, 1) here reads as "my scale did nothing" with nothing to grep
      // for, so name the bad value instead.
      Logger::Warn(
          "scale component expects { value = { x = , y = } } or a bare number; got something else. "
          "Defaulting to (1, 1).");
    }

    return ScaleComponent(x, y);
  }

  static void bindUsertype(sol::state& lua) {
    // `scale` is the uniform-scale view: it reads the x axis, so ask is_uniform() before trusting
    // it on an entity that may have been scaled per-axis.
    lua.new_usertype<ScaleComponent>(kUsertypeName, "value", &ScaleComponent::value, "x",
                                     sol::property(&ScaleComponent::get_x, &ScaleComponent::set_x), "y",
                                     sol::property(&ScaleComponent::get_y, &ScaleComponent::set_y), "scale",
                                     sol::property(&ScaleComponent::get_scale, &ScaleComponent::set_scale),
                                     "is_uniform", &ScaleComponent::is_uniform);
  }
};
