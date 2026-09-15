#include "AssetManager/FontStore.h"

#include <filesystem>
#include <utility>

#include "AssetManager/GlyphAtlas.h"
#include "General/Logger.h"

// Out-of-line so the unique_ptr<GlyphAtlas> members can be destroyed/moved where GlyphAtlas is a
// complete type (the header only forward-declares it).
FontStore::FontStore() = default;
FontStore::FontStore(FontStore&&) noexcept = default;
FontStore& FontStore::operator=(FontStore&&) noexcept = default;
FontStore::~FontStore() { Clear(); }

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
TTF_Font* FontStore::Add(const std::string& id, SDL_IOStream* io, const float fontSize, const std::string& basePath,
                         const IoOpener& openIO) {
  TTF_Font* font = TTF_OpenFontIO(io, true, fontSize);  // closes the stream
  if (!font) {
    Logger::Error("Failed to load font " + id + ": " + std::string(SDL_GetError()));
    return nullptr;
  }

  if (const auto it = fonts_.find(id); it != fonts_.end()) {
    Logger::Warn("Replacing existing font: " + id);
    TTF_CloseFont(it->second);
    it->second = font;
  } else {
    fonts_.emplace(id, font);
  }

  Logger::Info("Added font: " + id);

  // Probe for a glyph-atlas sidecar pair (Stage 14 B3). The bake step writes them under
  // atlases/<asset_id>.atlas.{png,lua}; presence is the opt-in. When openIO is provided
  // (e.g. via AssetManager), streams resolve through asset_bundle.pak or loose files alike.
  const std::string relPng = "atlases/" + id + ".atlas.png";
  const std::string relLua = "atlases/" + id + ".atlas.lua";

  SDL_IOStream* pngStream = nullptr;
  SDL_IOStream* luaStream = nullptr;

  if (openIO) {
    pngStream = openIO(relPng);
    if (pngStream != nullptr) {
      luaStream = openIO(relLua);
    }
  } else if (!basePath.empty()) {
    const std::filesystem::path atlasPng = std::filesystem::path(basePath) / relPng;
    const std::filesystem::path atlasLua = std::filesystem::path(basePath) / relLua;
    pngStream = SDL_IOFromFile(atlasPng.string().c_str(), "rb");
    if (pngStream != nullptr) {
      luaStream = SDL_IOFromFile(atlasLua.string().c_str(), "rb");
    }
  }

  if (pngStream != nullptr) {
    if (luaStream != nullptr) {
      auto atlas = std::make_unique<GlyphAtlas>();
      if (atlas->Load(pngStream, luaStream, relPng, relLua)) {
        glyph_atlases_[id] = std::move(atlas);
      }
    } else {
      SDL_CloseIO(pngStream);
    }
  }

  return font;
}

TTF_Font* FontStore::Get(const std::string& id) const {
  const auto it = fonts_.find(id);
  return it == fonts_.end() ? nullptr : it->second;
}

const GlyphAtlas* FontStore::GetGlyphAtlas(const std::string& id) const {
  const auto it = glyph_atlases_.find(id);
  return it == glyph_atlases_.end() ? nullptr : it->second.get();
}

bool FontStore::Remove(const std::string& id) {
  const auto it = fonts_.find(id);
  if (it == fonts_.end()) return false;
  TTF_CloseFont(it->second);
  fonts_.erase(it);
  return true;
}

void FontStore::Clear() {
  for (const auto& [id, font] : fonts_) {
    TTF_CloseFont(font);
  }
  fonts_.clear();
}
