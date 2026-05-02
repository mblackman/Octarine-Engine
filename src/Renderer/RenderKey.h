#pragma once

#include <SDL3/SDL.h>

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

  bool operator==(const RenderKey& other) const {
    return layer == other.layer && depth == other.depth && type == other.type && entity.GetId() == other.entity.GetId();
  }

  bool operator!=(const RenderKey& other) const { return !(*this == other); }

  bool operator<(const RenderKey& other) const {
    if (layer != other.layer) return layer < other.layer;
    if (depth != other.depth) return depth < other.depth;
    if (type != other.type) return type < other.type;
    return entity.GetId() < other.entity.GetId();
  }

  bool operator<=(const RenderKey& other) const {
    if (layer != other.layer) return layer < other.layer;
    if (depth != other.depth) return depth < other.depth;
    if (type != other.type) return type < other.type;
    return entity.GetId() <= other.entity.GetId();
  }

  bool operator>(const RenderKey& other) const {
    if (layer != other.layer) return layer > other.layer;
    if (depth != other.depth) return depth > other.depth;
    if (type != other.type) return type > other.type;
    return entity.GetId() > other.entity.GetId();
  }

  bool operator>=(const RenderKey& other) const {
    if (layer != other.layer) return layer > other.layer;
    if (depth != other.depth) return depth > other.depth;
    if (type != other.type) return type > other.type;
    return entity.GetId() >= other.entity.GetId();
  }
};