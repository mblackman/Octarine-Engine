#pragma once

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <map>
#include <string>

// Owns the resident MIX_Audio* handles keyed by asset id. Pak-agnostic like the texture/font
// stores: AssetManager opens the IO stream and passes it in along with the mixer and the catalog's
// `stream` flag (predecode toggle).
class AudioClipStore {
 public:
  AudioClipStore() = default;
  AudioClipStore(const AudioClipStore&) = delete;
  AudioClipStore& operator=(const AudioClipStore&) = delete;
  AudioClipStore(AudioClipStore&&) noexcept = default;
  AudioClipStore& operator=(AudioClipStore&&) noexcept = default;
  ~AudioClipStore() { Clear(); }

  // Load an audio clip from an already-opened, owned IO stream (consumed and closed by the loader),
  // replacing any prior handle under `id`. `stream == false` predecodes the full PCM at load time
  // (short SFX, lowest play latency); `stream == true` decodes on demand (long music / ambient).
  // Returns the resident clip, or nullptr on failure. `logPath` feeds the log line only.
  MIX_Audio* Add(MIX_Mixer* mixer, const std::string& id, SDL_IOStream* io, bool stream, const std::string& logPath);

  [[nodiscard]] MIX_Audio* Get(const std::string& id) const;
  [[nodiscard]] bool Contains(const std::string& id) const { return audio_clips_.contains(id); }

  bool Remove(const std::string& id);
  void Clear();

  [[nodiscard]] const std::map<std::string, MIX_Audio*>& All() const { return audio_clips_; }

 private:
  std::map<std::string, MIX_Audio*> audio_clips_;
};
