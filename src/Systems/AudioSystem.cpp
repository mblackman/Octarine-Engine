#include "AudioSystem.h"

#include <SDL3/SDL.h>

#include <algorithm>
#include <string>

#include "../AssetManager/AssetManager.h"
#include "../Game/GameConfig.h"
#include "../General/Logger.h"

AudioSystem::AudioSystemState::~AudioSystemState() {
  for (TrackSlot& slot : track_pool) {
    if (slot.track) MIX_DestroyTrack(slot.track);
  }
  track_pool.clear();

  if (mixer) {
    MIX_DestroyMixer(mixer);
    MIX_Quit();
  }
}

bool AudioSystem::Init(Registry* registry, const std::unique_ptr<EventBus>& eventBus) {
  registry_ = registry;

  const auto& engineOptions = registry_->Get<GameConfig>().GetEngineOptions();
  const int trackPoolInitial = std::max(1, engineOptions.audioTrackPoolInitial);
  const int trackPoolMax = std::max(trackPoolInitial, engineOptions.audioTrackPoolMax);

  if (!MIX_Init()) {
    Logger::Error("MIX_Init failed: " + std::string(SDL_GetError()));
    return false;
  }

  MIX_Mixer* mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
  if (!mixer) {
    Logger::Error("MIX_CreateMixerDevice failed: " + std::string(SDL_GetError()));
    MIX_Quit();
    return false;
  }

  state_ = std::make_shared<AudioSystemState>();
  state_->mixer = mixer;
  state_->track_pool_max = trackPoolMax;

  state_->track_pool.reserve(static_cast<std::size_t>(trackPoolInitial));
  for (int i = 0; i < trackPoolInitial; ++i) {
    if (MIX_Track* track = MIX_CreateTrack(mixer)) {
      state_->track_pool.push_back({track, 0});
    } else {
      Logger::Warn("MIX_CreateTrack failed at slot " + std::to_string(i) + ": " + std::string(SDL_GetError()));
      break;
    }
  }

  registry_->Set<MIX_Mixer*>(mixer);

  eventBus->SubscribeEvent<AudioSystem, AudioPlayEvent>(this, &AudioSystem::OnAudioPlay);

  Logger::Info("AudioSystem initialized; track pool size = " + std::to_string(state_->track_pool.size()) +
               ", max = " + std::to_string(trackPoolMax));
  return true;
}

void AudioSystem::Shutdown() { state_.reset(); }

bool AudioSystem::SinkOwnsTrack(const AudioSinkComponent& sink) const {
  if (!state_ || !sink.track) return false;
  for (const TrackSlot& slot : state_->track_pool) {
    if (slot.track == sink.track) return slot.generation == sink.generation;
  }
  return false;
}

AudioSystem::Acquired AudioSystem::AcquireTrack() {
  if (!state_) return {};

  for (TrackSlot& slot : state_->track_pool) {
    if (slot.track && !MIX_TrackPlaying(slot.track)) {
      ++slot.generation;
      return {slot.track, slot.generation};
    }
  }

  if (static_cast<int>(state_->track_pool.size()) < state_->track_pool_max) {
    if (MIX_Track* track = MIX_CreateTrack(state_->mixer)) {
      state_->track_pool.push_back({track, 1});
      return {track, 1};
    }
    Logger::Warn("MIX_CreateTrack failed during pool expansion: " + std::string(SDL_GetError()));
  }

  return {};
}

void AudioSystem::OnAudioPlay(const AudioPlayEvent& event) {
  if (!state_ || !registry_) return;

  const auto& engineOptions = registry_->Get<GameConfig>().GetEngineOptions();
  if (!engineOptions.audioEnabled) return;

  auto& assetManager = registry_->Get<AssetManager>();
  MIX_Audio* clip = assetManager.GetAudioClip(event.clipId);
  if (!clip) return;

  const float gain = std::max(0.0F, event.volume * engineOptions.masterVolume);

  const Acquired acquired = AcquireTrack();
  if (!acquired.track) {
    Logger::Warn("Audio track pool exhausted (cap " + std::to_string(state_->track_pool_max) + "); dropping " +
                 event.clipId);
    return;
  }

  if (!MIX_SetTrackAudio(acquired.track, clip)) {
    Logger::Warn("MIX_SetTrackAudio failed for " + event.clipId + ": " + std::string(SDL_GetError()));
    return;
  }
  MIX_SetTrackGain(acquired.track, gain);
  if (!MIX_PlayTrack(acquired.track, 0)) {
    Logger::Warn("MIX_PlayTrack failed for " + event.clipId + ": " + std::string(SDL_GetError()));
  }
}

void AudioSystem::operator()(Entity entity, AudioSourceComponent& source) {
  if (!state_ || !registry_) return;

  const auto& engineOptions = registry_->Get<GameConfig>().GetEngineOptions();
  if (!engineOptions.audioEnabled) return;

  if (!registry_->HasComponent<AudioSinkComponent>(entity)) {
    if (!source.playOnSpawn) return;

    auto& assetManager = registry_->Get<AssetManager>();
    MIX_Audio* clip = assetManager.GetAudioClip(source.clipId);
    if (!clip) {
      Logger::Warn("AudioSystem: Clip not found: " + source.clipId);
      // Mark "tried" by attaching a finished sink — prevents per-frame retry without mutating the source.
      AudioSinkComponent sink;
      sink.finished = true;
      cmd_buffer_.AddComponent<AudioSinkComponent>(entity, sink);
      return;
    }

    const Acquired acquired = AcquireTrack();
    if (!acquired.track) return;  // pool full, retry next frame

    if (!MIX_SetTrackAudio(acquired.track, clip)) {
      Logger::Warn("MIX_SetTrackAudio failed for " + source.clipId + ": " + std::string(SDL_GetError()));
      return;
    }
    const float gain = std::max(0.0F, source.volume * engineOptions.masterVolume);
    MIX_SetTrackGain(acquired.track, gain);

    // SDL3_mixer: MIX_PlayTrack's second arg is an SDL_PropertiesID, not a loop count.
    // Encode looping via MIX_PROP_PLAY_LOOPS_NUMBER (-1 = infinite).
    SDL_PropertiesID props = 0;
    if (source.loop) {
      props = SDL_CreateProperties();
      SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
    }
    const bool played = MIX_PlayTrack(acquired.track, props);
    if (props) SDL_DestroyProperties(props);
    if (!played) {
      Logger::Warn("MIX_PlayTrack failed for " + source.clipId + ": " + std::string(SDL_GetError()));
      return;
    }
    cmd_buffer_.AddComponent<AudioSinkComponent>(entity, AudioSinkComponent(acquired.track, acquired.generation));
    return;
  }

  auto& sink = registry_->GetComponent<AudioSinkComponent>(entity);
  if (sink.finished || !sink.track) return;

  // Validate the sink still owns its track — generation guards against pool reuse stealing it
  // after our clip stopped. Treat a stolen track the same as a natural finish.
  const bool stillOwns = SinkOwnsTrack(sink);
  if (stillOwns && MIX_TrackPlaying(sink.track)) {
    const float gain = std::max(0.0F, source.volume * engineOptions.masterVolume);
    MIX_SetTrackGain(sink.track, gain);
    return;
  }

  sink.finished = true;
  sink.track = nullptr;

  if (source.despawnOnFinish) {
    cmd_buffer_.EmplaceBlam(entity);
  }
}
