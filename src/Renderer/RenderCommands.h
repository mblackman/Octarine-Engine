#pragma once

#include <SDL3/SDL.h>

#include "General/BlendMode.h"

namespace octarine {

// Boundary translation from the backend-agnostic component enum to SDL's sparse constants.
inline SDL_BlendMode ToSdlBlendMode(const BlendMode mode) {
  switch (mode) {
    case BlendMode::None:
      return SDL_BLENDMODE_NONE;
    case BlendMode::Add:
      return SDL_BLENDMODE_ADD;
    case BlendMode::Mod:
      return SDL_BLENDMODE_MOD;
    case BlendMode::Mul:
      return SDL_BLENDMODE_MUL;
    case BlendMode::Blend:
    default:
      return SDL_BLENDMODE_BLEND;
  }
}

}  // namespace octarine

struct SpriteCommand {
  float destX{}, destY{}, destW{}, destH{};
  SDL_FRect srcRect{};
  double rotation{};
  SDL_FPoint pivot{};
  SDL_FlipMode flip{};
  SDL_Texture* texture{};
  // Per-draw modulation: rgb tints (multiplies) the texture, alpha fades it. 255 = no-op.
  SDL_Color colorMod{255, 255, 255, 255};
  SDL_BlendMode blendMode{SDL_BLENDMODE_BLEND};
};

struct SquareCommand {
  SDL_FRect destRect{};
  SDL_Color color{};
  double rotation{};
  SDL_BlendMode blendMode{SDL_BLENDMODE_BLEND};
};

struct TextCommand {
  SDL_FRect destRect{};
  SDL_Texture* texture{};
};
