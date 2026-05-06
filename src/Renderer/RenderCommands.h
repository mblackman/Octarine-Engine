#pragma once

#include <SDL3/SDL.h>

struct SpriteCommand {
  float destX{}, destY{}, destW{}, destH{};
  SDL_FRect srcRect{};
  double rotation{};
  SDL_FPoint pivot{};
  SDL_FlipMode flip{};
  SDL_Texture* texture{};
};

struct SquareCommand {
  SDL_FRect destRect{};
  SDL_Color color{};
};

struct TextCommand {
  SDL_FRect destRect{};
  SDL_Texture* texture{};
};
