#pragma once

#include "../Components/SquarePrimitiveComponent.h"
#include "../ECS/ECS.h"
#include "../Renderer/RenderKey.h"
#include "../Renderer/RenderQueue.h"
#include "../Renderer/RenderableType.h"

class RenderPrimitiveSystem : public System {
   public:
    RenderPrimitiveSystem() {
    }

    ~RenderPrimitiveSystem() = default;

    void Update(RenderQueue& renderQueue) {
        auto entities = GetEntities();

        for (auto entity : entities) {
            if (entity.HasComponent<SquarePrimitiveComponent>()) {
                auto square = entity.GetComponent<SquarePrimitiveComponent>();
                RenderKey renderKey(
                    square.layer,
                    square.position.y,
                    RenderableType::SQUARE_PRIMITIVE,
                    entity);

                renderQueue.AddRenderKey(renderKey);
            }
        }
    }
};