#pragma once

#include <string>

class AssetCatalog;

// Bake-time texture atlas packer. Walks the catalog for entries whose `atlas` field is set (and
// `no_atlas` is not), groups them by atlas name, packs each group into one square PNG written
// under `<basePath>/_atlases/<group>.png`, and mutates the member entries to record their slice
// inside the atlas (`atlasId` + `atlasSlice`). It also adds one new CatalogEntry per atlas group
// (id = `__atlas_<group>`, type = Texture) so the runtime can resolve the atlas as a normal
// loadable texture.
//
// Pure bake-time helper: never invoked in a running game, only from Game::RunBakeValidation. The
// stb_image + stb_image_write + stb_rect_pack implementation is compiled into the engine binary
// behind this single translation unit so the runtime cost is one extra .o on the link line.
class TextureAtlasBaker
{
public:
    // Default padding (transparent gutter) between packed sprites, in pixels. Avoids bilinear
    // bleed at atlas boundaries — most sprite-packs end up with this exact value.
    static constexpr int kDefaultPaddingPx = 2;

    // Default maximum atlas dimension. SDL3 GPU backends commonly cap at 4096 or 8192; 4096 is
    // the conservative default that survives every shipping target the engine claims. A group
    // whose sprites won't fit fails the bake (no silent fallback to multiple atlases — this
    // keeps the manifest one-atlas-per-group, and forces the project author to split or shrink
    // their art instead of silently producing 5x the textures).
    static constexpr int kDefaultMaxAtlasDim = 4096;

    TextureAtlasBaker() = default;

    // Pack every atlas group declared by `catalog`. Writes `<basePath>/_atlases/<group>.png` for
    // each, mutates member entries to point at the atlas, and inserts the new atlas entries.
    // Returns false on any image load failure, pack failure, or PNG write failure. Idempotent
    // across runs: the same input catalog + sources produce the same atlas bytes (member ordering
    // is the catalog's std::map id order, packing seeds are deterministic per stb_rect_pack).
    [[nodiscard]] bool Run(AssetCatalog& catalog, const std::string& basePath) const;
};
