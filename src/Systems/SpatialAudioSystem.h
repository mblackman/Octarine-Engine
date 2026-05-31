#pragma once

#include <SDL3_mixer/SDL_mixer.h>

#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>

#include "../Components/AudioListenerComponent.h"
#include "../Components/AudioSinkComponent.h"
#include "../Components/AudioSourceComponent.h"
#include "../Components/GlobalTransformComponent.h"
#include "../ECS/Iterable.h"
#include "../ECS/Registry.h"

// Per-emitter system: applies distance attenuation + stereo pan to spatial sources each
// frame. Reads the AudioListenerCache singleton populated by UpdateListenerTransformSystem
// — register both AFTER TransformSystem so emitter/listener positions are this-frame
// globals. Non-spatial sources are skipped entirely; their gain stays at whatever
// AudioSystem set from `source.volume`.
class SpatialAudioSystem {
 public:
  void operator()(const ContextFacade& ctx, const GlobalTransformComponent& transform, AudioSourceComponent& source,
                  AudioSinkComponent& sink) {
    if (sink.finished || !sink.track) return;

    auto* registry = ctx.GetRegistry();
    const auto& cache = registry->Get<AudioListenerCache>();

    if (!source.spatial || !cache.valid) {
      // Source isn't (or no longer is) spatial: clear any prior stereo override and let
      // the gain reflect raw source.volume so toggling `spatial` at runtime resets cleanly.
      MIX_SetTrackStereo(sink.track, nullptr);
      MIX_SetTrackGain(sink.track, std::max(0.0f, source.volume));
      return;
    }

    const glm::vec2 rel = transform.position - cache.position;
    const float distance = glm::length(rel);

    const float baseVolume = std::max(0.0f, source.volume);
    const float minDist = std::max(0.0f, source.minDistance);
    const float maxDist = std::max(minDist + 1e-3f, source.maxDistance);
    const float rolloff = std::max(0.0f, cache.rolloff);

    float attenuation;
    if (distance <= minDist) {
      attenuation = 1.0f;
    } else if (distance >= maxDist) {
      attenuation = 0.0f;
    } else {
      const float t = (distance - minDist) / (maxDist - minDist);
      attenuation = 1.0f / (1.0f + rolloff * t);
    }

    const float gain = baseVolume * std::clamp(attenuation, 0.0f, 1.0f);
    MIX_SetTrackGain(sink.track, gain);

    // Pan: project the listener→emitter vector onto the listener's right axis. Center
    // (distance ≈ 0) → equal L/R. Past `maxDist`, gain is already zero so the pan stays
    // applied but inaudible — no extra check needed.
    constexpr float kEpsilon = 1e-4f;
    float pan = 0.0f;
    if (distance > kEpsilon) {
      const float rightDot = glm::dot(rel, cache.right);
      pan = std::clamp(rightDot / distance, -1.0f, 1.0f);
    }

    MIX_StereoGains gains{};
    gains.left = std::clamp(1.0f - pan, 0.0f, 1.0f);
    gains.right = std::clamp(1.0f + pan, 0.0f, 1.0f);
    MIX_SetTrackStereo(sink.track, &gains);
  }
};
