#pragma once

#include <SDL3_mixer/SDL_mixer.h>

#include <memory>
#include <vector>

#include "../EventBus/EventBus.h"
#include "../Events/AudioPlayEvent.h"
#include "ECS/Registry.h"

class AudioSystem {
 public:
  AudioSystem() = default;
  ~AudioSystem();

  AudioSystem(const AudioSystem&) = delete;
  AudioSystem& operator=(const AudioSystem&) = delete;
  AudioSystem(AudioSystem&&) = default;
  AudioSystem& operator=(AudioSystem&&) = delete;

  bool Init(Registry* registry, const std::unique_ptr<EventBus>& eventBus, int trackPoolSize = 32);
  void Shutdown();

  [[nodiscard]] MIX_Mixer* Mixer() const { return mixer_; }

  void OnAudioPlay(const AudioPlayEvent& event);

 private:
  MIX_Track* AcquireTrack();

  Registry* registry_ = nullptr;
  MIX_Mixer* mixer_ = nullptr;
  std::vector<MIX_Track*> track_pool_;
};
