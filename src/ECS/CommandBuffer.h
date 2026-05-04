#pragma once

#include <algorithm>
#include <atomic>
#include <memory>
#include <mutex>
#include <vector>

#include "ECS/Entity.h"
#include "ECS/Registry.h"
#include "General/Constants.h"

template <typename T>
struct Channel {
  std::vector<T> buffer;
  std::atomic<size_t> count{0};

  std::mutex overflow_mutex;
  std::vector<T> overflow_buffer;

  explicit Channel(const size_t capacity) { buffer.resize(capacity); }

  template <typename... Args>
  T& Emplace(Args&&... args) {
    const size_t index = count.fetch_add(1, std::memory_order_relaxed);
    if (index < buffer.size()) {
      buffer[index] = T{std::forward<Args>(args)...};
      return buffer[index];
    }
    std::lock_guard<std::mutex> lock(overflow_mutex);
    return overflow_buffer.emplace_back(std::forward<Args>(args)...);
  }

  void Clear() {
    count.store(0, std::memory_order_relaxed);
    overflow_buffer.clear();
  }
};

// Per-system, thread-safe deferred command buffer for state changes that must resolve
// on the main thread (e.g. entity destruction). Render keys do NOT go through here —
// parallel render systems write straight into the lock-free RenderQueue. Keeping this
// buffer narrow avoids the channel + playback copy that dominated render perf.
class CommandBuffer {
 public:
  explicit CommandBuffer(size_t capacity = Constants::kSystemCommandBufferSize)
      : state_(std::make_unique<State>(capacity)) {}

  CommandBuffer(CommandBuffer&&) = default;
  CommandBuffer& operator=(CommandBuffer&&) = default;
  CommandBuffer(const CommandBuffer&) = delete;
  CommandBuffer& operator=(const CommandBuffer&) = delete;

  template <typename T, typename... Args>
  T& Emplace(Args&&... args) const {
    static_assert(std::is_same_v<T, Entity>, "CommandBuffer currently only buffers Entity blam requests");
    return state_->blams.Emplace(std::forward<Args>(args)...);
  }

  void Playback(Registry* registry) const {
    auto& chan = state_->blams;
    const size_t total = std::min(chan.count.load(std::memory_order_relaxed), chan.buffer.size());
    for (size_t i = 0; i < total; ++i) registry->QueueBlamEntity(chan.buffer[i]);
    for (const Entity& e : chan.overflow_buffer) registry->QueueBlamEntity(e);
    chan.Clear();
  }

 private:
  struct State {
    Channel<Entity> blams;
    explicit State(const size_t capacity) : blams(capacity) {}
  };

  std::unique_ptr<State> state_;
};
