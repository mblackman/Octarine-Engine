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
#include "../ECS/ECS.h"
#include "../General/Logger.h"
#include "./RenderKey.h"
#include "./RenderQueue.h"
#include "./RenderableType.h"

void Renderer::Render(const RenderQueue& renderQueue, SDL_Renderer* renderer, const SDL_FRect& camera,
                      const std::unique_ptr<AssetManager>& assetManager) const {
  for (const RenderKey& renderKey : renderQueue) {
    const Entity entity = renderKey.entity;
    const RenderableType type = renderKey.type;

    switch (type) {
      case SPRITE:
        RenderSprite(entity, renderer, assetManager, camera);
        break;
      case TEXT:
        RenderText(entity, renderer, assetManager, camera);
        break;
      case SQUARE_PRIMITIVE:
        RenderSquare(entity, renderer, camera);
        break;
      default:
        Logger::Error("Unknown renderable type: " + std::to_string(type));
        break;
    }
  }
}

void Renderer::RenderSprite(const Entity& entity, SDL_Renderer* renderer,
                            const std::unique_ptr<AssetManager>& assetManager, const SDL_FRect& camera) {
  const auto transform = entity.GetComponent<TransformComponent>();
  const auto& sprite = entity.GetComponent<SpriteComponent>();

  const auto texture = assetManager->GetTexture(sprite.assetId);
  const float x = sprite.isFixed ? transform.position.x : transform.position.x - camera.x;
  const float y = sprite.isFixed ? transform.position.y : transform.position.y - camera.y;

  const SDL_FRect destRect = {x, y, sprite.width * transform.scale.x, sprite.height * transform.scale.y};

  SDL_RenderTextureRotated(renderer, texture, &sprite.srcRect, &destRect, transform.rotation, nullptr, sprite.flip);
}

void Renderer::RenderSquare(const Entity& entity, SDL_Renderer* renderer, const SDL_FRect& camera) {
  const auto square = entity.GetComponent<SquarePrimitiveComponent>();
  const float x = square.isFixed ? square.position.x : square.position.x - camera.x;
  const float y = square.isFixed ? square.position.y : square.position.y - camera.y;

  const SDL_FRect rect = {x, y, square.width, square.height};

  SDL_SetRenderDrawColor(renderer, square.color.r, square.color.g, square.color.b, square.color.a);
  SDL_RenderFillRect(renderer, &rect);
}

void Renderer::RenderText(const Entity& entity, SDL_Renderer* renderer,
                          const std::unique_ptr<AssetManager>& assetManager, const SDL_FRect& camera) {
  const auto& textLabel = entity.GetComponent<TextLabelComponent>();
  const auto font = assetManager->GetFont(textLabel.fontId);
  SDL_Surface* surface = TTF_RenderText_Blended(font, textLabel.text.c_str(), 0, textLabel.color);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_DestroySurface(surface);

  float labelWidth = 0;
  float labelHeight = 0;

  SDL_GetTextureSize(texture, &labelWidth, &labelHeight);

  const SDL_FRect destRect = {textLabel.position.x - (textLabel.isFixed ? 0 : camera.x),
                              textLabel.position.y - (textLabel.isFixed ? 0 : camera.y), labelWidth, labelHeight};

  SDL_RenderTexture(renderer, texture, nullptr, &destRect);
}