#pragma once

#include "../Components/SquarePrimitiveComponent.h"
#include "../Renderer/RenderKey.h"
#include "../Renderer/RenderableType.h"

class RenderPrimitiveSystem {
 public:
  CommandBuffer& GetCommandBuffer() { return cmd_buffer_; }

  void operator()(const Entity entity, const SquarePrimitiveComponent& square) const {
    const RenderKey renderKey(square.layer, square.position.y, SQUARE_PRIMITIVE, entity);
    cmd_buffer_.QueueAddRenderKey(renderKey);
  }

 private:
  CommandBuffer cmd_buffer_;
};
