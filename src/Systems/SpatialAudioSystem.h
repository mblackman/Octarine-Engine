#pragma once

#include <SDL3_mixer/SDL_mixer.h>

#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>

#include "Audio/AudioTrackCache.h"
#include "Components/AudioListenerComponent.h"
#include "Components/AudioSinkComponent.h"
#include "Components/AudioSourceComponent.h"
#include "Components/GlobalTransformComponent.h"
#include "ECS/Iterable.h"
#include "ECS/Registry.h"

// Per-emitter system: applies distance attenuation + stereo pan to spatial sources each
// frame. Reads the AudioListenerCache singleton populated by UpdateListenerTransformSystem
// — register both AFTER TransformSystem so emitter/listener positions are this-frame
// globals. Non-spatial sources are skipped entirely; their gain stays at whatever
// AudioSystem set from `source.volume`.
//
// Cache pointer is hoisted to a member so the per-emitter callback does not hit
// Registry::Get<AudioListenerCache>() (typeid-keyed map lookup) on every call. The
// AudioListenerCache singleton is Set once during Game::Setup and never reseated, so
// the pointer is stable for the registry's lifetime.
class SpatialAudioSystem {
 public:
  void operator()(const ContextFacade& ctx, Entity entity, const GlobalTransformComponent& transform,
                  AudioSourceComponent& source, AudioSinkComponent& sink) {
    if (sink.finished) return;

    // cache_ and track_cache_ are both Set once during bootstrap and never reseated, so resolving
    // them together on the first tick keeps the pointers stable for the registry's lifetime.
    if (!cache_) {
      cache_ = &ctx.GetRegistry()->Get<AudioListenerCache>();
      track_cache_ = &ctx.GetRegistry()->Get<AudioTrackCache>();
    }
    const auto& cache = *cache_;

    MIX_Track* track = track_cache_->Track(entity);
    if (!track) return;

    if (!source.spatial || !cache.valid) {
      ApplyNonSpatial(track, source, sink);
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
    if (gain != sink.lastGain) {
      MIX_SetTrackGain(track, gain);
      sink.lastGain = gain;
    }

    // Pan: project the listener→emitter vector onto the listener's right axis. Center
    // (distance ≈ 0) → equal L/R. Past `maxDist`, gain is already zero so the pan stays
    // applied but inaudible — no extra check needed.
    constexpr float kEpsilon = 1e-4f;
    float pan = 0.0f;
    if (distance > kEpsilon) {
      const float rightDot = glm::dot(rel, cache.right);
      pan = std::clamp(rightDot / distance, -1.0f, 1.0f);
    }

    if (!sink.stereoApplied || pan != sink.lastPan) {
      MIX_StereoGains gains{};
      gains.left = std::clamp(1.0f - pan, 0.0f, 1.0f);
      gains.right = std::clamp(1.0f + pan, 0.0f, 1.0f);
      MIX_SetTrackStereo(track, &gains);
      sink.stereoApplied = true;
      sink.lastPan = pan;
    }
  }

 private:
  // Source isn't (or no longer is) spatial: clear any prior stereo override once on the transition
  // and let the gain reflect raw source.volume. Subsequent frames skip both calls when unchanged.
  static void ApplyNonSpatial(MIX_Track* track, const AudioSourceComponent& source, AudioSinkComponent& sink) {
    if (sink.stereoApplied) {
      MIX_SetTrackStereo(track, nullptr);
      sink.stereoApplied = false;
      sink.lastPan = AudioSinkComponent::kPanSentinel;
    }
    const float gain = std::max(0.0f, source.volume);
    if (gain != sink.lastGain) {
      MIX_SetTrackGain(track, gain);
      sink.lastGain = gain;
    }
  }

  const AudioListenerCache* cache_ = nullptr;
  AudioTrackCache* track_cache_ = nullptr;
};
