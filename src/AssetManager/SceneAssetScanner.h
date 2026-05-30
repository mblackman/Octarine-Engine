#pragma once

#include <sol/sol.hpp>

#include <string>
#include <unordered_set>
#include <vector>

#include "AssetManager/AssetReference.h"

// Derives the set of asset ids a scene needs from the scene's own data, so the engine no longer
// relies on a hand-maintained `assets = {}` list. Walks the scene's entity tree (mirroring
// LuaEntityLoader's stack walk) reading the asset-id fields off each component, plus the
// scene-level `tilemap.texture_asset_id` and an explicit `preload = { ids }` list.
//
// `preload` covers everything the static scan cannot see: runtime spawns (projectiles,
// click-spawned enemies) and load-time injection (Lua adding components to entity tables before
// they are loaded). It is the documented escape hatch, not zero-maintenance.
class SceneAssetScanner {
 public:
  // Every asset reference in the scene, with provenance and duplicates preserved.
  static std::vector<AssetReference> CollectRefs(const sol::table& scene);

  // The deduped required id set (convenience wrapper over CollectRefs).
  static std::unordered_set<std::string> Collect(const sol::table& scene);

 private:
  // Read the asset-id fields off a single entity node's `components` table into `out`,
  // tagging each with `entityContext` (e.g. "entity 'Player'").
  static void CollectFromComponents(const sol::table& components, const std::string& entityContext,
                                    std::vector<AssetReference>& out);
};
