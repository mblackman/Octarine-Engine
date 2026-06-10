#pragma once

#include <algorithm>
#include <array>
#include <atomic>
#include <cstdint>
#include <vector>

#include "./RenderCommands.h"
#include "./RenderKey.h"
#include "General/Constants.h"

// Lock-free, multi-producer render queue. Pre-sized to a fixed capacity; parallel
// systems atomically claim a slot via fetch_add and write directly. No per-system
// channel + playback merge — that intermediate copy was the bulk of the overhead in
// the previous CommandBuffer-driven design.
//
// Sort() uses an LSB radix sort on the precomputed RenderKey::sortKey. With ~32K+ keys
// this is dramatically faster than std::sort on the full 80-byte RenderKey because the
// radix passes only touch 16-byte (key, srcIdx) entries. The full 80-byte permutation is
// applied via a gather into a scratch buffer at the end — sequential writes are
// prefetch-friendly and the source data fits in L3 cache at typical entry counts.
//
// Capacity must comfortably exceed the max keys-per-frame for the scene; an overflow
// asserts in debug. Bump Constants::kInitialRenderQueueCapacity if you hit it.
class RenderQueue {
 public:
  using value_type = RenderKey;
  using const_iterator = std::vector<RenderKey>::const_iterator;

  explicit RenderQueue(const size_t capacity = Constants::kInitialRenderQueueCapacity) {
    render_keys_.resize(capacity);
  }

  ~RenderQueue() = default;
  RenderQueue(const RenderQueue&) = delete;
  RenderQueue& operator=(const RenderQueue&) = delete;

  // Atomic isn't move-constructible, but Registry::Set takes the singleton by value.
  // Hand-rolled move just snapshots the counter; only valid before any concurrent producer
  // attaches (i.e. registration time).
  RenderQueue(RenderQueue&& other) noexcept
      : render_keys_(std::move(other.render_keys_)), count_(other.count_.load(std::memory_order_relaxed)) {}
  RenderQueue& operator=(RenderQueue&& other) noexcept {
    render_keys_ = std::move(other.render_keys_);
    count_.store(other.count_.load(std::memory_order_relaxed), std::memory_order_relaxed);
    return *this;
  }

  SpriteCommand& EmplaceSprite(unsigned int layer, float depth, const void* batchKey = nullptr,
                               octarine::BlendMode blendMode = octarine::BlendMode::Blend) {
    auto& key = ClaimSlot();
    key.sortKey = RenderKey::ComputeSortKey(layer, depth, SPRITE, batchKey, static_cast<std::uint8_t>(blendMode));
    key.type = SPRITE;
    return key.payload.sprite;
  }

  SquareCommand& EmplaceSquare(unsigned int layer, float depth, const void* batchKey = nullptr,
                               octarine::BlendMode blendMode = octarine::BlendMode::Blend) {
    auto& key = ClaimSlot();
    key.sortKey =
        RenderKey::ComputeSortKey(layer, depth, SQUARE_PRIMITIVE, batchKey, static_cast<std::uint8_t>(blendMode));
    key.type = SQUARE_PRIMITIVE;
    return key.payload.square;
  }

  TextCommand& EmplaceText(unsigned int layer, float depth, const void* batchKey = nullptr) {
    auto& key = ClaimSlot();
    key.sortKey = RenderKey::ComputeSortKey(layer, depth, TEXT, batchKey);
    key.type = TEXT;
    return key.payload.text;
  }

  void Clear() { count_.store(0, std::memory_order_relaxed); }

  void Sort() {
    const size_t n = count_.load(std::memory_order_relaxed);
    if (n <= 1) return;

    // Build (sortKey, srcIdx) entries — 16 bytes each. Sorting these instead of the
    // 80-byte RenderKeys is the main speedup; we only gather the heavy keys once at
    // the end via a sequential-write scatter into a scratch buffer.
    sort_entries_.resize(n);
    for (size_t i = 0; i < n; ++i) {
      sort_entries_[i].key = render_keys_[i].sortKey;
      sort_entries_[i].srcIdx = static_cast<std::uint32_t>(i);
    }

    RadixSort();

    // Gather: write the sorted RenderKeys sequentially into permute_scratch_ by
    // reading from random source positions. Sequential writes are prefetch-friendly;
    // the random reads hit ~2.5 MB of source data which fits comfortably in L3 cache.
    // The scratch vector is kept alive across frames so this doesn't allocate after
    // the first frame.
    // Resize to full capacity — after the swap this becomes render_keys_, and ClaimSlot()
    // indexes into it up to the pre-sized capacity.
    permute_scratch_.resize(render_keys_.size());
    const auto* perm = sort_entries_.data();
    const auto* src = render_keys_.data();
    auto* dst = permute_scratch_.data();
    for (size_t i = 0; i < n; ++i) {
      dst[i] = src[perm[i].srcIdx];
    }
    std::swap(render_keys_, permute_scratch_);
  }

  [[nodiscard]] const_iterator begin() const noexcept { return render_keys_.cbegin(); }
  [[nodiscard]] const_iterator end() const noexcept {
    return render_keys_.cbegin() + static_cast<std::ptrdiff_t>(count_.load(std::memory_order_relaxed));
  }
  [[nodiscard]] const_iterator cbegin() const noexcept { return begin(); }
  [[nodiscard]] const_iterator cend() const noexcept { return end(); }
  [[nodiscard]] bool IsEmpty() const noexcept { return count_.load(std::memory_order_relaxed) == 0; }
  [[nodiscard]] size_t Size() const noexcept { return count_.load(std::memory_order_relaxed); }

 private:
  RenderKey& ClaimSlot() {
    const size_t i = count_.fetch_add(1, std::memory_order_relaxed);
    // Pre-sized vector — caller must size capacity to peak frame load.
    return render_keys_[i];
  }

  struct SortEntry {
    std::uint64_t key;
    std::uint32_t srcIdx;
  };

  // LSB radix sort on the 64-bit key with two optimizations over the naive 8-pass version:
  //
  // 1. Single-pass histogram: build all 8 byte-histograms in one scan of the data instead
  //    of re-reading the array for each pass. At 32k entries × 16 bytes this avoids ~3.5 MB
  //    of redundant reads and the associated cache-miss overhead.
  //
  // 2. Skip-uniform-byte: if every entry has the same value for a given byte (common — e.g.
  //    all sprites on layer 0 share the top 2 bytes), that pass cannot reorder anything.
  //    Skipping it saves both the scatter pass AND its prefix-sum. On a typical stress scene
  //    this cuts 8 passes to ~4.
  //
  // Pass-parity bookkeeping ensures the final sorted result lives in sort_entries_ regardless
  // of how many passes actually executed.
  void RadixSort() {
    const size_t n = sort_entries_.size();
    if (n <= 1) return;
    radix_scratch_.resize(n);

    // --- Phase 1: build all 8 histograms in one scan ---
    std::array<std::array<size_t, 256>, 8> histograms{};
    for (size_t i = 0; i < n; ++i) {
      const std::uint64_t k = sort_entries_[i].key;
      histograms[0][k & 0xFFu]++;
      histograms[1][(k >> 8) & 0xFFu]++;
      histograms[2][(k >> 16) & 0xFFu]++;
      histograms[3][(k >> 24) & 0xFFu]++;
      histograms[4][(k >> 32) & 0xFFu]++;
      histograms[5][(k >> 40) & 0xFFu]++;
      histograms[6][(k >> 48) & 0xFFu]++;
      histograms[7][(k >> 56) & 0xFFu]++;
    }

    // --- Phase 2: determine which passes are needed ---
    // A pass is skippable when exactly one bucket holds all n entries (all bytes identical).
    std::array<bool, 8> skip{};
    for (size_t pass = 0; pass < 8; ++pass) {
      for (size_t bucket = 0; bucket < 256; ++bucket) {
        if (histograms[pass][bucket] == n) {
          skip[pass] = true;
          break;
        }
      }
    }

    // --- Phase 3: scatter passes (LSB → MSB), skipping uniform bytes ---
    std::vector<SortEntry>* in = &sort_entries_;
    std::vector<SortEntry>* out = &radix_scratch_;

    for (size_t pass = 0; pass < 8; ++pass) {
      if (skip[pass]) continue;

      const size_t shift = pass * 8;

      // Convert the histogram to exclusive prefix sums (offsets) for scatter.
      auto& counts = histograms[pass];
      size_t sum = 0;
      for (auto& c : counts) {
        const size_t saved = c;
        c = sum;
        sum += saved;
      }

      // Scatter
      for (size_t i = 0; i < n; ++i) {
        const auto bucket = static_cast<std::uint8_t>(((*in)[i].key >> shift) & 0xFFu);
        (*out)[counts[bucket]++] = (*in)[i];
      }
      std::swap(in, out);
    }

    // Ensure the sorted result lives in sort_entries_.
    if (in != &sort_entries_) {
      std::swap(sort_entries_, radix_scratch_);
    }
  }

  std::vector<RenderKey> render_keys_;
  std::atomic<size_t> count_{0};

  std::vector<SortEntry> sort_entries_;
  std::vector<SortEntry> radix_scratch_;
  std::vector<RenderKey> permute_scratch_;  // Gather output buffer, kept alive to avoid re-alloc.
};
