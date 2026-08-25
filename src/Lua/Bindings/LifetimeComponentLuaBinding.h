#pragma once

#include <sol/sol.hpp>

#include "Components/LifetimeComponent.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<LifetimeComponent> {
  static constexpr const char* kLuaKey = "lifetime";
  static constexpr const char* kUsertypeName = "lifetime_component";

  static LifetimeComponent fromLua(const sol::object& data) {
    if (data.is<float>()) {
      return LifetimeComponent(data.as<float>());
    }
    if (data.is<sol::table>()) {
      const auto t = data.as<sol::table>();
      const auto lifetimeDuration = LuaComponentHelpers::SafeGetOptionalValue<float>(t, "duration", 1.0f);
      return LifetimeComponent(lifetimeDuration);
    }
    return LifetimeComponent(1.0f);
  }

  static void bindUsertype(sol::state& lua) {
    lua.new_usertype<LifetimeComponent>(
        kUsertypeName, "remaining_duration", sol::readonly(&LifetimeComponent::remainingDuration), "duration",
        sol::property([](const LifetimeComponent& s) { return s.lifetimeDuration; },
                      [](LifetimeComponent& s, const float duration) { s.lifetimeDuration = duration; }),
        "is_alive", &LifetimeComponent::IsAlive, "extend", &LifetimeComponent::Extend, "reset",
        &LifetimeComponent::Reset);
  }
};
