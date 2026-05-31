#pragma once

#include <SDL3/SDL.h>

#include <cstdint>
#include <unordered_map>

#include "ECS/Entity.h"

// Render-side cache mapping Entity -> last-resolved SDL_Texture* + atlas-slice origin + the
// AssetManager texture-generation the values were resolved against. Lives in the render layer
// so SpriteComponent stays POD (no SDL handles in game data). For atlas-packed assets, the
// slice origin is added to the sprite's logical srcRect during emit so the same script-level
// (sx, sy) selects inside the packed atlas; loose textures leave the offset zero (no-op add).
//
// Single-threaded: callers must serialize Store/Forget/Clear. RenderSpriteSystem warms the
// cache in a serial Prepare pass and reads it in the parallel emit pass — never inserts under
// ParallelForEach because std::unordered_map cannot tolerate concurrent writes.
class SpriteRenderCache {
 public:
  struct Entry {
    SDL_Texture* texture = nullptr;
    SDL_FRect atlasOffset{};
    std::uint64_t generation = 0;
  };

  // Returns the cached entry pointer iff present and generation matches; nullptr otherwise.
  // Caller resolves + Store()s on a miss (warm-pass only — emit is read-only).
  [[nodiscard]] const Entry* Lookup(Entity entity, std::uint64_t currentGen) const {
    if (const auto it = entries_.find(entity.GetId()); it != entries_.end() && it->second.generation == currentGen) {
      return &it->second;
    }
    return nullptr;
  }

  void Store(Entity entity, SDL_Texture* texture, SDL_FRect atlasOffset, std::uint64_t generation) {
    entries_[entity.GetId()] = {texture, atlasOffset, generation};
  }

  void Forget(Entity entity) { entries_.erase(entity.GetId()); }

  void Clear() { entries_.clear(); }

 private:
  std::unordered_map<EcsId, Entry> entries_;
};
