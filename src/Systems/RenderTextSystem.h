#pragma once

#include "../Components/TextLabelComponent.h"
#include "../Renderer/RenderKey.h"
#include "../Renderer/RenderableType.h"

class RenderTextSystem {
 public:
  CommandBuffer& GetCommandBuffer() { return cmd_buffer_; }

  void operator()(const Entity entity, const TextLabelComponent& text) const {
    const RenderKey renderKey(text.layer, text.position.y, TEXT, entity);
    cmd_buffer_.QueueAddRenderKey(renderKey);
  }

 private:
  CommandBuffer cmd_buffer_;
};
