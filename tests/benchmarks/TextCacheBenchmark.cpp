#include <benchmark/benchmark.h>

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

// Head-to-head micro-bench for the RenderTextSystem cache-key change. The old path rebuilt a
// "fontId|text|color" std::string every frame for every visible label (even on a cache hit) and
// looked it up in an unordered_map<string>. The new path keys the cache by entity id and only
// touches the (fontId, text, color) fields to decide whether to re-rasterize. This measures the
// steady-state (cache-hit) per-label lookup cost of each strategy for N on-screen labels.
//
// This is an in-build A/B of the two lookup strategies — it does not rely on the with/without
// source toggle, since both strategies live side by side here. It isolates the CPU the change
// removes (the per-frame string build + hash). The change's other benefit — bounding GPU texture
// memory for text that changes every frame — is a memory property, not a CPU metric, so it is out
// of scope for this bench.

namespace {
struct Label {
  std::uint32_t entity = 0;
  std::string fontId;
  std::string text;
  std::uint32_t color = 0;
};

struct ContentEntry {
  float width = 0.0F;
  float height = 0.0F;
};

struct EntityEntry {
  std::string fontId;
  std::string text;
  std::uint32_t color = 0;
  float width = 0.0F;
  float height = 0.0F;
};

// Representative UI labels: a shared font id and short, distinct strings (the realistic "Score:
// 1234" / "HP: 87" shape), each owned by a distinct entity.
std::vector<Label> MakeLabels(int n) {
  std::vector<Label> labels;
  labels.reserve(static_cast<std::size_t>(n));
  for (int i = 0; i < n; ++i) {
    labels.push_back(
        Label{static_cast<std::uint32_t>(i), "main-font", "Score: " + std::to_string(1000 + i), 0xFFFFFFFFu});
  }
  return labels;
}
}  // namespace

// Old strategy: concatenate the content key every frame, then look it up in a string-keyed map.
static void BM_TextCache_ContentKeyed(benchmark::State& state) {
  const int n = static_cast<int>(state.range(0));
  const std::vector<Label> labels = MakeLabels(n);

  std::unordered_map<std::string, ContentEntry> cache;
  for (const auto& l : labels) {
    cache[l.fontId + "|" + l.text + "|" + std::to_string(l.color)] = ContentEntry{16.0f, 16.0f};
  }

  for (auto _ : state) {
    for (const auto& l : labels) {
      const std::string key = l.fontId + "|" + l.text + "|" + std::to_string(l.color);
      auto it = cache.find(key);
      benchmark::DoNotOptimize(it->second);
    }
  }
  state.SetItemsProcessed(static_cast<int64_t>(state.iterations()) * n);
}
BENCHMARK(BM_TextCache_ContentKeyed)->Range(8, 1024);

// New strategy: look up by entity id, then confirm the cached (fontId, text, color) still match.
static void BM_TextCache_EntityKeyed(benchmark::State& state) {
  const int n = static_cast<int>(state.range(0));
  const std::vector<Label> labels = MakeLabels(n);

  std::unordered_map<std::uint32_t, EntityEntry> cache;
  for (const auto& l : labels) {
    cache[l.entity] = EntityEntry{l.fontId, l.text, l.color, 16.0f, 16.0f};
  }

  for (auto _ : state) {
    for (const auto& l : labels) {
      auto it = cache.find(l.entity);
      bool fresh = it != cache.end() && it->second.color == l.color && it->second.fontId == l.fontId &&
                   it->second.text == l.text;
      benchmark::DoNotOptimize(fresh);
      benchmark::DoNotOptimize(it->second);
    }
  }
  state.SetItemsProcessed(static_cast<int64_t>(state.iterations()) * n);
}
BENCHMARK(BM_TextCache_EntityKeyed)->Range(8, 1024);
