#pragma once

#include <algorithm>
#include <vector>

#include "./RenderKey.h"

class RenderQueue {
  constexpr static size_t kInitialCapacity = 1024;

 public:
  using value_type = RenderKey;
  using const_iterator = std::vector<RenderKey>::const_iterator;

  explicit RenderQueue(const size_t initial_capacity = kInitialCapacity) { render_keys_.reserve(initial_capacity); }

  ~RenderQueue() = default;

  RenderQueue(const RenderQueue&) = delete;
  RenderQueue& operator=(const RenderQueue&) = delete;

  RenderQueue(RenderQueue&&) = default;
  RenderQueue& operator=(RenderQueue&&) = default;

  void AddRenderKey(const RenderKey& key) { render_keys_.push_back(key); }

  void AddRenderKey(RenderKey&& key) { render_keys_.push_back(key); }

  void Clear() { render_keys_.clear(); }

  void Sort() { std::sort(render_keys_.begin(), render_keys_.end()); }

  [[nodiscard]] const_iterator begin() const noexcept { return render_keys_.cbegin(); }

  [[nodiscard]] const_iterator end() const noexcept { return render_keys_.cend(); }

  [[nodiscard]] const_iterator cbegin() const noexcept { return render_keys_.cbegin(); }

  [[nodiscard]] const_iterator cend() const noexcept { return render_keys_.cend(); }

  [[nodiscard]] bool IsEmpty() const noexcept { return render_keys_.empty(); }

  [[nodiscard]] size_t Size() const noexcept { return render_keys_.size(); }

 private:
  std::vector<RenderKey> render_keys_;
};