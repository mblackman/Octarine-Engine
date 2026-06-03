#pragma once

#include <SDL3_mixer/SDL_mixer.h>

#include <cstdint>
#include <memory>
#include <vector>

#include "Audio/AudioTrackCache.h"
#include "Components/AudioSinkComponent.h"
#include "Components/AudioSourceComponent.h"
#include "ECS/CommandBuffer.h"
#include "ECS/Registry.h"
#include "EventBus/EventBus.h"
#include "Events/AudioPlayEvent.h"

class AudioSystem {
 public:
  AudioSystem() = default;
  ~AudioSystem() = default;

  AudioSystem(const AudioSystem&) = delete;
  AudioSystem& operator=(const AudioSystem&) = delete;
  AudioSystem(AudioSystem&&) = default;
  AudioSystem& operator=(AudioSystem&&) = default;

  bool Init(Registry* registry, const std::unique_ptr<EventBus>& eventBus);
  void Shutdown();

  [[nodiscard]] MIX_Mixer* Mixer() const { return state_ ? state_->mixer : nullptr; }

  void OnAudioPlay(const AudioPlayEvent& event);

  void operator()(Entity entity, AudioSourceComponent& source);

  CommandBuffer& GetCommandBuffer() { return cmd_buffer_; }

 private:
  struct TrackSlot {
    MIX_Track* track = nullptr;
    std::uint32_t generation = 0;
  };

  struct AudioSystemState {
    MIX_Mixer* mixer = nullptr;
    std::vector<TrackSlot> track_pool;
    int track_pool_max = 0;

    ~AudioSystemState();
  };

  // Returns track + the generation issued for this acquisition. {nullptr, 0} when none free.
  struct Acquired {
    MIX_Track* track = nullptr;
    std::uint32_t generation = 0;
  };
  Acquired AcquireTrack();

  // Per-entity helper for operator(): acquire a pool track, start the clip (honoring loop +
  // resume-from-cull), cache the MIX_Track* in AudioTrackCache, and attach a POD AudioSinkComponent.
  // Split from operator() so each stays a single, readable responsibility.
  void SpawnSinkForSource(Entity entity, AudioSourceComponent& source);

  // True if the cached (track, generation) pair still matches the pool slot — guards against
  // a pool slot being reissued to a different emitter after a track stops.
  [[nodiscard]] bool SinkOwnsTrack(const AudioTrackCache::Entry& entry) const;

  Registry* registry_ = nullptr;
  std::shared_ptr<AudioSystemState> state_;
  CommandBuffer cmd_buffer_;
  EventBus::SubscriptionHandle audio_subscription_;
  // Resolved lazily on first per-entity tick; the cache singleton is Set during bootstrap and
  // never reseated, so the pointer is stable for the registry's lifetime.
  AudioTrackCache* track_cache_ = nullptr;
};
