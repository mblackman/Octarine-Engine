#include "Engine/Platform/PlatformPaths.h"

#include <SDL3/SDL.h>

#include "General/Logger.h"

namespace engine_bootstrap::platform {

void ApplyDefaultBasePath(std::string& effectivePath) {
#ifndef __ANDROID__
  if (effectivePath.empty()) {
    if (const char* basePath = SDL_GetBasePath()) {
      effectivePath = basePath;
      Logger::Info("No project path provided; defaulting asset base path to: " + effectivePath);
    }
  }
#else
  (void)effectivePath;
  Logger::Info("Android: resolving assets from the APK asset root (empty base path).");
#endif
}

bool ShouldAttemptProjectLoad(const std::string& effectivePath) {
#ifdef __ANDROID__
  (void)effectivePath;
  return true;  // empty base → APK title storage
#else
  return !effectivePath.empty();
#endif
}

}  // namespace engine_bootstrap::platform
