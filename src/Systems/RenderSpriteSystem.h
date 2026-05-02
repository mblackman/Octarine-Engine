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
        const auto windowWidth = static_cast<float>(GameConfig::GetInstance().windowWidth);
        const auto windowHeight = static_cast<float>(GameConfig::GetInstance().windowHeight);
        isOutsideCamera = transform.globalPosition.x + sprite.width * transform.globalScale.x < 0 ||
                          transform.globalPosition.x > windowWidth ||
                          transform.globalPosition.y + sprite.height * transform.globalScale.y < 0 ||
                          transform.globalPosition.y > windowHeight;
      } else {
        isOutsideCamera = transform.globalPosition.x + sprite.width * transform.globalScale.x < camera.x ||
                          transform.globalPosition.x > camera.x + camera.w ||
                          transform.globalPosition.y + sprite.height * transform.globalScale.y < camera.y ||
                          transform.globalPosition.y > camera.y + camera.h;
      }

      if (!isOutsideCamera) {
        RenderKey renderKey(sprite.layer, transform.globalPosition.y, SPRITE, entity);

        renderQueue.AddRenderKey(renderKey);
      }
    }
  }
};