#pragma once

#include <SDL3/SDL.h>

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

class AssetManager;

// Runtime side of the glyph atlas: holds a CPU pixel buffer of the packed glyph PNG + a codepoint
// → metrics map. CPU-side (vs. an SDL_Texture) so RenderTextSystem can compose strings by blitting
// from this surface into a destination SDL_Surface, then SDL_CreateTextureFromSurface — same
// lifetime + caching shape RenderTextSystem already uses for the TTF_RenderText path. Loaded once
// per (font, size) pair when AssetManager::AddFont sees a sidecar .atlas.png next to the .ttf.
class GlyphAtlas {
 public:
  struct Glyph {
    float x{0}, y{0}, w{0}, h{0};
    float advance{0};
    float minx{0}, miny{0};
  };

  GlyphAtlas() = default;
  GlyphAtlas(const GlyphAtlas&) = delete;
  GlyphAtlas& operator=(const GlyphAtlas&) = delete;
  GlyphAtlas(GlyphAtlas&&) noexcept;
  GlyphAtlas& operator=(GlyphAtlas&&) noexcept;
  ~GlyphAtlas();

  // Load PNG + Lua sidecar from disk paths. Both routed through SDL_IOFromFile so a packed APK
  // / shipped pak resolves identically to the loose tree. Returns true on success; false leaves
  // the instance empty + safe to destroy.
  bool Load(const std::string& pngFullPath, const std::string& luaFullPath);

  [[nodiscard]] bool IsLoaded() const { return source_surface_ != nullptr; }
  [[nodiscard]] const Glyph* Find(std::uint32_t codepoint) const;
  [[nodiscard]] bool Contains(std::uint32_t codepoint) const { return Find(codepoint) != nullptr; }
  // Source surface wrapping the loaded RGBA pixel buffer. RenderTextSystem blits glyph rects out
  // of this into a destination surface during compose. Lifetime is owned by the GlyphAtlas.
  [[nodiscard]] SDL_Surface* SourceSurface() const { return source_surface_; }
  [[nodiscard]] int LineSkip() const { return line_skip_; }
  [[nodiscard]] int AtlasWidth() const { return atlas_width_; }
  [[nodiscard]] int AtlasHeight() const { return atlas_height_; }
  [[nodiscard]] std::size_t Size() const { return glyphs_.size(); }

 private:
  std::vector<std::uint8_t> pixels_;
  SDL_Surface* source_surface_{nullptr};
  std::unordered_map<std::uint32_t, Glyph> glyphs_;
  int line_skip_{0};
  int atlas_width_{0};
  int atlas_height_{0};
};
