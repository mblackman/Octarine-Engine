#pragma once

#include <SDL2/SDL.h>

#include "../Components/CameraFollowComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"
#include "../General/Logger.h"

class CameraFollowSystem : public System {
   public:
    CameraFollowSystem() {
        RequireComponent<CameraFollowComponent>();
        RequireComponent<TransformComponent>();
    }

    void Update(SDL_Rect& camera) {
        for (auto entity : GetEntities()) {
            auto transform = entity.GetComponent<TransformComponent>();

            if (transform.position.x + (camera.w / 2) < Game::mapWidth) {
                camera.x = transform.position.x - (Game::windowWidth / 2);
            }

            if (transform.position.y + (camera.h / 2) < Game::mapHeight) {
                camera.y = transform.position.y - (Game::windowHeight / 2);
            }

            camera.x = std::max(0, camera.x);
            camera.y = std::max(0, camera.y);
            camera.x = std::min(camera.w, camera.x);
            camera.y = std::min(camera.h, camera.y);
        }
    }
};