#pragma once

#include <SDL3/SDL.h>

#include <cstdint>
#include <map>
#include <optional>
#include <string>

// Owns the resident SDL_Texture* handles keyed by asset id, the project default scale mode, and the
// generation counter sprite renderers poll to know when a cached SDL_Texture* has gone stale.
//
// Deliberately narrow: it knows nothing about the catalog, refcounts, or asset paks. AssetManager
// opens the SDL_IOStream (resolving paks / base path) and feeds it here; atlas-member redirection
// stays in AssetManager because it needs the catalog. Lookups here are direct id -> handle.
class TextureStore {
 public:
  TextureStore() = default;
  TextureStore(const TextureStore&) = delete;
  TextureStore& operator=(const TextureStore&) = delete;
  TextureStore(TextureStore&&) noexcept = default;
  TextureStore& operator=(TextureStore&&) noexcept = default;
  ~TextureStore() { Clear(); }

  void SetDefaultScaleMode(std::optional<SDL_ScaleMode> mode) { default_scale_mode_ = mode; }

  // Create a texture from an already-opened, owned IO stream (consumed and closed by the loader).
  // Applies the default scale mode, replaces any prior handle under `id`, and bumps the generation.
  // Returns the resident texture, or nullptr on load failure. `logPath` feeds the log line only.
  SDL_Texture* Add(SDL_Renderer* renderer, const std::string& id, SDL_IOStream* io, const std::string& logPath);

  // Direct lookup — no atlas-member redirect (that lives in AssetManager, which has the catalog).
  [[nodiscard]] SDL_Texture* Get(const std::string& id) const;
  [[nodiscard]] bool Contains(const std::string& id) const { return textures_.contains(id); }

  // Destroy and erase the handle under `id`, bumping the generation so cached pointers re-resolve.
  // Returns whether an entry was removed.
  bool Remove(const std::string& id);
  void Clear();

  [[nodiscard]] std::uint64_t Generation() const { return generation_; }
  [[nodiscard]] const std::map<std::string, SDL_Texture*>& All() const { return textures_; }

 private:
  std::map<std::string, SDL_Texture*> textures_;
  std::optional<SDL_ScaleMode> default_scale_mode_;
  // Bumped whenever a texture is added, replaced, or removed. Sprite renderers compare against their
  // cached value to know when to re-resolve a stale SDL_Texture* pointer.
  std::uint64_t generation_{0};
};
