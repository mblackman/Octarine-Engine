#pragma once

#include <SDL2/SDL.h>

#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"
#include "../General/Logger.h"

class DrawColliderSystem : public System {
   public:
    DrawColliderSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<BoxColliderComponent>();
    }

    ~DrawColliderSystem() = default;

    void Update(SDL_Renderer* renderer, SDL_Rect& camera) {
        for (auto entity : GetEntities()) {
            const auto& transform = entity.GetComponent<TransformComponent>();
            const auto& collider = entity.GetComponent<BoxColliderComponent>();

            const SDL_FRect rect = {
                transform.position.x - camera.x,
                transform.position.y - camera.y,
                collider.width * transform.scale.x,
                collider.height * transform.scale.y};

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawRectF(renderer, &rect);
        }
    }
};