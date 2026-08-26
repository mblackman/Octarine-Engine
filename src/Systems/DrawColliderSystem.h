#pragma once

#include <SDL3/SDL.h>

#include <cmath>

#include "Components/BoxColliderComponent.h"
#include "Components/CameraComponents.h"
#include "Components/GlobalTransformComponent.h"
#include "ECS/Iterable.h"
#include "ECS/Registry.h"
#include "Engine/EngineContext.h"
#include "General/Constants.h"
#include "General/Rotation2D.h"

class DrawColliderSystem {
 public:
  void operator()(const ContextFacade& context, const GlobalTransformComponent& transform,
                  const BoxColliderComponent& collider) const {
    auto* renderer = context.GetRegistry()->Get<EngineContext>().sdlRenderer;
    const auto& camera = context.GetRegistry()->Get<CameraComponent>().viewport;

    const float w = static_cast<float>(collider.width) * transform.scale.x;
    const float h = static_cast<float>(collider.height) * transform.scale.y;
    const float hx = w * 0.5f;
    const float hy = h * 0.5f;

    // transform.position is top-left. apply collider offset (scaled).
    glm::vec2 centre = {transform.position.x + collider.offset.x * transform.scale.x + hx,
                        transform.position.y + collider.offset.y * transform.scale.y + hy};
    glm::vec2 origin = transform.position + transform.pivot;

    if (!collider.isFixed) {
      const glm::vec2 cameraOffset = {camera.x, camera.y};
      centre -= cameraOffset;
      origin -= cameraOffset;
    }

    SDL_SetRenderDrawColor(renderer, Constants::kUint8Max, 0, 0, Constants::kUint8Max);

    const octarine::Rotation2D rot = octarine::Rotation2D::FromRadians(transform.rotation);
    if (rot.IsIdentity()) {
      const SDL_FRect rect = {centre.x - hx, centre.y - hy, w, h};
      SDL_RenderRect(renderer, &rect);
      return;
    }

    // Match CollisionSystem: orbit the box centre about `position + pivot` before drawing the
    // OBB, so the debug outline lands on the box the broadphase actually tested.
    centre = octarine::RotateAround(centre, origin, rot);
    DrawOrientedBox(renderer, centre, {hx, hy}, rot);
  }

 private:
  static constexpr int kBoxCorners = 4;
  // SDL_RenderLines draws an open polyline, so the outline repeats the first corner to close.
  static constexpr int kOutlinePoints = kBoxCorners + 1;

  // Trace the four corners of a centred box under `rot`, closing back on the first.
  static void DrawOrientedBox(SDL_Renderer* renderer, const glm::vec2 centre, const glm::vec2 half,
                              const octarine::Rotation2D rot) {
    const glm::vec2 offsets[kBoxCorners] = {{-half.x, -half.y}, {half.x, -half.y}, {half.x, half.y}, {-half.x, half.y}};
    SDL_FPoint corners[kOutlinePoints];
    for (int i = 0; i < kBoxCorners; ++i) {
      const glm::vec2 p = centre + octarine::Rotate(offsets[i], rot);
      corners[i] = {p.x, p.y};
    }
    corners[kBoxCorners] = corners[0];
    SDL_RenderLines(renderer, corners, kOutlinePoints);
  }
};
