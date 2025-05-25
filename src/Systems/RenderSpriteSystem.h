#pragma once

#include <SDL2/SDL.h>

#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"
#include "../Renderer/RenderKey.h"
#include "../Renderer/RenderQueue.h"
#include "../Renderer/RenderableType.h"

class RenderSpriteSystem : public System {
   public:
    RenderSpriteSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<SpriteComponent>();
    }

    ~RenderSpriteSystem() = default;

    void Update(RenderQueue& renderQueue, SDL_Rect& camera) {
        auto entities = GetEntities();

        for (auto entity : GetEntities()) {
            auto transform = entity.GetComponent<TransformComponent>();
            auto sprite = entity.GetComponent<SpriteComponent>();

            bool isOutsideCamera = false;

            if (sprite.isFixed) {
                isOutsideCamera = (transform.position.x + sprite.width * transform.scale.x < 0 ||
                                   transform.position.x > Game::windowWidth ||
                                   transform.position.y + sprite.height * transform.scale.y < 0 ||
                                   transform.position.y > Game::windowHeight);
            } else {
                isOutsideCamera = (transform.position.x + sprite.width * transform.scale.x < camera.x ||
                                   transform.position.x > camera.x + camera.w ||
                                   transform.position.y + sprite.height * transform.scale.y < camera.y ||
                                   transform.position.y > camera.y + camera.h);
            }

            if (!isOutsideCamera) {
                RenderKey renderKey(
                    sprite.layer,
                    transform.position.y,
                    RenderableType::SPRITE,
                    entity);

                renderQueue.AddRenderKey(renderKey);
            }
        }
    }
};