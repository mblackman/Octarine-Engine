#pragma once

#include <string>

// Platform-specific asset base-path resolution. Concentrates the __ANDROID__ vs desktop forks that
// used to sit inline in Game::Initialize so Game.cpp carries none: the selection happens on
// __ANDROID__ inside the .cpp.
namespace engine_bootstrap::platform {

// When `effectivePath` is empty, fill it with the platform default and log the choice; no-op when
// already set. Desktop: the executable/bundle dir (SDL_GetBasePath) — the exe dir on a plain
// desktop build, Contents/Resources in a .app, the bundle root on iOS. Android: left empty, since
// assets live inside the APK and resolve through SDL title storage against *relative* paths
// (SDL_GetBasePath there returns the app data dir, not the APK).
void ApplyDefaultBasePath(std::string& effectivePath);

// Whether to attempt loading a project config from `effectivePath`. Desktop: only when non-empty.
// Android: always — the empty base resolves against the APK asset root via title storage.
[[nodiscard]] bool ShouldAttemptProjectLoad(const std::string& effectivePath);

}  // namespace engine_bootstrap::platform
