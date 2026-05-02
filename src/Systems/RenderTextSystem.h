#pragma once

#include "../Components/TextLabelComponent.h"
#include "../ECS/ECS.h"
#include "../Renderer/RenderKey.h"
#include "../Renderer/RenderQueue.h"
#include "../Renderer/RenderableType.h"

class RenderTextSystem : public System {
   public:
    RenderTextSystem() {
        RequireComponent<TextLabelComponent>();
    }

    ~RenderTextSystem() = default;

    void Update(RenderQueue& renderQueue) {
        auto entities = GetEntities();

        for (auto entity : entities) {
            auto text = entity.GetComponent<TextLabelComponent>();
            RenderKey renderKey(
                text.layer,
                text.position.y,
                RenderableType::TEXT,
                entity);

            renderQueue.AddRenderKey(renderKey);
        }
    }
};