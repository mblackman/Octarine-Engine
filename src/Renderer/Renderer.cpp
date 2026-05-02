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
        RenderText(entity, renderer, assetManager, camera);
        break;
      }
      case SQUARE_PRIMITIVE: {
        const Entity entity = renderKey.entity;
        if (!registry->IsAlive(entity)) continue;
        const auto camera = registry->Get<CameraComponent>().viewport;
        RenderSquare(entity, renderer, camera);
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

void Renderer::RenderSquare(const Entity& entity, SDL_Renderer* renderer, const SDL_FRect& camera) {
  // const auto& transform = entity.GetComponent<TransformComponent>();
  // const auto square = entity.GetComponent<SquarePrimitiveComponent>();
  // const float positionX = transform.globalPosition.x + square.position.x;
  // const float positionY = transform.globalPosition.y + square.position.y;
  // const float x = square.isFixed ? positionX : positionX - camera.x;
  // const float y = square.isFixed ? positionY : positionY - camera.y;
  //
  // const SDL_FRect rect = {x, y, square.width, square.height};
  //
  // SDL_SetRenderDrawColor(renderer, square.color.r, square.color.g, square.color.b, square.color.a);
  // SDL_RenderFillRect(renderer, &rect);
}

void Renderer::RenderText(const Entity& entity, SDL_Renderer* renderer, const AssetManager& assetManager,
                          const SDL_FRect& camera) {
  // const auto transform = entity.GetComponent<TransformComponent>();
  // const auto& textLabel = entity.GetComponent<TextLabelComponent>();
  // const auto font = assetManager->GetFont(textLabel.fontId);
  // SDL_Surface* surface = TTF_RenderText_Blended(font, textLabel.text.c_str(), 0, textLabel.color);
  // SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  // SDL_DestroySurface(surface);
  //
  // float labelWidth = 0;
  // float labelHeight = 0;
  // const float positionX = transform.globalPosition.x + textLabel.position.x;
  // const float positionY = transform.globalPosition.y + textLabel.position.y;
  //
  // SDL_GetTextureSize(texture, &labelWidth, &labelHeight);
  //
  // const SDL_FRect destRect = {positionX - (textLabel.isFixed ? 0 : camera.x),
  //                             positionY - (textLabel.isFixed ? 0 : camera.y), labelWidth, labelHeight};
  //
  // SDL_RenderTexture(renderer, texture, nullptr, &destRect);
}