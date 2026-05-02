#pragma once

#include "../ECS/ECS.h"
#include "./RenderableType.h"

struct RenderKey {
   public:
    unsigned int layer;
    float depth;
    RenderableType type;
    Entity entity;

    RenderKey(unsigned int layer, float depth, RenderableType type, Entity entity)
        : layer(layer), depth(depth), type(type), entity(entity) {
    }

    bool operator==(const RenderKey& other) const {
        return layer == other.layer && depth == other.depth && type == other.type && entity.GetId() == other.entity.GetId();
    }

    bool operator!=(const RenderKey& other) const {
        return !(*this == other);
    }

    bool operator<(const RenderKey& other) const {
        if (layer != other.layer) {
            return layer < other.layer;
        }
        if (depth != other.depth) {
            return depth < other.depth;
        }
        if (type != other.type) {
            return type < other.type;
        }
        return entity.GetId() < other.entity.GetId();
    }

    bool operator<=(const RenderKey& other) const {
        if (layer != other.layer) {
            return layer < other.layer;
        }
        if (depth != other.depth) {
            return depth < other.depth;
        }
        if (type != other.type) {
            return type < other.type;
        }
        return entity.GetId() <= other.entity.GetId();
    }

    bool operator>(const RenderKey& other) const {
        if (layer != other.layer) {
            return layer > other.layer;
        }
        if (depth != other.depth) {
            return depth > other.depth;
        }
        if (type != other.type) {
            return type > other.type;
        }
        return entity.GetId() > other.entity.GetId();
    }

    bool operator>=(const RenderKey& other) const {
        if (layer != other.layer) {
            return layer > other.layer;
        }
        if (depth != other.depth) {
            return depth > other.depth;
        }
        if (type != other.type) {
            return type > other.type;
        }
        return entity.GetId() >= other.entity.GetId();
    }
};