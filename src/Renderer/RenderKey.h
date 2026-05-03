#pragma once

#include <SDL3/SDL.h>

#include <compare>
#include <tuple>

#include "./RenderableType.h"
#include "ECS/Entity.h"

struct RenderKey {
  unsigned int layer;
  float depth;
  RenderableType type;
  Entity entity;

  // Cached sprite render data — populated during RenderSpriteSystem to avoid
  // per-entity GetComponent lookups in the Renderer.
  float destX{}, destY{}, destW{}, destH{};
  SDL_FRect srcRect{};
  double rotation{};
  SDL_FlipMode flip{};
  SDL_Texture* texture{};
  bool isFixed{};

  RenderKey(unsigned int layer, float depth, RenderableType type, Entity entity)
      : layer(layer), depth(depth), type(type), entity(entity) {}

  // Ordering uses (layer, depth, type, entityId) — the cached sprite fields are not
  // identity-bearing, so we compare a tuple of the keying fields explicitly instead of
  // defaulting <=> across all members (which would also drag in NaN-prone floats etc.).
  [[nodiscard]] auto OrderingTuple() const { return std::tuple(layer, depth, static_cast<int>(type), entity.GetId()); }

  bool operator==(const RenderKey& other) const { return OrderingTuple() == other.OrderingTuple(); }

  std::strong_ordering operator<=>(const RenderKey& other) const {
    if (auto c = layer <=> other.layer; c != 0) return c;
    if (depth < other.depth) return std::strong_ordering::less;
    if (depth > other.depth) return std::strong_ordering::greater;
    if (auto c = static_cast<int>(type) <=> static_cast<int>(other.type); c != 0) return c;
    return entity.GetId() <=> other.entity.GetId();
  }
};