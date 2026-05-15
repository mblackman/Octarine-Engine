#pragma once

#include <SDL3/SDL.h>

// Renderable viewport-cull test shared by all Render*System producers. World-space items
// are tested against the camera rect; screen-space (isFixed) items against the window rect.
// AABB params describe the renderable's top-left + size in its native space (world or screen).
inline bool IsRenderableOutsideViewport(const float originX, const float originY, const float width, const float height,
                                        const bool isFixed, const SDL_FRect& camera, const float windowWidth,
                                        const float windowHeight) {
  if (isFixed) {
    return originX + width < 0 || originX > windowWidth || originY + height < 0 || originY > windowHeight;
  }
  return originX + width < camera.x || originX > camera.x + camera.w || originY + height < camera.y ||
         originY > camera.y + camera.h;
}
