#pragma once

#include <SDL3/SDL.h>

#include <optional>
#include <string>

#include "General/Logger.h"

// Read a file's bytes through SDL_IO so the same path resolves on desktop, inside an APK asset
// root, or inside a .app bundle. Lua's stock fopen-based loader only sees a real filesystem and
// misses AAssetManager-backed entries on Android. Shared by Game::LoadGame and SceneLoader (the
// dofile override that exposes the same behavior to scripts lives in
// engine_bootstrap::InstallLuaLibraries). Header-only inline so both engine-layer TUs share one
// definition without a separate object.
inline std::optional<std::string> ReadFileViaSDL(const std::string& path) {
  SDL_IOStream* io = SDL_IOFromFile(path.c_str(), "rb");
  if (!io) {
    Logger::Error("SDL_IOFromFile failed for '" + path + "': " + std::string(SDL_GetError()));
    return std::nullopt;
  }
  std::size_t size = 0;
  void* data = SDL_LoadFile_IO(io, &size, true);  // closes io
  if (!data) {
    Logger::Error("SDL_LoadFile_IO failed for '" + path + "': " + std::string(SDL_GetError()));
    return std::nullopt;
  }
  std::string out(static_cast<const char*>(data), size);
  SDL_free(data);
  return out;
}
