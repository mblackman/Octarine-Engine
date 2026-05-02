#pragma once

#include "../Components/TextLabelComponent.h"
#include "../Renderer/RenderKey.h"
#include "../Renderer/RenderQueue.h"
#include "../Renderer/RenderableType.h"
#include "ECS/Iterable.h"
#include "ECS/Registry.h"

class RenderTextSystem {
 public:
  void operator()(const ContextFacade& context, const TextLabelComponent& text) const {
    auto& renderQueue = context.Registry()->Get<RenderQueue>();
    const RenderKey renderKey(text.layer, text.position.y, TEXT, context.Entity());
    renderQueue.AddRenderKey(renderKey);
  }
};
