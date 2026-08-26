#pragma once

#include <sol/sol.hpp>

#include "Components/RotationComponent.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<RotationComponent> {
  static constexpr const char* kLuaKey = "rotation";
  static constexpr const char* kUsertypeName = "rotation_component";

  static RotationComponent fromLua(const sol::object& data) {
    if (data.is<double>()) return RotationComponent(data.as<double>());
    if (data.is<sol::table>()) {
      const auto t = data.as<sol::table>();
      // Omitting `pivot` keeps the centre anchor, which is how sprites rotated before pivots
      // were authorable — existing games must not shift.
      return RotationComponent(LuaComponentHelpers::SafeGetOptionalValue<double>(t, "value", 0.0),
                               LuaComponentHelpers::SafeGetVec2(t, "pivot", RotationComponent::kDefaultPivotX,
                                                                RotationComponent::kDefaultPivotY));
    }
    return RotationComponent(0.0);
  }

  static void bindUsertype(sol::state& lua) {
    // pivotOffset is derived state owned by PivotResolveSystem and is deliberately not exposed.
    lua.new_usertype<RotationComponent>(kUsertypeName, "value", &RotationComponent::value, "pivot",
                                        &RotationComponent::pivot);
  }
};
