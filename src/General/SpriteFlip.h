#pragma once

#include <cstdint>

namespace octarine {

// Mirror flags for sprite rendering. Bit layout matches SDL_FlipMode but the
// enum is backend-agnostic so component data doesn't depend on SDL headers.
enum class SpriteFlip : std::uint8_t {
  None = 0,
  Horizontal = 1u << 0,
  Vertical = 1u << 1,
  Both = Horizontal | Vertical,
};

}  // namespace octarine
