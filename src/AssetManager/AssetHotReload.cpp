#include "AssetManager/AssetHotReload.h"

#ifndef OCTARINE_SHIPPED

#include <filesystem>
#include <system_error>

#include "AssetManager/AssetManager.h"
#include "General/Logger.h"

namespace {
std::string Canonicalize(const std::string& path) {
  std::error_code ec;
  auto canon = std::filesystem::weakly_canonical(std::filesystem::path(path), ec);
  if (ec) {
    return path;
  }
  return canon.string();
}
}  // namespace

void AssetHotReload::Tick(AssetManager& assets, SDL_Renderer* renderer, MIX_Mixer* mixer, float dt, float pollSeconds) {
  accumulator_ += dt;
  if (accumulator_ < pollSeconds) {
    return;
  }
  accumulator_ = 0.0F;

  RefreshTracking(assets);
  if (watcher_.Empty()) {
    return;
  }
  for (const auto& dirty : watcher_.Poll()) {
    const int reloaded = assets.ReloadByPath(dirty, renderer, mixer);
    if (reloaded > 0) {
      last_.path = dirty;
      ++last_.totalReloads;
      last_.totalIdsReloaded += reloaded;
      Logger::Info("Asset hot reload: " + dirty + " (" + std::to_string(reloaded) + " ids)");
    }
  }
}

void AssetHotReload::ForceReloadAll(AssetManager& assets, SDL_Renderer* renderer, MIX_Mixer* mixer) {
  for (const auto& path : assets.ResidentSourcePaths()) {
    const int reloaded = assets.ReloadByPath(path, renderer, mixer);
    if (reloaded > 0) {
      last_.path = path;
      ++last_.totalReloads;
      last_.totalIdsReloaded += reloaded;
    }
  }
}

void AssetHotReload::RefreshTracking(AssetManager& assets) {
  // Canonicalize so the keys match what FileWatcher::Poll returns and what tracked_ stores.
  std::unordered_set<std::string> seen;
  for (const auto& path : assets.ResidentSourcePaths()) {
    seen.insert(Canonicalize(path));
  }

  for (const auto& path : seen) {
    if (tracked_.find(path) == tracked_.end()) {
      watcher_.Track(path);
      tracked_.insert(path);
    }
  }
  for (auto it = tracked_.begin(); it != tracked_.end();) {
    if (seen.find(*it) == seen.end()) {
      watcher_.Untrack(*it);
      it = tracked_.erase(it);
    } else {
      ++it;
    }
  }
}

#endif  // OCTARINE_SHIPPED
