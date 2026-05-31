#pragma once

#include <SDL3_mixer/SDL_mixer.h>

#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>

#include "../Components/AudioListenerComponent.h"
#include "../Components/AudioSinkComponent.h"
#include "../Components/AudioSourceComponent.h"
#include "../Components/GlobalTransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../ECS/Iterable.h"
#include "../ECS/Registry.h"

// Per-emitter system: applies Doppler shift via MIX_SetTrackFrequencyRatio. Registered on
// the (GlobalTransform, RigidBody, AudioSource, AudioSink) archetype — a doppler-flagged
// source on an entity without a RigidBodyComponent is silently ignored (no velocity, no
// shift). Runs alongside SpatialAudioSystem; the two are independent: spatial governs
// gain + pan, this governs frequency ratio. Toggling `source.doppler` false at runtime
// resets the ratio to 1.0 on the next frame.
//
// SDL3_mixer's MIX_Track exposes a built-in per-track frequency ratio (added in the
// MIX_Track API revision). The original plan called for a Mix_RegisterEffect / postmix
// resampler or an SDL_AudioStream rewrite — both unnecessary against this surface.
//
// Cache pointer is hoisted to a member (mirrors SpatialAudioSystem) so the per-emitter
// callback avoids a Registry::Get<AudioListenerCache>() hashmap lookup per call.
// MIX_SetTrackFrequencyRatio fires only when the computed ratio differs from
// sink.lastRatio so an unchanged ratio (the steady-state case) costs zero mixer calls.
class DopplerSystem {
 public:
  void operator()(const ContextFacade& ctx, const GlobalTransformComponent& transform,
                  const RigidBodyComponent& rigidBody, AudioSourceComponent& source, AudioSinkComponent& sink) {
    if (sink.finished || !sink.track) return;

    if (!cache_) cache_ = &ctx.GetRegistry()->Get<AudioListenerCache>();
    const auto& cache = *cache_;

    const float ratio = ComputeRatio(transform, rigidBody, source, cache);
    if (ratio != sink.lastRatio) {
      MIX_SetTrackFrequencyRatio(sink.track, ratio);
      sink.lastRatio = ratio;
    }
  }

 private:
  static float ComputeRatio(const GlobalTransformComponent& transform, const RigidBodyComponent& rigidBody,
                            const AudioSourceComponent& source, const AudioListenerCache& cache) {
    if (!source.doppler || !cache.valid) return 1.0f;

    const float c = std::max(1.0f, cache.speedOfSound);
    const float factor = std::max(0.0f, cache.dopplerFactor);
    if (factor <= 0.0f) return 1.0f;

    const glm::vec2 rel = transform.position - cache.position;
    constexpr float kEpsilon = 1e-4f;
    const float distance = glm::length(rel);
    if (distance <= kEpsilon) return 1.0f;
    const glm::vec2 dir = rel / distance;

    // Radial components: positive = receding from listener, negative = approaching.
    const float vsRadial = glm::dot(rigidBody.velocity, dir);
    const float vlRadial = glm::dot(cache.velocity, dir);

    // Clamp denominator away from zero to avoid the Mach-1 singularity (object moving
    // toward listener at the speed of sound). Cap ratio to a sane musical range so an
    // exaggerated dopplerFactor doesn't yank a track into ultrasonic / DC territory.
    const float denom = std::max(c - vsRadial * factor, c * 0.01f);
    const float numer = c + vlRadial * factor;
    return std::clamp(numer / denom, 0.25f, 4.0f);
  }

  const AudioListenerCache* cache_ = nullptr;
};
