#pragma once

#include <algorithm>
#include <sol/sol.hpp>

#include "Components/HealthComponent.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<HealthComponent> {
  static constexpr const char* kLuaKey = "health";
  static constexpr const char* kUsertypeName = "health_component";

  static HealthComponent fromLua(const sol::object& data) {
    const auto t = data.as<sol::table>();
    using namespace LuaComponentHelpers;
    const int maxHealth = SafeGetOptionalValue<int>(t, "max_health", 100);
    return HealthComponent(maxHealth);
  }

  static void bindUsertype(sol::state& lua) {
    // current_health is a clamping property — direct writes from Lua are clamped to
    // [0, maxHealth] so scripts can't bypass damage()/heal() invariants by assigning the
    // field. Reads stay zero-overhead.
    lua.new_usertype<HealthComponent>(
        kUsertypeName, "current_health",
        sol::property([](const HealthComponent& h) { return h.currentHealth; },
                      [](HealthComponent& h, const int v) { h.currentHealth = std::clamp(v, 0, h.maxHealth); }),
        "max_health",
        sol::property([](const HealthComponent& h) { return h.maxHealth; },
                      [](HealthComponent& h, const int v) { h.maxHealth = v; }),
        "damage", &HealthComponent::Damage, "heal", &HealthComponent::Heal, "is_dead",
        sol::property(&HealthComponent::IsDead), "fraction", sol::property(&HealthComponent::Fraction));
  }
};
