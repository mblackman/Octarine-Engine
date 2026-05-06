#include "./Renderer.h"

#include <cmath>

#include "General/Logger.h"

void Renderer::Render(const RenderQueue& renderQueue, SDL_Renderer* renderer) const {
  for (const RenderKey& key : renderQueue) {
    switch (key.type) {
      case SPRITE: {
        const auto& cmd = key.payload.sprite;
        const SDL_FRect destRect = {cmd.destX, cmd.destY, cmd.destW, cmd.destH};
        const double deg = cmd.rotation * (180.0 / 3.14159265358979323846);
        SDL_RenderTextureRotated(renderer, cmd.texture, &cmd.srcRect, &destRect, deg, &cmd.pivot, cmd.flip);
        break;
      }
      case SQUARE_PRIMITIVE: {
        const auto& cmd = key.payload.square;
        SDL_SetRenderDrawColor(renderer, cmd.color.r, cmd.color.g, cmd.color.b, cmd.color.a);
        SDL_RenderFillRect(renderer, &cmd.destRect);
        break;
      }
      case TEXT: {
        const auto& cmd = key.payload.text;
        SDL_RenderTexture(renderer, cmd.texture, nullptr, &cmd.destRect);
        break;
      }
      default:
        Logger::Error("Unknown renderable type");
        break;
    }
  }
}
