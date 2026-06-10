#pragma once

#include <cstdint>
#include <string_view>

namespace octarine {

// Blend modes for sprite / primitive rendering. Values are a compact 0..N enum (3 bits —
// RenderKey packs them into the sort key) rather than SDL's sparse bitmask constants, so
// component data doesn't depend on SDL headers. Mapping to SDL_BlendMode happens at the
// renderer boundary.
enum class BlendMode : std::uint8_t {
  None = 0,  // No blending: dst = src
  Blend,     // Alpha blending: dst = src*srcA + dst*(1-srcA)
  Add,       // Additive: dst = src*srcA + dst
  Mod,       // Color modulate: dst = src*dst
  Mul,       // Color multiply: dst = src*dst + dst*(1-srcA)
};

constexpr const char* ToString(const BlendMode mode) {
  switch (mode) {
    case BlendMode::None:
      return "none";
    case BlendMode::Add:
      return "add";
    case BlendMode::Mod:
      return "mod";
    case BlendMode::Mul:
      return "mul";
    case BlendMode::Blend:
    default:
      return "blend";
  }
}

constexpr BlendMode BlendModeFromString(const std::string_view name, const BlendMode fallback = BlendMode::Blend) {
  if (name == "none") return BlendMode::None;
  if (name == "blend") return BlendMode::Blend;
  if (name == "add") return BlendMode::Add;
  if (name == "mod") return BlendMode::Mod;
  if (name == "mul") return BlendMode::Mul;
  return fallback;
}

}  // namespace octarine
