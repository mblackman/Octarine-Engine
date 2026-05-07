#pragma once

#include <compare>
#include <tuple>

#include "./RenderCommands.h"
#include "./RenderableType.h"

// Render entry — slim header (layer/depth/type) plus one of three POD payloads in
// a union, selected by `type`. Inlining the payload here means a sorted iteration is
// fully sequential reads through one vector instead of a random gather into per-type
// command arrays. With ~100K entries per frame the random-access cost is the
// bottleneck of Renderer::Render.
struct RenderKey {
  unsigned int layer{};
  float depth{};
  RenderableType type{};

  union Payload {
    SpriteCommand sprite;
    SquareCommand square;
    TextCommand text;

    // Leaves union storage uninitialized — the producing system always overwrites the
    // active member chosen by `type`. Skipping zero-init saves ~56 bytes of writes per
    // emplace, which is meaningful at 100K+ keys per frame.
    Payload() {}
  } payload;

  RenderKey() = default;

  RenderKey(unsigned int t_layer, float t_depth, RenderableType t_type)
      : layer(t_layer), depth(t_depth), type(t_type) {}

  [[nodiscard]] auto OrderingTuple() const { return std::tuple(layer, depth, static_cast<int>(type)); }

  bool operator==(const RenderKey& other) const { return OrderingTuple() == other.OrderingTuple(); }

  std::strong_ordering operator<=>(const RenderKey& other) const {
    if (auto c = layer <=> other.layer; c != 0) return c;
    if (depth < other.depth) return std::strong_ordering::less;
    if (depth > other.depth) return std::strong_ordering::greater;
    return static_cast<int>(type) <=> static_cast<int>(other.type);
  }
};
