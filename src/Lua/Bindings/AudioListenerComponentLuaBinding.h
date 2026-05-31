#pragma once

#include <sol/sol.hpp>

#include "Components/AudioListenerComponent.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<AudioListenerComponent>
{
    static constexpr const char* kLuaKey = "audio_listener";
    static constexpr const char* kUsertypeName = "audio_listener_component";

    static AudioListenerComponent fromLua(const sol::object& data)
    {
        using namespace LuaComponentHelpers;
        if (!data.is<sol::table>()) return {};
        const auto t = data.as<sol::table>();
        const float maxDistance = SafeGetOptionalValue<float>(t, "max_distance", 1000.0f);
        const float rolloff = SafeGetOptionalValue<float>(t, "rolloff", 1.0f);
        return AudioListenerComponent(maxDistance, rolloff);
    }

    static void bindUsertype(sol::state& lua)
    {
        lua.new_usertype<AudioListenerComponent>(kUsertypeName,
                                                 "max_distance", &AudioListenerComponent::maxDistance,
                                                 "rolloff", &AudioListenerComponent::rolloff);
    }
};
