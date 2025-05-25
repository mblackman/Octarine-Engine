#include "./Renderer.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <algorithm>
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

void Renderer::Render(const RenderQueue& renderQueue, SDL_Renderer* renderer, SDL_Rect& camera, std::unique_ptr<AssetManager>& assetManager) {
    for (const RenderKey& renderKey : renderQueue) {
        const Entity entity = renderKey.entity;
        const RenderableType type = renderKey.type;

        switch (type) {
            case RenderableType::SPRITE:
                RenderSprite(entity, renderer, assetManager, camera);
                break;
            case RenderableType::TEXT:
                RenderText(entity, renderer, assetManager, camera);
                break;
            case RenderableType::SQUARE_PRIMITIVE:
                RenderSquare(entity, renderer, camera);
                break;
            default:
                Logger::Error("Unknown renderable type: " + std::to_string(static_cast<int>(type)));
                break;
        }
    }
}

void Renderer::RenderSprite(const Entity& entity, SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager, SDL_Rect& camera) {
    const auto transform = entity.GetComponent<TransformComponent>();
    const auto sprite = entity.GetComponent<SpriteComponent>();

    const auto texture = assetManager->GetTexture(sprite.assetId);
    float x = sprite.isFixed ? transform.position.x : transform.position.x - camera.x;
    float y = sprite.isFixed ? transform.position.y : transform.position.y - camera.y;

    SDL_Rect destRect = {
        static_cast<int>(x),
        static_cast<int>(y),
        static_cast<int>(sprite.width * transform.scale.x),
        static_cast<int>(sprite.height * transform.scale.y)};

    SDL_RenderCopyEx(renderer, texture, &sprite.srcRect, &destRect, transform.rotation, nullptr, sprite.flip);
}

void Renderer::RenderSquare(const Entity& entity, SDL_Renderer* renderer, SDL_Rect& camera) {
    auto square = entity.GetComponent<SquarePrimitiveComponent>();
    float x = square.isFixed ? square.position.x : square.position.x - camera.x;
    float y = square.isFixed ? square.position.y : square.position.y - camera.y;

    SDL_Rect rect = {
        static_cast<int>(x),
        static_cast<int>(y),
        square.width,
        square.height};

    SDL_SetRenderDrawColor(renderer, square.color.r, square.color.g, square.color.b, square.color.a);
    SDL_RenderFillRect(renderer, &rect);
}

void Renderer::RenderText(const Entity& entity, SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager, SDL_Rect& camera) {
    const auto textLabel = entity.GetComponent<TextLabelComponent>();
    const auto font = assetManager->GetFont(textLabel.fontId);
    SDL_Surface* surface = TTF_RenderText_Blended(
        font,
        textLabel.text.c_str(),
        textLabel.color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    int labelWidth = 0;
    int labelHeight = 0;

    SDL_QueryTexture(texture, nullptr, nullptr, &labelWidth, &labelHeight);

    SDL_Rect destRect = {
        static_cast<int>(textLabel.position.x - (textLabel.isFixed ? 0 : camera.x)),
        static_cast<int>(textLabel.position.y - (textLabel.isFixed ? 0 : camera.y)),
        labelWidth,
        labelHeight};

    SDL_RenderCopy(renderer, texture, nullptr, &destRect);
}