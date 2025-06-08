#pragma once

#include "../Components/SquarePrimitiveComponent.h"
#include "../ECS/ECS.h"
#include "../Renderer/RenderKey.h"
#include "../Renderer/RenderQueue.h"
#include "../Renderer/RenderableType.h"

class RenderPrimitiveSystem : public System {
 public:
  RenderPrimitiveSystem() = default;

  RenderPrimitiveSystem(const RenderPrimitiveSystem&) = delete;
  RenderPrimitiveSystem& operator=(const RenderPrimitiveSystem&) = delete;

  RenderPrimitiveSystem(RenderPrimitiveSystem&&) = delete;
  RenderPrimitiveSystem& operator=(RenderPrimitiveSystem&&) = delete;

  ~RenderPrimitiveSystem() = default;

  void Update(RenderQueue& renderQueue) {
    const auto entities = GetEntities();

    for (auto entity : entities) {
      if (entity.HasComponent<SquarePrimitiveComponent>()) {
        const auto square = entity.GetComponent<SquarePrimitiveComponent>();
        RenderKey renderKey(square.layer, square.position.y,
                            SQUARE_PRIMITIVE, entity);

        renderQueue.AddRenderKey(renderKey);
      }
    }
  }
};