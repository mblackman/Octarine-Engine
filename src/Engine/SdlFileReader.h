#pragma once

#include <SDL3/SDL.h>

#include <optional>
#include <string>

#include "AssetManager/AssetManager.h"
#include "General/Logger.h"

// Read a file's bytes through SDL_IO so the same path resolves on desktop, inside an APK asset
// root, inside a .app bundle, or out of an in-memory AssetPak. Shared by Game::LoadGame and SceneLoader
// (and the dofile override in engine_bootstrap::InstallLuaLibraries). Header-only inline so both
// engine-layer TUs share one definition without a separate object.
inline std::optional<std::string> ReadFileViaSDL(const std::string& path, const AssetManager* assetManager = nullptr) {
  SDL_IOStream* io = assetManager != nullptr ? assetManager->OpenAssetIO(path) : SDL_IOFromFile(path.c_str(), "rb");
  if (!io) {
    Logger::Error("Failed to open '" + path + "': " + std::string(SDL_GetError()));
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
