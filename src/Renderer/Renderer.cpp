#include "./Renderer.h"

#include <cmath>
#include <string>

#include "General/Constants.h"
#include "General/Logger.h"

namespace {
constexpr Uint8 kSceneClearGrey = 24;
constexpr Uint8 kWindowClearBlack = 0;
}  // namespace

bool Renderer::CreateScene(SDL_Renderer* sdlRenderer, const int width, const int height) {
  if (sdlRenderer == nullptr) return false;
  scene_texture_ = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
  if (scene_texture_ == nullptr) {
    Logger::Error("Renderer::CreateScene SDL_CreateTexture failed: " + std::string(SDL_GetError()));
    return false;
  }
  return true;
}

void Renderer::DestroyScene() {
  if (scene_texture_ != nullptr) {
    SDL_DestroyTexture(scene_texture_);
    scene_texture_ = nullptr;
  }
}

void Renderer::BeginScene(SDL_Renderer* sdlRenderer) const {
  SDL_SetRenderTarget(sdlRenderer, scene_texture_);
  SDL_SetRenderDrawColor(sdlRenderer, kSceneClearGrey, kSceneClearGrey, kSceneClearGrey, Constants::kUint8Max);
  SDL_RenderClear(sdlRenderer);
}

void Renderer::EndScene(SDL_Renderer* sdlRenderer) const {
  SDL_SetRenderTarget(sdlRenderer, nullptr);
  SDL_SetRenderDrawColor(sdlRenderer, kWindowClearBlack, kWindowClearBlack, kWindowClearBlack, Constants::kUint8Max);
  SDL_RenderClear(sdlRenderer);
}

void Renderer::CompositeSceneToWindow(SDL_Renderer* sdlRenderer) const {
  if (scene_texture_ != nullptr) {
    SDL_RenderTexture(sdlRenderer, scene_texture_, nullptr, nullptr);
  }
}

void Renderer::Present(SDL_Renderer* sdlRenderer) const { SDL_RenderPresent(sdlRenderer); }

void Renderer::DrawQueue(const RenderQueue& renderQueue, SDL_Renderer* renderer) const {
  for (const RenderKey& key : renderQueue) {
    switch (key.type) {
      case SPRITE: {
        const auto& cmd = key.payload.sprite;
        const SDL_FRect destRect = {cmd.destX, cmd.destY, cmd.destW, cmd.destH};
        const auto deg = static_cast<float>(cmd.rotation * (180.0 / 3.14159265358979323846));
        SDL_RenderTextureRotated(renderer, cmd.texture, &cmd.srcRect, &destRect, deg, &cmd.pivot, cmd.flip);
        break;
      }
      case SQUARE_PRIMITIVE: {
        const auto& cmd = key.payload.square;
        if (cmd.rotation == 0.0) {
          SDL_SetRenderDrawColor(renderer, cmd.color.r, cmd.color.g, cmd.color.b, cmd.color.a);
          SDL_RenderFillRect(renderer, &cmd.destRect);
        } else {
          const float cx = cmd.destRect.x + cmd.destRect.w * 0.5f;
          const float cy = cmd.destRect.y + cmd.destRect.h * 0.5f;
          const float hx = cmd.destRect.w * 0.5f;
          const float hy = cmd.destRect.h * 0.5f;
          const auto c = static_cast<float>(std::cos(cmd.rotation));
          const auto s = static_cast<float>(std::sin(cmd.rotation));
          const SDL_FColor fcol = {cmd.color.r / 255.0f, cmd.color.g / 255.0f, cmd.color.b / 255.0f,
                                   cmd.color.a / 255.0f};
          const SDL_FPoint corners[4] = {
              {cx + (-hx) * c - (-hy) * s, cy + (-hx) * s + (-hy) * c},
              {cx + (hx)*c - (-hy) * s, cy + (hx)*s + (-hy) * c},
              {cx + (hx)*c - (hy)*s, cy + (hx)*s + (hy)*c},
              {cx + (-hx) * c - (hy)*s, cy + (-hx) * s + (hy)*c},
          };
          SDL_Vertex verts[4];
          for (int i = 0; i < 4; ++i) {
            verts[i].position = corners[i];
            verts[i].color = fcol;
            verts[i].tex_coord = {0.0f, 0.0f};
          }
          const int indices[6] = {0, 1, 2, 0, 2, 3};
          SDL_RenderGeometry(renderer, nullptr, verts, 4, indices, 6);
        }
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
