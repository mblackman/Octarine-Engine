#include "AssetManager/TextureStore.h"

#include <SDL3_image/SDL_image.h>

#include "General/Logger.h"

SDL_Texture* TextureStore::Add(SDL_Renderer* renderer, const std::string& id, SDL_IOStream* io,
                               const std::string& logPath) {
  SDL_Texture* texture = IMG_LoadTexture_IO(renderer, io, true);  // closes the stream
  if (!texture) {
    Logger::Error("Failed to create texture: " + std::string(SDL_GetError()));
    return nullptr;
  }

  if (default_scale_mode_.has_value()) {
    SDL_SetTextureScaleMode(texture, default_scale_mode_.value());
  }

  // Replace prior texture under the same id rather than leaking it.
  if (const auto it = textures_.find(id); it != textures_.end()) {
    Logger::Warn("Replacing existing texture: " + id);
    SDL_DestroyTexture(it->second);
    it->second = texture;
  } else {
    textures_.emplace(id, texture);
  }
  ++generation_;

  Logger::Info("Added texture: " + id + " from path: " + logPath);
  return texture;
}

SDL_Texture* TextureStore::Get(const std::string& id) const {
  const auto it = textures_.find(id);
  return it == textures_.end() ? nullptr : it->second;
}

bool TextureStore::Remove(const std::string& id) {
  const auto it = textures_.find(id);
  if (it == textures_.end()) return false;
  SDL_DestroyTexture(it->second);
  textures_.erase(it);
  ++generation_;  // invalidate cached SDL_Texture* in sprite renderers
  return true;
}

void TextureStore::Clear() {
  for (const auto& [id, texture] : textures_) {
    SDL_DestroyTexture(texture);
  }
  textures_.clear();
}
