#include "./Renderer.h"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <glm/glm.hpp>
#include <memory>
#include <string>

#include "../AssetManager/AssetManager.h"
#include "../Components/SpriteComponent.h"
#include "../Components/SquarePrimitiveComponent.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/TransformComponent.h"
#include "../General/Logger.h"
#include "./RenderKey.h"
#include "./RenderQueue.h"
#include "./RenderableType.h"
#include "Components/CameraComponents.h"
#include "ECS/Registry.h"

Renderer::~Renderer() {
  for (auto& [key, entry] : text_cache_) {
    if (entry.texture) {
      SDL_DestroyTexture(entry.texture);
    }
  }
}

void Renderer::Render(const Registry* registry, SDL_Renderer* renderer) const {
  auto& renderQueue = registry->Get<RenderQueue>();

  for (const RenderKey& renderKey : renderQueue) {
    switch (const RenderableType type = renderKey.type) {
      case SPRITE:
        // Sprite data is pre-computed in the RenderKey — no GetComponent needed.
        RenderSprite(renderKey, renderer);
        break;
      case TEXT: {
        const Entity entity = renderKey.entity;
        if (!registry->IsAlive(entity)) continue;
        const auto camera = registry->Get<CameraComponent>().viewport;
        auto& assetManager = registry->Get<AssetManager>();
        RenderText(entity, registry, renderer, assetManager, camera);
        break;
      }
      case SQUARE_PRIMITIVE: {
        const Entity entity = renderKey.entity;
        if (!registry->IsAlive(entity)) continue;
        const auto camera = registry->Get<CameraComponent>().viewport;
        RenderSquare(entity, registry, renderer, camera);
        break;
      }
      default:
        Logger::Error("Unknown renderable type: " + std::to_string(type));
        break;
    }
  }
}

void Renderer::RenderSprite(const RenderKey& key, SDL_Renderer* renderer) {
  const SDL_FRect destRect = {key.destX, key.destY, key.destW, key.destH};
  SDL_RenderTextureRotated(renderer, key.texture, &key.srcRect, &destRect, key.rotation, nullptr, key.flip);
}

void Renderer::RenderSquare(const Entity& entity, const Registry* registry, SDL_Renderer* renderer,
                            const SDL_FRect& camera) {
  const auto& square = registry->GetComponent<SquarePrimitiveComponent>(entity);

  // Prefer global transform when present so squares parented to game entities track their parent.
  glm::vec2 origin = square.position;
  if (registry->HasComponent<TransformComponent>(entity)) {
    const auto& transform = registry->GetComponent<TransformComponent>(entity);
    origin += transform.globalPosition;
  }

  const float x = square.isFixed ? origin.x : origin.x - camera.x;
  const float y = square.isFixed ? origin.y : origin.y - camera.y;

  const SDL_FRect rect = {x, y, square.width, square.height};

  SDL_SetRenderDrawColor(renderer, square.color.r, square.color.g, square.color.b, square.color.a);
  SDL_RenderFillRect(renderer, &rect);
}

void Renderer::RenderText(const Entity& entity, const Registry* registry, SDL_Renderer* renderer,
                          const AssetManager& assetManager, const SDL_FRect& camera) const {
  const auto& textLabel = registry->GetComponent<TextLabelComponent>(entity);
  if (textLabel.text.empty()) return;

  TTF_Font* font = assetManager.GetFont(textLabel.fontId);
  if (!font) return;

  const auto packedColor = static_cast<Uint32>(textLabel.color.r) << 24 | static_cast<Uint32>(textLabel.color.g) << 16 |
                           static_cast<Uint32>(textLabel.color.b) << 8 | static_cast<Uint32>(textLabel.color.a);
  const std::string cacheKey = textLabel.fontId + "|" + textLabel.text + "|" + std::to_string(packedColor);

  auto it = text_cache_.find(cacheKey);
  if (it == text_cache_.end()) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, textLabel.text.c_str(), 0, textLabel.color);
    if (!surface) {
      Logger::Error("TTF_RenderText_Blended failed: " + std::string(SDL_GetError()));
      return;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    if (!texture) {
      Logger::Error("SDL_CreateTextureFromSurface failed: " + std::string(SDL_GetError()));
      return;
    }
    float w = 0;
    float h = 0;
    SDL_GetTextureSize(texture, &w, &h);
    it = text_cache_.emplace(cacheKey, TextCacheEntry{texture, w, h}).first;
  }

  glm::vec2 origin = textLabel.position;
  if (registry->HasComponent<TransformComponent>(entity)) {
    const auto& transform = registry->GetComponent<TransformComponent>(entity);
    origin += transform.globalPosition;
  }

  const SDL_FRect destRect = {origin.x - (textLabel.isFixed ? 0 : camera.x),
                              origin.y - (textLabel.isFixed ? 0 : camera.y), it->second.width, it->second.height};

  SDL_RenderTexture(renderer, it->second.texture, nullptr, &destRect);
}
