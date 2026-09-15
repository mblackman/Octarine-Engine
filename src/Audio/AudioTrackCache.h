#pragma once

#include <SDL3_mixer/SDL_mixer.h>

#include <cstdint>
#include <unordered_map>

#include "ECS/ECS.h"
#include "ECS/Entity.h"

// Cache mapping Entity to its assigned MIX_Track* and track-pool generation.
// Decouples backend audio handles from AudioSinkComponent data while allowing
// SpatialAudioSystem, DopplerSystem, and AudioCullingSystem to resolve active tracks each frame.
//
// Stored as a Registry singleton and cleared on scene unload. Single-threaded access:
// audio systems execute sequentially.
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
