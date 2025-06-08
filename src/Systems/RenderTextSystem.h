#pragma once

#include "../Components/TextLabelComponent.h"
#include "../ECS/ECS.h"
#include "../Renderer/RenderKey.h"
#include "../Renderer/RenderQueue.h"
#include "../Renderer/RenderableType.h"

class RenderTextSystem : public System {
 public:
  RenderTextSystem() { RequireComponent<TextLabelComponent>(); }

  RenderTextSystem(const RenderTextSystem&) = delete;
  RenderTextSystem& operator=(const RenderTextSystem&) = delete;

  RenderTextSystem(RenderTextSystem&&) = delete;
  RenderTextSystem& operator=(RenderTextSystem&&) = delete;

  ~RenderTextSystem() = default;

  void Update(RenderQueue& renderQueue) {
    const auto entities = GetEntities();

    for (auto entity : entities) {
      const auto text = entity.GetComponent<TextLabelComponent>();
      RenderKey renderKey(text.layer, text.position.y, TEXT,
                          entity);

      renderQueue.AddRenderKey(renderKey);
    }
  }
};