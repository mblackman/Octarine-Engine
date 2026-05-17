#pragma once

#include <sol/sol.hpp>

#include "Components/AnimationComponent.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<AnimationComponent>
{
    static constexpr const char* kLuaKey = "animation";
    static constexpr const char* kUsertypeName = "animation_component";

    static AnimationComponent fromLua(const sol::object& data)
    {
        const auto t = data.as<sol::table>();
        using namespace LuaComponentHelpers;
        const int numberFrames = SafeGetOptionalValue<int>(t, "num_frames", 1);
        const int speedRate = SafeGetOptionalValue<int>(t, "speed_rate", 1);
        return AnimationComponent(numberFrames, speedRate);
    }

    static void bindUsertype(sol::state& lua)
    {
        lua.new_usertype<AnimationComponent>(kUsertypeName,
                                             "num_frames", &AnimationComponent::numFrames,
                                             "current_frame", &AnimationComponent::currentFrame,
                                             "frame_rate_speed", &AnimationComponent::frameRateSpeed,
                                             "should_loop", &AnimationComponent::shouldLoop,
                                             "is_finished", &AnimationComponent::isFinished);
    }
};
