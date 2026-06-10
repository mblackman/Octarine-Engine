// Unit checks for RenderKey::ComputeSortKey packing and RenderQueue's radix sort: field
// precedence (layer > depth band > type > blend > batch hash), tie stability, and the
// clustering invariants the renderer's draw batching relies on. gtest-free; exit code is
// the number of failed checks. Registered with ctest as RenderQueueTest.

#include <SDL3/SDL.h>

#include <cstdint>
#include <string>
#include <vector>

#include "General/BlendMode.h"
#include "General/Constants.h"
#include "Renderer/RenderKey.h"
#include "Renderer/RenderQueue.h"
#include "TestHarness.h"

using octarine::BlendMode;
using octarine::test::Check;
using octarine::test::CheckEq;

namespace {

std::uint64_t Key(const unsigned int layer, const float depth, const RenderableType type, const void* batchKey,
                  const BlendMode blend = BlendMode::Blend) {
  return RenderKey::ComputeSortKey(layer, depth, type, batchKey, static_cast<std::uint8_t>(blend));
}

// Fake texture handles — ComputeSortKey only hashes the pointer value. 64-byte alignment keeps
// the two values apart in the bits the hash actually reads (it shifts off the low 4).
alignas(64) char g_texMemA[64];
alignas(64) char g_texMemB[64];

}  // namespace

int main() {
  const void* texA = g_texMemA;
  const void* texB = g_texMemB;

  std::cout << "[sortkey] field precedence\n";
  {
    Check(Key(1, 0.0f, SPRITE, nullptr) > Key(0, 1000000.0f, SPRITE, nullptr), "layer outranks any depth difference");
    Check(Key(0, 2.0f * Constants::kRenderBatchYBandPx, SPRITE, nullptr) > Key(0, 0.0f, SPRITE, nullptr),
          "deeper band sorts later within a layer");
    CheckEq(Key(0, 0.0f, SPRITE, nullptr), Key(0, Constants::kRenderBatchYBandPx - 1.0f, SPRITE, nullptr),
            "depths inside one band share a key");
    Check(Key(0, -2.0f * Constants::kRenderBatchYBandPx, SPRITE, nullptr) < Key(0, 0.0f, SPRITE, nullptr),
          "negative depth sorts before zero (band bias)");
    Check(Key(0, 0.0f, SPRITE, nullptr) != Key(0, 0.0f, SQUARE_PRIMITIVE, nullptr), "type participates in the key");
    Check(Key(0, 0.0f, SPRITE, texA) != Key(0, 0.0f, SPRITE, texB), "distinct textures get distinct batch hashes");
    CheckEq(Key(0, 0.0f, SPRITE, texA), Key(0, 0.0f, SPRITE, texA), "key is deterministic");
    // Blend sits above the texture hash inside the batch bits, so every same-blend run clusters
    // before texture clustering — regardless of which texture pointers are involved.
    Check(Key(0, 0.0f, SPRITE, texA, BlendMode::Blend) < Key(0, 0.0f, SPRITE, texB, BlendMode::Add) &&
              Key(0, 0.0f, SPRITE, texB, BlendMode::Blend) < Key(0, 0.0f, SPRITE, texA, BlendMode::Add),
          "blend mode outranks texture hash");
  }

  std::cout << "[sort] orders by key, ties keep insertion order\n";
  {
    RenderQueue queue(1024);
    // Three layers emplaced in reverse; four entries per layer marked with their insertion index.
    int idx = 0;
    for (const unsigned int layer : {2u, 1u, 0u}) {
      for (int i = 0; i < 4; ++i) {
        auto& cmd = queue.EmplaceSprite(layer, 0.0f, texA);
        cmd.destX = static_cast<float>(idx++);
      }
    }
    queue.Sort();
    CheckEq(queue.Size(), static_cast<size_t>(12), "Size unchanged by Sort");

    bool ascending = true;
    std::vector<float> markers;
    std::uint64_t prev = 0;
    for (const RenderKey& key : queue) {
      if (!markers.empty() && key.sortKey < prev) ascending = false;
      prev = key.sortKey;
      markers.push_back(key.payload.sprite.destX);
    }
    Check(ascending, "sortKey is non-decreasing after Sort");
    Check(markers[0] == 8 && markers[4] == 4 && markers[8] == 0, "layer groups come out 0, 1, 2");
    bool stable = true;
    for (int group = 0; group < 3; ++group) {
      for (int i = 1; i < 4; ++i) {
        if (markers[group * 4 + i] != markers[group * 4 + i - 1] + 1.0f) stable = false;
      }
    }
    Check(stable, "tied keys keep insertion order (stable radix passes)");
  }

  std::cout << "[sort] blend + texture clustering within one layer/band\n";
  {
    RenderQueue queue(1024);
    // Worst-case interleave of (blend, texture) combos: 16 sprites alternating blend every entry
    // and texture every other entry. After Sort the queue must contain exactly one blend boundary
    // and one texture boundary inside each blend run.
    const BlendMode blends[2] = {BlendMode::Blend, BlendMode::Add};
    const void* textures[2] = {texA, texB};
    for (int i = 0; i < 16; ++i) {
      const BlendMode blend = blends[i % 2];
      const void* texture = textures[(i / 2) % 2];
      auto& cmd = queue.EmplaceSprite(0, 0.0f, texture, blend);
      cmd.texture = static_cast<SDL_Texture*>(const_cast<void*>(texture));
      cmd.blendMode = octarine::ToSdlBlendMode(blend);
    }
    queue.Sort();

    int blendTransitions = 0;
    int textureTransitions = 0;
    SDL_BlendMode prevBlend{};
    SDL_Texture* prevTexture{};
    bool first = true;
    for (const RenderKey& key : queue) {
      const auto& cmd = key.payload.sprite;
      if (!first) {
        if (cmd.blendMode != prevBlend) {
          ++blendTransitions;
        } else if (cmd.texture != prevTexture) {
          ++textureTransitions;
        }
      }
      prevBlend = cmd.blendMode;
      prevTexture = cmd.texture;
      first = false;
    }
    CheckEq(blendTransitions, 1, "same-blend runs cluster (one blend boundary)");
    CheckEq(textureTransitions, 2, "textures cluster within each blend run");
  }

  std::cout << "[sort] type clusters within one layer/band\n";
  {
    RenderQueue queue(256);
    queue.EmplaceSquare(0, 0.0f);
    queue.EmplaceSprite(0, 0.0f, texA);
    queue.EmplaceText(0, 0.0f);
    queue.EmplaceSquare(0, 0.0f);
    queue.EmplaceSprite(0, 0.0f, texA);
    queue.Sort();

    int typeTransitions = 0;
    RenderableType prevType{};
    bool first = true;
    for (const RenderKey& key : queue) {
      if (!first && key.type != prevType) ++typeTransitions;
      prevType = key.type;
      first = false;
    }
    CheckEq(typeTransitions, 2, "three types -> two type boundaries");
  }

  std::cout << "[lifecycle] Clear resets, queue is reusable\n";
  {
    RenderQueue queue(64);
    queue.EmplaceSprite(0, 0.0f, nullptr);
    Check(!queue.IsEmpty(), "queue non-empty after emplace");
    queue.Clear();
    Check(queue.IsEmpty() && queue.Size() == 0 && queue.begin() == queue.end(), "Clear resets the queue");
    auto& cmd = queue.EmplaceSprite(0, 0.0f, nullptr);
    cmd.destX = 42.0f;
    queue.Sort();
    CheckEq(queue.Size(), static_cast<size_t>(1), "queue reusable after Clear");
    CheckEq(queue.begin()->payload.sprite.destX, 42.0f, "payload survives the post-Clear sort");
  }

  return octarine::test::ReportSummary("RenderQueueTest");
}
