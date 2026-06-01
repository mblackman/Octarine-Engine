#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <cstdint>
#include <string>
#include <vector>

// Bake-time glyph atlas writer: rasterize a fixed codepoint set into a single packed PNG +
// sidecar Lua metrics, so a shipped build can draw text by sampling the atlas instead of paying
// per-string TTF_Render at runtime. Pure data emit — no SDL renderer involved; the metrics file
// is the source of truth, the PNG is just pixel storage.
class AtlasBaker {
 public:
  // Rasterize `codepoints` from `font` at the (caller-already-opened) size into an atlas. Writes
  // `<outPngPath>` (RGBA8 atlas image) and `<outLuaPath>` (Lua `return { ... }` metrics file).
  // Returns false on any TTF rasterization, packing, or I/O failure; partial files are left in
  // place for diagnostics. The codepoint list is taken verbatim — caller is responsible for
  // de-duping + picking the right ASCII / Latin-1 / scripts subset.
  static bool Bake(TTF_Font* font, float fontSize, const std::vector<std::uint32_t>& codepoints,
                   const std::string& outPngPath, const std::string& outLuaPath);

  // Default ASCII printable set (codepoints 32..126). Suitable for most western UIs; projects with
  // accented Latin or non-Latin scripts will eventually want an explicit list via meta.glyphs (not
  // yet plumbed).
  static std::vector<std::uint32_t> DefaultAsciiPrintable();
};
