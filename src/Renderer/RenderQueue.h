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
// Sort() uses an LSB radix sort on the precomputed RenderKey::sortKey. With ~120K keys
// this is dramatically faster than std::sort on the full 80-byte RenderKey because the
// radix passes only touch 16-byte (key, srcIdx) entries. The full 80-byte permutation is
// applied in place via cycle-walking at the end, keeping render_keys_ memory warm for
// the subsequent Renderer::Render iteration.
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

  SpriteCommand& EmplaceSprite(unsigned int layer, float depth, const void* batchKey = nullptr) {
    auto& key = ClaimSlot();
    key.sortKey = RenderKey::ComputeSortKey(layer, depth, SPRITE, batchKey);
    key.type = SPRITE;
    return key.payload.sprite;
  }

  SquareCommand& EmplaceSquare(unsigned int layer, float depth, const void* batchKey = nullptr) {
    auto& key = ClaimSlot();
    key.sortKey = RenderKey::ComputeSortKey(layer, depth, SQUARE_PRIMITIVE, batchKey);
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
    // 80-byte RenderKeys is the main speedup; we only permute the heavy keys once at
    // the end via an in-place cycle walk.
    sort_entries_.resize(n);
    for (size_t i = 0; i < n; ++i) {
      sort_entries_[i].key = render_keys_[i].sortKey;
      sort_entries_[i].srcIdx = static_cast<std::uint32_t>(i);
    }

    RadixSort();

    // Apply the permutation in place by walking each cycle. We reuse sort_entries_[].srcIdx
    // as both the input permutation and a "done" marker (setting srcIdx = j after placing
    // marks j as finished, since cycles in a permutation are disjoint). Performance ties
    // the gather-then-swap variant in benchmarking; we keep this version because it avoids
    // the ~22MB out_buf_ scratch allocation.
    auto* perm = sort_entries_.data();
    auto* keys = render_keys_.data();
    for (std::uint32_t i = 0; i < n; ++i) {
      if (perm[i].srcIdx == i) continue;

      RenderKey saved = std::move(keys[i]);
      std::uint32_t j = i;
      std::uint32_t next = perm[j].srcIdx;
      while (next != i) {
        keys[j] = std::move(keys[next]);
        perm[j].srcIdx = j;
        j = next;
        next = perm[j].srcIdx;
      }
      keys[j] = std::move(saved);
      perm[j].srcIdx = j;
    }
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

  // 8-pass LSB radix sort on the 64-bit key. Each pass distributes entries across 256
  // buckets by one byte of the key. After 8 passes (one per byte), entries are in
  // ascending key order. We always do all 8 passes — detecting and skipping zero-bit
  // ranges saves a couple of ms in some scenes but adds branch complexity; not worth it
  // yet.
  void RadixSort() {
    const size_t n = sort_entries_.size();
    if (n <= 1) return;
    radix_scratch_.resize(n);

    std::vector<SortEntry>* in = &sort_entries_;
    std::vector<SortEntry>* out = &radix_scratch_;

    for (int pass = 0; pass < 8; ++pass) {
      const int shift = pass * 8;
      std::array<size_t, 256> counts{};

      for (size_t i = 0; i < n; ++i) {
        ++counts[((*in)[i].key >> shift) & 0xFFu];
      }
      size_t sum = 0;
      for (auto& c : counts) {
        const size_t saved = c;
        c = sum;
        sum += saved;
      }
      for (size_t i = 0; i < n; ++i) {
        const std::uint8_t bucket = static_cast<std::uint8_t>(((*in)[i].key >> shift) & 0xFFu);
        (*out)[counts[bucket]++] = (*in)[i];
      }
      std::swap(in, out);
    }

    // Even pass count → sorted data ends in sort_entries_. Keep a safety check in case
    // the loop bound changes later.
    if (in != &sort_entries_) {
      std::swap(sort_entries_, radix_scratch_);
    }
  }

  std::vector<RenderKey> render_keys_;
  std::atomic<size_t> count_{0};

  std::vector<SortEntry> sort_entries_;
  std::vector<SortEntry> radix_scratch_;
};
