#pragma once

#include <SDL3/SDL.h>

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

  RenderSpriteSystem(const RenderSpriteSystem&) = delete;
  RenderSpriteSystem& operator=(const RenderSpriteSystem&) = delete;

  RenderSpriteSystem(RenderSpriteSystem&&) = delete;
  RenderSpriteSystem& operator=(RenderSpriteSystem&&) = delete;

  ~RenderSpriteSystem() = default;

  void Update(RenderQueue& renderQueue, const SDL_FRect& camera) const {
    auto entities = GetEntities();

    for (auto entity : GetEntities()) {
      const auto transform = entity.GetComponent<TransformComponent>();
      const auto& sprite = entity.GetComponent<SpriteComponent>();

      bool isOutsideCamera = false;

      if (sprite.isFixed) {
        isOutsideCamera = transform.position.x + sprite.width * transform.scale.x < 0 ||
                          transform.position.x > static_cast<float>(Game::windowWidth) ||
                          transform.position.y + sprite.height * transform.scale.y < 0 ||
                          transform.position.y > static_cast<float>(Game::windowHeight);
      } else {
        isOutsideCamera = transform.position.x + sprite.width * transform.scale.x < camera.x ||
                          transform.position.x > camera.x + camera.w ||
                          transform.position.y + sprite.height * transform.scale.y < camera.y ||
                          transform.position.y > camera.y + camera.h;
      }

      if (!isOutsideCamera) {
        RenderKey renderKey(sprite.layer, transform.position.y, SPRITE, entity);

        renderQueue.AddRenderKey(renderKey);
      }
    }
  }
};