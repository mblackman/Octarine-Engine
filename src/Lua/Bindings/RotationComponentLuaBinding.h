#pragma once

#include <sol/sol.hpp>

#include "Components/RotationComponent.h"
#include "General/AngleUnit.h"
#include "General/Logger.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<RotationComponent> {
  static constexpr const char* kLuaKey = "rotation";
  static constexpr const char* kUsertypeName = "rotation_component";

  static RotationComponent fromLua(const sol::object& data) {
    if (data.is<float>()) return RotationComponent(octarine::AngleUnits::ToRadians(data.as<float>()));
    if (data.is<sol::table>()) {
      const auto t = data.as<sol::table>();
      // Omitting `pivot` keeps the centre anchor, which is how sprites rotated before pivots
      // were authorable — existing games must not shift.
      if (t["pivot"].valid()) {
        Logger::Warn(
            "rotation.pivot is no longer read: the anchor moved to its own `pivot` component, so "
            "rotation and scale can share it. Author `pivot = { x = , y = }` alongside `rotation`.");
      }
      return RotationComponent(
          octarine::AngleUnits::ToRadians(LuaComponentHelpers::SafeGetOptionalValue<float>(t, "value", 0.0F)));
    }
    return RotationComponent(0.0F);
  }

  static void bindUsertype(sol::state& lua) {
    // A property, not a direct member binding, so reads and writes convert against AngleUnit.
    lua.new_usertype<RotationComponent>(
        kUsertypeName, "value",
        sol::property([](const RotationComponent& r) { return octarine::AngleUnits::ToAuthored(r.value); },
                      [](RotationComponent& r, const float v) { r.value = octarine::AngleUnits::ToRadians(v); }));
  }
};
