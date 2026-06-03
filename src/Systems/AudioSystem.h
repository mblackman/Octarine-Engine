#pragma once

#include <SDL3_mixer/SDL_mixer.h>

#include <cstdint>
#include <memory>
#include <vector>

#include "../Components/AudioSinkComponent.h"
#include "../Components/AudioSourceComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/AudioPlayEvent.h"
#include "ECS/CommandBuffer.h"
#include "ECS/Registry.h"

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

  // True if the sink's (track, generation) pair still matches the pool slot — guards against
  // a pool slot being reissued to a different emitter after a track stops.
  [[nodiscard]] bool SinkOwnsTrack(const AudioSinkComponent& sink) const;

  Registry* registry_ = nullptr;
  std::shared_ptr<AudioSystemState> state_;
  CommandBuffer cmd_buffer_;
  EventBus::SubscriptionHandle audio_subscription_;
};
