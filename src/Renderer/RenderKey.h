#pragma once

#include <cmath>
#include <compare>
#include <cstdint>

#include "./RenderCommands.h"
#include "./RenderableType.h"
#include "General/Constants.h"

// Render entry — slim header (precomputed sortKey + type) plus one of three POD payloads
// in a union, selected by `type`. Inlining the payload here means a sorted iteration is
// fully sequential reads through one vector instead of a random gather into per-type
// command arrays.
//
// `sortKey` packs all of (layer, depthBand, type, batchKey-hash) into a 64-bit integer
// so RenderQueue::Sort can use a comparison-free radix sort. Layout, MSB → LSB:
//   bits 63-48 : layer (16 bits, unsigned — 64K layers covers kDebugUIBaseLayer headroom)
//   bits 47-28 : depthBand + bias (20 bits — ±16M world pixels at 32px bands)
//   bits 27-24 : type (4 bits)
//   bits 23- 0 : batchKey hash (24 bits) — clusters same-texture runs within a band so
//                SDL3's internal renderer can coalesce them. 16M slots makes collisions
//                vanishingly rare for any realistic texture count; collisions only
//                degrade batching, never produce wrong pixels.
//
// type is also stored separately because Renderer::Render dispatches on it to choose the
// payload union member.
struct RenderKey {
  std::uint64_t sortKey{};
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

  static std::uint64_t ComputeSortKey(unsigned int layer, float depth, RenderableType type, const void* batchKey) {
    std::int32_t band = 0;
    if constexpr (Constants::kRenderBatchYBandPx > 0.0f) {
      band = static_cast<std::int32_t>(std::floor(depth / Constants::kRenderBatchYBandPx));
    }

    // Bias depthBand to non-negative 20-bit range. Layer is unsigned, masked to 16 bits
    // (kDebugUIBaseLayer is ~1000, so 64K headroom is plenty).
    constexpr std::int32_t kBandBias = 1 << 19;
    const std::uint64_t layer16 = static_cast<std::uint64_t>(layer & 0xFFFFu);
    const std::uint64_t band20 = static_cast<std::uint64_t>(static_cast<std::uint32_t>(band + kBandBias) & 0xFFFFFu);
    const std::uint64_t type4 = static_cast<std::uint64_t>(static_cast<std::uint8_t>(type)) & 0xFu;

    // 24-bit hash. Shift off allocator alignment noise then XOR-fold; for ~20 textures
    // mapped into 16M slots the collision probability is effectively zero. nullptr (used
    // by primitives) hashes to 0, which keeps all primitives clustered.
    const auto ptr = reinterpret_cast<std::uintptr_t>(batchKey);
    const std::uint64_t hash24 = static_cast<std::uint64_t>((ptr >> 4) ^ (ptr >> 28)) & 0xFFFFFFu;

    return (layer16 << 48) | (band20 << 28) | (type4 << 24) | hash24;
  }

  // Kept for completeness; not used by Sort (radix sort uses sortKey directly).
  auto operator<=>(const RenderKey& other) const { return sortKey <=> other.sortKey; }
  bool operator==(const RenderKey& other) const { return sortKey == other.sortKey && type == other.type; }
};
