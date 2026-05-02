#pragma once

#include "../Components/SquarePrimitiveComponent.h"
#include "../Renderer/RenderKey.h"
#include "../Renderer/RenderQueue.h"
#include "../Renderer/RenderableType.h"
#include "ECS/Iterable.h"
#include "ECS/Registry.h"

class RenderPrimitiveSystem {
 public:
  void operator()(const ContextFacade& context, const SquarePrimitiveComponent& square) const {
    auto& renderQueue = context.Registry()->Get<RenderQueue>();
    const RenderKey renderKey(square.layer, square.position.y, SQUARE_PRIMITIVE, context.Entity());
    renderQueue.AddRenderKey(renderKey);
  }
};
