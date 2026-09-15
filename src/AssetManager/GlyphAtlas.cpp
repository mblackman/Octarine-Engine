#include "AssetManager/GlyphAtlas.h"

#include <cstring>

#define STB_IMAGE_IMPLEMENTATION
#include <sol/sol.hpp>

#include "Engine/LuaProtect.h"
#include "General/Logger.h"
#include "stb/stb_image.h"

GlyphAtlas::GlyphAtlas(GlyphAtlas&& other) noexcept
    : pixels_(std::move(other.pixels_)),
      source_surface_(other.source_surface_),
      glyphs_(std::move(other.glyphs_)),
      line_skip_(other.line_skip_),
      atlas_width_(other.atlas_width_),
      atlas_height_(other.atlas_height_) {
  other.source_surface_ = nullptr;
}

GlyphAtlas& GlyphAtlas::operator=(GlyphAtlas&& other) noexcept {
  if (this != &other) {
    if (source_surface_ != nullptr) SDL_DestroySurface(source_surface_);
    pixels_ = std::move(other.pixels_);
    source_surface_ = other.source_surface_;
    glyphs_ = std::move(other.glyphs_);
    line_skip_ = other.line_skip_;
    atlas_width_ = other.atlas_width_;
    atlas_height_ = other.atlas_height_;
    other.source_surface_ = nullptr;
  }
  return *this;
}

GlyphAtlas::~GlyphAtlas() {
  if (source_surface_ != nullptr) SDL_DestroySurface(source_surface_);
}

bool GlyphAtlas::Load(SDL_IOStream* pngIo, SDL_IOStream* luaIo, const std::string& debugPngName,
                      const std::string& debugLuaName) {
  if (pngIo == nullptr || luaIo == nullptr) {
    if (pngIo != nullptr) SDL_CloseIO(pngIo);
    if (luaIo != nullptr) SDL_CloseIO(luaIo);
    return false;
  }

  // PNG -> raw RGBA8.
  std::size_t pngBytes = 0;
  void* raw = SDL_LoadFile_IO(pngIo, &pngBytes, true);
  if (raw == nullptr) {
    SDL_CloseIO(luaIo);
    Logger::Error("GlyphAtlas::Load: SDL_LoadFile_IO failed for " + debugPngName + ": " + std::string(SDL_GetError()));
    return false;
  }
  int w = 0, h = 0, channels = 0;
  stbi_uc* decoded =
      stbi_load_from_memory(static_cast<const stbi_uc*>(raw), static_cast<int>(pngBytes), &w, &h, &channels, 4);
  SDL_free(raw);
  if (decoded == nullptr) {
    SDL_CloseIO(luaIo);
    Logger::Error("GlyphAtlas::Load: stbi_load_from_memory failed for " + debugPngName);
    return false;
  }
  pixels_.assign(decoded, decoded + static_cast<std::size_t>(w) * h * 4);
  stbi_image_free(decoded);
  atlas_width_ = w;
  atlas_height_ = h;

  source_surface_ = SDL_CreateSurfaceFrom(w, h, SDL_PIXELFORMAT_RGBA32, pixels_.data(), w * 4);
  if (source_surface_ == nullptr) {
    SDL_CloseIO(luaIo);
    Logger::Error("GlyphAtlas::Load: SDL_CreateSurfaceFrom failed: " + std::string(SDL_GetError()));
    return false;
  }

  sol::state lua;
  lua.open_libraries(sol::lib::base);
  std::size_t luaBytes = 0;
  void* luaRaw = SDL_LoadFile_IO(luaIo, &luaBytes, true);
  if (luaRaw == nullptr) {
    Logger::Error("GlyphAtlas::Load: SDL_LoadFile_IO failed for " + debugLuaName + ": " + std::string(SDL_GetError()));
    SDL_DestroySurface(source_surface_);
    source_surface_ = nullptr;
    return false;
  }
  std::string luaChunk(static_cast<const char*>(luaRaw), luaBytes);
  SDL_free(luaRaw);
  DecryptLuaBytes(luaChunk);
  sol::protected_function_result rc = lua.safe_script(luaChunk, sol::script_pass_on_error, "@" + debugLuaName);
  if (!rc.valid()) {
    const sol::error err = rc;
    Logger::Error("GlyphAtlas::Load: dofile failed for " + debugLuaName + ": " + err.what());
    SDL_DestroySurface(source_surface_);
    source_surface_ = nullptr;
    return false;
  }
  const sol::table table = rc;
  line_skip_ = table.get_or("line_skip", 0);

  glyphs_.clear();
  const sol::optional<sol::table> g = table["glyphs"];
  if (!g.has_value()) {
    Logger::Error("GlyphAtlas::Load: " + debugLuaName + " missing `glyphs` table");
    SDL_DestroySurface(source_surface_);
    source_surface_ = nullptr;
    return false;
  }
  for (const auto& pair : *g) {
    const std::uint32_t cp = pair.first.as<std::uint32_t>();
    const sol::table entry = pair.second.as<sol::table>();
    Glyph out{};
    out.x = entry.get_or("x", 0.0F);
    out.y = entry.get_or("y", 0.0F);
    out.w = entry.get_or("w", 0.0F);
    out.h = entry.get_or("h", 0.0F);
    out.advance = entry.get_or("advance", 0.0F);
    out.minx = entry.get_or("minx", 0.0F);
    out.miny = entry.get_or("miny", 0.0F);
    glyphs_.emplace(cp, out);
  }
  Logger::Info("GlyphAtlas: loaded " + std::to_string(glyphs_.size()) + " glyphs from " + debugPngName);
  return true;
}

bool GlyphAtlas::Load(const std::string& pngFullPath, const std::string& luaFullPath) {
  SDL_IOStream* pngIo = SDL_IOFromFile(pngFullPath.c_str(), "rb");
  if (pngIo == nullptr) {
    Logger::Error("GlyphAtlas::Load: SDL_IOFromFile failed for " + pngFullPath + ": " + std::string(SDL_GetError()));
    return false;
  }
  SDL_IOStream* luaIo = SDL_IOFromFile(luaFullPath.c_str(), "rb");
  if (luaIo == nullptr) {
    SDL_CloseIO(pngIo);
    Logger::Error("GlyphAtlas::Load: SDL_IOFromFile failed for " + luaFullPath + ": " + std::string(SDL_GetError()));
    return false;
  }
  return Load(pngIo, luaIo, pngFullPath, luaFullPath);
}

const GlyphAtlas::Glyph* GlyphAtlas::Find(std::uint32_t codepoint) const {
  const auto it = glyphs_.find(codepoint);
  return it == glyphs_.end() ? nullptr : &it->second;
}
