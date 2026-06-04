#pragma once

#include <sol/sol.hpp>

#include "Components/ProjectileEmitterComponent.h"
#include "General/Constants.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<ProjectileEmitterComponent> {
  static constexpr const char* kLuaKey = "projectile_emitter";
  static constexpr const char* kUsertypeName = "projectile_emitter_component";

  static ProjectileEmitterComponent fromLua(const sol::object& data) {
    const auto t = data.as<sol::table>();
    using namespace LuaComponentHelpers;
    const glm::vec2 projectileVelocity = SafeGetVec2(t, "projectile_velocity");
    // repeat_frequency drives the engine ProjectileEmitSystem auto-fire tick (interval in seconds).
    // Defaults to 0 == no auto-fire, so an emitter only auto-fires when a scene opts in with a
    // positive value; omitting it leaves firing to manual fire_projectile / scripted logic.
    const auto repeatFrequency = SafeGetOptionalValue<float>(t, "repeat_frequency", 0.0f);
    const auto projectileDuration = SafeGetOptionalValue<float>(t, "projectile_duration", 1.0f);
    const int projectileDamage = SafeGetOptionalValue<int>(t, "hit_damage", 10);
    const auto collisionMask = SafeGetOptionalValue<int>(t, "collision_mask", Constants::kDefaultEntityMask);
    const auto collisionMaskBits = EntityMask(static_cast<unsigned long long>(collisionMask));
    const auto projectileMask = SafeGetOptionalValue<int>(t, "projectile_mask", 0);
    const auto projectileMaskBits = EntityMask(static_cast<unsigned long long>(projectileMask));
    const auto projectileName = SafeGetOptionalValue<std::string>(t, "projectile_name", std::string{});

    auto component =
        ProjectileEmitterComponent(projectileVelocity, projectileDuration, repeatFrequency, projectileDamage,
                                   collisionMaskBits, projectileMaskBits, projectileName);

    // Optional override for the initial countdown timer. Defaults to repeat_frequency
    // (the C++ constructor default). Stress tests use this to stagger emitter fire times
    // so they don't all spawn projectiles on the same frame (thundering-herd).
    component.countDownTimer = SafeGetOptionalValue<float>(t, "countdown_timer", repeatFrequency);
    return component;
  }

  static void bindUsertype(sol::state& lua) {
    lua.new_usertype<ProjectileEmitterComponent>(
        kUsertypeName, "velocity", &ProjectileEmitterComponent::velocity, "duration",
        &ProjectileEmitterComponent::duration, "frequency", &ProjectileEmitterComponent::frequency, "damage",
        &ProjectileEmitterComponent::damage, "countdown_timer", &ProjectileEmitterComponent::countDownTimer,
        "projectile_name", &ProjectileEmitterComponent::projectileName);
  }
};
