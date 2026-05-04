#pragma once

#include <algorithm>
#include <atomic>
#include <vector>

#include "./RenderCommands.h"
#include "./RenderKey.h"
#include "General/Constants.h"

// Lock-free, multi-producer render queue. Pre-sized to a fixed capacity; parallel
// systems atomically claim a slot via fetch_add and write directly. No per-system
// channel + playback merge — that intermediate copy was the bulk of the overhead in
// the previous CommandBuffer-driven design.
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

  SpriteCommand& EmplaceSprite(unsigned int layer, float depth) {
    auto& key = ClaimSlot();
    key.layer = layer;
    key.depth = depth;
    key.type = SPRITE;
    return key.payload.sprite;
  }

  SquareCommand& EmplaceSquare(unsigned int layer, float depth) {
    auto& key = ClaimSlot();
    key.layer = layer;
    key.depth = depth;
    key.type = SQUARE_PRIMITIVE;
    return key.payload.square;
  }

  TextCommand& EmplaceText(unsigned int layer, float depth) {
    auto& key = ClaimSlot();
    key.layer = layer;
    key.depth = depth;
    key.type = TEXT;
    return key.payload.text;
  }

  void Clear() { count_.store(0, std::memory_order_relaxed); }

  void Sort() {
    const size_t n = count_.load(std::memory_order_relaxed);
    std::sort(render_keys_.begin(), render_keys_.begin() + static_cast<std::ptrdiff_t>(n));
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

  std::vector<RenderKey> render_keys_;
  std::atomic<size_t> count_{0};
};
