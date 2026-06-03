#include "AssetManager/AudioClipStore.h"

#include "General/Logger.h"

MIX_Audio* AudioClipStore::Add(MIX_Mixer* mixer, const std::string& id, SDL_IOStream* io, const bool stream,
                               const std::string& logPath) {
  // predecode=true is the default for short SFX — full PCM in RAM at load time, lowest play-call
  // latency. predecode=false (meta.stream=true on the source) keeps the file compressed and
  // decodes on demand; pays back as flat memory for long-form music + ambient beds.
  MIX_Audio* clip = MIX_LoadAudio_IO(mixer, io, !stream, true);  // closes the stream regardless
  if (!clip) {
    Logger::Error("Failed to load audio clip " + id + " from " + logPath + ": " + std::string(SDL_GetError()));
    return nullptr;
  }

  if (const auto it = audio_clips_.find(id); it != audio_clips_.end()) {
    Logger::Warn("Replacing existing audio clip: " + id);
    MIX_DestroyAudio(it->second);
    it->second = clip;
  } else {
    audio_clips_.emplace(id, clip);
  }

  Logger::Info("Added audio clip: " + id + " from path: " + logPath);
  return clip;
}

MIX_Audio* AudioClipStore::Get(const std::string& id) const {
  const auto it = audio_clips_.find(id);
  return it == audio_clips_.end() ? nullptr : it->second;
}

bool AudioClipStore::Remove(const std::string& id) {
  const auto it = audio_clips_.find(id);
  if (it == audio_clips_.end()) return false;
  MIX_DestroyAudio(it->second);
  audio_clips_.erase(it);
  return true;
}

void AudioClipStore::Clear() {
  for (const auto& [id, clip] : audio_clips_) {
    MIX_DestroyAudio(clip);
  }
  audio_clips_.clear();
}
