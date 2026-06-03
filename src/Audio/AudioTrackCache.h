#pragma once

#include <SDL3_mixer/SDL_mixer.h>

#include <cstdint>
#include <unordered_map>

#include "ECS/ECS.h"
#include "ECS/Entity.h"

// Audio-side cache mapping Entity -> the MIX_Track* AudioSystem assigned that emitter's
// AudioSinkComponent, plus the track-pool generation issued for the acquisition. Mirrors
// Renderer/SpriteRenderCache: it keeps AudioSinkComponent POD-no-SDL while SpatialAudioSystem /
// DopplerSystem / AudioCullingSystem resolve the live track here each frame.
//
// A Registry singleton, Set on the live-frame path only (bake runs no audio systems) and Clear()ed
// on scene unload. Single-threaded: every audio system runs serially (RegisterSystem, not the
// parallel path), so unsynchronized map writes are safe — std::unordered_map cannot tolerate
// concurrent insertion.
class AudioTrackCache {
 public:
  struct Entry {
    MIX_Track* track = nullptr;
    std::uint32_t generation = 0;
  };

  void Store(Entity entity, MIX_Track* track, std::uint32_t generation) {
    entries_[entity.GetId()] = {track, generation};
  }

  // The live MIX_Track* cached for this entity, or nullptr if none.
  [[nodiscard]] MIX_Track* Track(Entity entity) const {
    const auto it = entries_.find(entity.GetId());
    return it != entries_.end() ? it->second.track : nullptr;
  }

  // The full (track, generation) entry, or nullptr if absent — used for the pool-ownership check.
  [[nodiscard]] const Entry* Lookup(Entity entity) const {
    const auto it = entries_.find(entity.GetId());
    return it != entries_.end() ? &it->second : nullptr;
  }

  void Forget(Entity entity) { entries_.erase(entity.GetId()); }

  void Clear() { entries_.clear(); }

 private:
  std::unordered_map<EcsId, Entry> entries_;
};
