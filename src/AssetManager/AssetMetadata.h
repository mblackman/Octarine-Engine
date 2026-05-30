#pragma once

#include <optional>
#include <string>
#include <vector>

// Per-file asset metadata, parsed from an optional `<file>.meta` Lua sidecar. A missing sidecar
// means "all defaults". Each struct owns an `applyDefaults(...)` that fills any unset field from
// project-wide defaults (GameConfig) and the constants below, so downstream code always sees a
// fully-resolved struct. These types depend on nothing engine-specific (no SDL) so they can be
// unit-tested in isolation.

// Texture scale filtering. Mirrors SDL_ScaleMode but kept engine-side so the metadata layer does
// not pull in SDL headers. Converted to the engine's string form via toScaleModeString.
enum class ScaleMode { Nearest, Linear };

// Glyph size baked when a font sidecar omits `sizes`.
inline constexpr float kDefaultFontSize = 16.0F;

inline const char* toScaleModeString(ScaleMode mode) { return mode == ScaleMode::Linear ? "linear" : "nearest"; }

struct TextureMeta {
  std::optional<std::string> id;       // override the derived id (filename stem)
  std::optional<ScaleMode> scaleMode;  // nearest/linear; falls back to the project default
  std::optional<std::string> atlas;    // atlas group name ; nullopt = loose texture
  bool noAtlas{false};                 // meta.no_atlas = true ; never folded into an atlas even if
                                       // a sibling pulls the group via atlas=. For pixel-art that
                                       // hates packing bleed at sub-pixel src_rect math.

  // `defaultScaleMode` is the GameConfig DefaultScalingMode (nullopt -> nearest). An explicit
  // sidecar value always wins.
  void applyDefaults(std::optional<ScaleMode> defaultScaleMode) {
    if (!scaleMode.has_value()) {
      scaleMode = defaultScaleMode.value_or(ScaleMode::Nearest);
    }
  }
};

struct FontMeta {
  std::optional<std::string> id;  // override the derived id base (filename stem)
  std::vector<float> sizes;       // one catalog entry per size; defaults to { kDefaultFontSize }

  void applyDefaults() {
    if (sizes.empty()) {
      sizes.push_back(kDefaultFontSize);
    }
  }
};

struct AudioMeta {
  std::optional<std::string> id;  // override the derived id (filename stem)
  bool stream{false};             // stream from disk vs full decode 

  void applyDefaults() {}  // `stream` already carries its default
};
