#pragma once

#include <sol/sol.hpp>

#include "Components/PivotComponent.h"
#include "General/Logger.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<PivotComponent> {
  static constexpr const char* kLuaKey = "pivot";
  static constexpr const char* kUsertypeName = "pivot_component";

  static PivotComponent fromLua(const sol::object& data) {
    if (data.is<sol::table>()) {
      const auto t = data.as<sol::table>();
      // Flat { x = , y = } is the primary shape, matching the one it has inside the transform
      // table. `{ value = { x = , y = } }` is accepted too: every other vec2-valued component is
      // authored that way standalone, and reasoning by analogy from `scale` should not fail
      // silently into the default anchor.
      if (t["value"].valid()) {
        return PivotComponent(
            LuaComponentHelpers::SafeGetVec2(t, "value", PivotComponent::kDefaultX, PivotComponent::kDefaultY));
      }
      return PivotComponent(LuaComponentHelpers::SafeGetOptionalValue<float>(t, "x", PivotComponent::kDefaultX),
                            LuaComponentHelpers::SafeGetOptionalValue<float>(t, "y", PivotComponent::kDefaultY));
    }
    if (data.is<float>()) {
      const auto uniform = data.as<float>();
      return PivotComponent(uniform, uniform);
    }
    Logger::Warn(
        "pivot component expects { x = , y = }, { value = { x = , y = } } or a bare number; "
        "got something else. Defaulting to the centre anchor.");
    return PivotComponent();
  }

  static void bindUsertype(sol::state& lua) {
    lua.new_usertype<PivotComponent>(kUsertypeName, "value", &PivotComponent::value, "x",
                                     sol::property(&PivotComponent::get_x, &PivotComponent::set_x), "y",
                                     sol::property(&PivotComponent::get_y, &PivotComponent::set_y));
  }
};
