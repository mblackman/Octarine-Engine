#pragma once

#include <sol/sol.hpp>

#include "Components/AudioSourceComponent.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<AudioSourceComponent>
{
    static constexpr const char* kLuaKey = "audio_source";
    static constexpr const char* kUsertypeName = "audio_source_component";

    static AudioSourceComponent fromLua(const sol::object& data)
    {
        const auto t = data.as<sol::table>();
        using namespace LuaComponentHelpers;
        const std::string clipId = t["clip_id"].get<std::string>();
        const float volume = SafeGetOptionalValue<float>(t, "volume", 1.0f);
        const float pitch = SafeGetOptionalValue<float>(t, "pitch", 1.0f);
        const bool loop = SafeGetOptionalValue<bool>(t, "loop", false);
        const bool playOnSpawn = SafeGetOptionalValue<bool>(t, "play_on_spawn", true);
        const bool despawnOnFinish = SafeGetOptionalValue<bool>(t, "despawn_on_finish", false);
        const bool spatial = SafeGetOptionalValue<bool>(t, "spatial", false);
        const float minDistance = SafeGetOptionalValue<float>(t, "min_distance", 50.0f);
        const float maxDistance = SafeGetOptionalValue<float>(t, "max_distance", 1000.0f);
        return AudioSourceComponent(clipId, volume, pitch, loop, playOnSpawn, despawnOnFinish, spatial, minDistance,
                                    maxDistance);
    }

    static void bindUsertype(sol::state& lua)
    {
        lua.new_usertype<AudioSourceComponent>(kUsertypeName,
                                               "clip_id", &AudioSourceComponent::clipId,
                                               "volume", &AudioSourceComponent::volume,
                                               "pitch", &AudioSourceComponent::pitch,
                                               "loop", &AudioSourceComponent::loop,
                                               "play_on_spawn", &AudioSourceComponent::playOnSpawn,
                                               "despawn_on_finish", &AudioSourceComponent::despawnOnFinish,
                                               "spatial", &AudioSourceComponent::spatial,
                                               "min_distance", &AudioSourceComponent::minDistance,
                                               "max_distance", &AudioSourceComponent::maxDistance);
    }
};
