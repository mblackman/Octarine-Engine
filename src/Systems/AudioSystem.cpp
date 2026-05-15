#include "AudioSystem.h"

#include <SDL3/SDL.h>

#include <algorithm>
#include <string>

#include "../AssetManager/AssetManager.h"
#include "../Game/GameConfig.h"
#include "../General/Logger.h"

AudioSystem::~AudioSystem() { Shutdown(); }

bool AudioSystem::Init(Registry* registry, const std::unique_ptr<EventBus>& eventBus, const int trackPoolSize) {
  registry_ = registry;

  if (!MIX_Init()) {
    Logger::Error("MIX_Init failed: " + std::string(SDL_GetError()));
    return false;
  }

  mixer_ = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
  if (!mixer_) {
    Logger::Error("MIX_CreateMixerDevice failed: " + std::string(SDL_GetError()));
    MIX_Quit();
    return false;
  }

  track_pool_.reserve(static_cast<size_t>(trackPoolSize));
  for (int i = 0; i < trackPoolSize; ++i) {
    if (MIX_Track* track = MIX_CreateTrack(mixer_)) {
      track_pool_.push_back(track);
    } else {
      Logger::Warn("MIX_CreateTrack failed at slot " + std::to_string(i) + ": " + std::string(SDL_GetError()));
      break;
    }
  }

  registry_->Set<MIX_Mixer*>(mixer_);

  eventBus->SubscribeEvent<AudioSystem, AudioPlayEvent>(this, &AudioSystem::OnAudioPlay);

  Logger::Info("AudioSystem initialized; track pool size = " + std::to_string(track_pool_.size()));
  return true;
}

void AudioSystem::Shutdown() {
  for (MIX_Track* track : track_pool_) {
    if (track) MIX_DestroyTrack(track);
  }
  track_pool_.clear();

  if (mixer_) {
    MIX_DestroyMixer(mixer_);
    mixer_ = nullptr;
    MIX_Quit();
  }
}

MIX_Track* AudioSystem::AcquireTrack() {
  for (MIX_Track* track : track_pool_) {
    if (track && !MIX_TrackPlaying(track)) {
      return track;
    }
  }
  return nullptr;
}

void AudioSystem::OnAudioPlay(const AudioPlayEvent& event) {
  if (!mixer_ || !registry_) return;

  const auto& engineOptions = registry_->Get<GameConfig>().GetEngineOptions();
  if (!engineOptions.audioEnabled) return;

  auto& assetManager = registry_->Get<AssetManager>();
  MIX_Audio* clip = assetManager.GetAudioClip(event.clipId);
  if (!clip) return;

  const float gain = std::max(0.0F, event.volume * engineOptions.masterVolume);

  MIX_Track* track = AcquireTrack();
  if (track) {
    if (!MIX_SetTrackAudio(track, clip)) {
      Logger::Warn("MIX_SetTrackAudio failed for " + event.clipId + ": " + std::string(SDL_GetError()));
      return;
    }
    MIX_SetTrackGain(track, gain);
    if (!MIX_PlayTrack(track, 0)) {
      Logger::Warn("MIX_PlayTrack failed for " + event.clipId + ": " + std::string(SDL_GetError()));
    }
    return;
  }

  Logger::Warn("Audio track pool exhausted; falling back to fire-and-forget for " + event.clipId);
  if (!MIX_PlayAudio(mixer_, clip)) {
    Logger::Warn("MIX_PlayAudio failed for " + event.clipId + ": " + std::string(SDL_GetError()));
  }
}
