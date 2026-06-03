#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <map>
#include <memory>
#include <string>

class GlyphAtlas;

// Owns the resident TTF_Font* handles keyed by asset id, plus the per-font glyph atlases probed
// alongside them. Like TextureStore it is pak-agnostic: AssetManager opens the font's IO stream and
// passes it in, along with the project base path used to probe for an `atlases/<id>.atlas.{png,lua}`
// sidecar pair (the glyph-atlas opt-in consumed by RenderTextSystem's compose path).
class FontStore {
 public:
  FontStore();
  FontStore(const FontStore&) = delete;
  FontStore& operator=(const FontStore&) = delete;
  FontStore(FontStore&&) noexcept;
  FontStore& operator=(FontStore&&) noexcept;
  ~FontStore();

  // Load a font from an already-opened, owned IO stream (consumed and closed by the loader) at
  // `fontSize`px, replacing any prior handle under `id`. When `basePath` is non-empty, also probes
  // for and loads a glyph-atlas sidecar for `id`. Returns the resident font, or nullptr on failure.
  TTF_Font* Add(const std::string& id, SDL_IOStream* io, float fontSize, const std::string& basePath);

  [[nodiscard]] TTF_Font* Get(const std::string& id) const;
  [[nodiscard]] bool Contains(const std::string& id) const { return fonts_.contains(id); }

  // Resident glyph atlas for `id`, or nullptr when none was loaded.
  [[nodiscard]] const GlyphAtlas* GetGlyphAtlas(const std::string& id) const;

  // Close and erase the font under `id`; returns whether one was removed. Mirrors the prior
  // behavior of leaving any glyph atlas for `id` resident (atlases outlive their font handle and
  // are refreshed on the next Add).
  bool Remove(const std::string& id);

  // Close every font. Glyph atlases are intentionally left resident (matching the pre-split
  // ClearAssets, which only cleared the font handles); they are freed when the store is destroyed.
  void Clear();

  [[nodiscard]] const std::map<std::string, TTF_Font*>& All() const { return fonts_; }

 private:
  std::map<std::string, TTF_Font*> fonts_;
  // Per-font glyph atlases keyed by font catalog id (e.g. "main-16"). unique_ptr keeps each
  // GlyphAtlas pinned across the map's rehashes (its surface wraps a vector<uint8_t> that must not
  // move underneath SDL).
  std::map<std::string, std::unique_ptr<GlyphAtlas>> glyph_atlases_;
};
