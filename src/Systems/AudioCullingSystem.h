#pragma once

#include <SDL3_mixer/SDL_mixer.h>

#include <glm/glm.hpp>

#include "../Components/AudioActiveTag.h"
#include "../Components/AudioListenerComponent.h"
#include "../Components/AudioSinkComponent.h"
#include "../Components/AudioSourceComponent.h"
#include "../Components/GlobalTransformComponent.h"
#include "../ECS/CommandBuffer.h"
#include "../ECS/Iterable.h"
#include "../ECS/Registry.h"
#include "../Game/GameConfig.h"

// Per-emitter system: gates spatial sources by listener radius. Inside the radius the
// emitter carries AudioActiveTag and plays normally; outside, the system stashes the
// MIX_Track's playback frame onto AudioSourceComponent, halts the track, and removes the
// AudioSinkComponent so the track returns to the pool for some other emitter to use.
// AudioSystem's "resumingFromCull" path consumes the stored offset on re-entry, so a
// looping clip picks up mid-loop without an audible glitch.
//
// Registered BEFORE SpatialAudioSystem so sinks halted this frame don't make a wasted
// pass through the spatial / Doppler systems. The query is (GlobalTransform, AudioSource)
// — sink is optional because a brand-new spatial source may enter the radius before
// AudioSystem has had a chance to spawn its sink.
class AudioCullingSystem {
 public:
  void operator()(const ContextFacade& ctx, Entity entity, const GlobalTransformComponent& transform,
                  AudioSourceComponent& source) {
    if (!source.spatial) return;

    auto* registry = ctx.GetRegistry();
    if (!cache_) cache_ = &registry->Get<AudioListenerCache>();
    if (!cache_->valid) return;

    const auto& engineOptions = registry->Get<GameConfig>().GetEngineOptions();
    const float radius = engineOptions.audioListenerRadius;
    const float radiusSq = radius * radius;

    const glm::vec2 rel = transform.position - cache_->position;
    const float distSq = glm::dot(rel, rel);
    const bool inRange = distSq <= radiusSq;
    const bool isActive = registry->HasTag<AudioActiveTag>(entity);

    if (inRange && !isActive) {
      cmd_buffer_.AddTag<AudioActiveTag>(entity);
      return;
    }

    if (!inRange && isActive) {
      if (registry->HasComponent<AudioSinkComponent>(entity)) {
        auto& sink = registry->GetComponent<AudioSinkComponent>(entity);
        if (sink.track && !sink.finished) {
          // Capture position BEFORE Stop — MIX_GetTrackPlaybackPosition on a stopped
          // track may return 0 or undefined.
          source.playbackOffsetFrames = MIX_GetTrackPlaybackPosition(sink.track);
          MIX_StopTrack(sink.track, 0);
        }
        cmd_buffer_.RemoveComponent<AudioSinkComponent>(entity);
      }
      cmd_buffer_.RemoveTag<AudioActiveTag>(entity);
    }
  }

  CommandBuffer& GetCommandBuffer() { return cmd_buffer_; }

 private:
  const AudioListenerCache* cache_ = nullptr;
  CommandBuffer cmd_buffer_;
};
