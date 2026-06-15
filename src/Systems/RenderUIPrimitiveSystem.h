#pragma once

#include <SDL3/SDL.h>

#include "Components/SquarePrimitiveComponent.h"
#include "Components/UIRectComponent.h"
#include "ECS/Iterable.h"
#include "ECS/Registry.h"
#include "General/BlendMode.h"
#include "Renderer/RenderQueue.h"

// Renders SquarePrimitiveComponent entities whose rectangle is driven by UIRectComponent
// (layout-driven colored panels). Registered serially after UILayoutSystem. World squares without
// UIRectComponent continue through the parallel RenderPrimitiveSystem path unchanged; the two
// queries are disjoint because UI nodes carry no GlobalTransformComponent.
//
// The RenderQueue is fetched per-call via the ContextFacade, the same way RenderTextSystem does:
// the serial RegisterSystem wrapper does not invoke a Prepare() hook (only the parallel wrapper
// does), so caching it in a member would leave it null.
class RenderUIPrimitiveSystem {
 public:
  void operator()(const ContextFacade& ctx, const UIRectComponent& rect,
                  const SquarePrimitiveComponent& square) const {
    // Position and size come from the resolved rect; color and blend come from the square. Layer
    // is the rect's computed layer (canvas base + accumulated z), matching RenderUISpriteSystem.
    auto& renderQueue = ctx.GetRegistry()->Get<RenderQueue>();
    auto& cmd = renderQueue.EmplaceSquare(static_cast<unsigned int>(rect.layer), rect.top, nullptr, square.blendMode);
    cmd.destRect = {rect.left, rect.top, rect.Width(), rect.Height()};
    cmd.color = SDL_Color{square.color.r, square.color.g, square.color.b, square.color.a};
    cmd.rotation = 0.0;
    cmd.blendMode = octarine::ToSdlBlendMode(square.blendMode);
  }
};
