#pragma once

#include <sol/sol.hpp>

#include "Components/LifetimeComponent.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<LifetimeComponent> {
  static constexpr const char* kLuaKey = "lifetime";
  static constexpr const char* kUsertypeName = "lifetime_component";

  static LifetimeComponent fromLua(const sol::object& data) {
    const auto t = data.as<sol::table>();
    using namespace LuaComponentHelpers;
    const float lifetimeDuration = t["duration"].get<float>();
    return LifetimeComponent(lifetimeDuration);
  }

  static void bindUsertype(sol::state& lua) {
    lua.new_usertype<LifetimeComponent>(
        kUsertypeName, "remaining_duration", sol::readonly(&LifetimeComponent::remainingDuration), "duration",
        sol::property([](const LifetimeComponent& s) { return s.lifetimeDuration; },
                      [](LifetimeComponent& s, const float duration) { s.lifetimeDuration = duration; }));
  }
};
