#pragma once

#include <string>

// A single reference to an asset id plus a human-readable description of where it came from
// (e.g. "entity 'Player' sprite.texture_asset_id"). Produced by SceneAssetScanner and consumed by
// AssetManager validation so a bad id can be reported against the entity/field that named it.
// Kept in its own header (no sol dependency) so AssetManager need not include sol2.
struct AssetReference {
  std::string id;
  std::string context;
};
