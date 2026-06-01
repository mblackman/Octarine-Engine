#include "AudioSystem.h"

#include <SDL3/SDL.h>

#include <algorithm>
#include <string>

#include "../AssetManager/AssetManager.h"
#include "../Components/AudioActiveTag.h"
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

  auto& engineOptions = registry_->Get<GameConfig>().GetEngineOptions();
  const int trackPoolInitial = std::max(1, engineOptions.audioTrackPoolInitial);
  const int trackPoolMax = std::max(trackPoolInitial, engineOptions.audioTrackPoolMax);

  // On any audio-stack init failure, flip audioEnabled off so per-frame audio gates short-circuit
  // and AssetManager skips audio acquires silently (no per-clip "no mixer" errors). One Error +
  // one Warn at startup is the user-visible signal; everything downstream stays quiet.
  if (!MIX_Init()) {
    Logger::Error("MIX_Init failed: " + std::string(SDL_GetError()));
    engineOptions.audioEnabled = false;
    return false;
  }

  MIX_Mixer* mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
  if (!mixer) {
    Logger::Error("MIX_CreateMixerDevice failed: " + std::string(SDL_GetError()));
    MIX_Quit();
    engineOptions.audioEnabled = false;
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

  // Mixer is published onto EngineContext by Game::Setup once Init returns; AudioSystem owns
  // the pointer and exposes it via Mixer() so callers don't need a Registry round-trip.
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

  // Per-track gain carries only this clip's own volume. Master volume + mute are applied at the
  // mixer level (MIX_SetMixerGain, synced each frame in Game::Update) so they affect live tracks.
  const float gain = std::max(0.0F, event.volume);

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
    // Spatial sources are gated by AudioCullingSystem's AudioActiveTag: an out-of-range
    // emitter has no tag, so even a resume-pending source waits here until it comes back
    // into the listener radius. Without this gate the resume + cull pair would oscillate
    // (AudioSystem re-spawns the sink, CullingSystem culls it again next frame).
    if (source.spatial && !registry_->HasTag<AudioActiveTag>(entity)) return;

    // Resume-from-cull is a second valid play trigger alongside playOnSpawn: when
    // AudioCullingSystem halts a culled emitter it stashes the frame offset on the
    // source and removes the sink, so the entity arrives here with a >=0 offset
    // waiting to be consumed.
    const bool resumingFromCull = source.playbackOffsetFrames >= 0;
    if (!source.playOnSpawn && !resumingFromCull) return;

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
    const float gain = std::max(0.0F, source.volume);
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

    // Seek AFTER play: SDL_mixer accepts position changes on a playing track and the
    // seek-then-play path skips an extra paused-state transition.
    if (resumingFromCull) {
      MIX_SetTrackPlaybackPosition(acquired.track, source.playbackOffsetFrames);
      source.playbackOffsetFrames = -1;
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
    const float gain = std::max(0.0F, source.volume);
    MIX_SetTrackGain(sink.track, gain);
    return;
  }

  sink.finished = true;
  sink.track = nullptr;

  if (source.despawnOnFinish) {
    cmd_buffer_.EmplaceBlam(entity);
  }
}
