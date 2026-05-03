#pragma once
#include <algorithm>
#include <atomic>
#include <memory>
#include <mutex>
#include <vector>

#include "Entity.h"
#include "General/Constants.h"
#include "Registry.h"

class CommandBuffer {
 public:
  explicit CommandBuffer(size_t capacity = Constants::kSystemCommandBufferSize)
      : state_(std::make_unique<State>(capacity)) {}

  // Explicitly enable move semantics, explicitly delete copy semantics
  CommandBuffer(CommandBuffer&&) = default;
  CommandBuffer& operator=(CommandBuffer&&) = default;
  CommandBuffer(const CommandBuffer&) = delete;
  CommandBuffer& operator=(const CommandBuffer&) = delete;

  void QueueBlam(const Entity entity) const {
    // Uses isolated blam counter
    const size_t index = state_->blam_count.fetch_add(1, std::memory_order_relaxed);

    if (index < state_->blam_buffer.size()) {
      state_->blam_buffer[index] = entity;
    } else {
      std::lock_guard<std::mutex> lock(state_->overflow_blam_mutex);
      state_->overflow_blam_buffer.push_back(entity);
    }
  }

  void QueueAddRenderKey(const RenderKey& renderKey) const {
    // Uses isolated render key counter
    const size_t index = state_->render_key_count.fetch_add(1, std::memory_order_relaxed);

    // Fixed size check to check renderKey_buffer instead of blam_buffer
    if (index < state_->renderKey_buffer.size()) {
      state_->renderKey_buffer[index] = renderKey;
    } else {
      std::lock_guard<std::mutex> lock(state_->overflow_render_key_mutex);
      state_->overflow_renderKey_buffer.push_back(renderKey);
    }
  }

  void Playback(Registry* registry) const {
    auto& renderQueue = registry->Get<RenderQueue>();

    // 1. Process Blams
    const size_t total_blams = std::min(state_->blam_count.load(std::memory_order_relaxed), state_->blam_buffer.size());
    for (size_t i = 0; i < total_blams; ++i) {
      registry->QueueBlamEntity(state_->blam_buffer[i]);
    }
    for (const Entity e : state_->overflow_blam_buffer) {
      registry->QueueBlamEntity(e);
    }

    // 2. Process RenderKeys
    const size_t total_keys =
        std::min(state_->render_key_count.load(std::memory_order_relaxed), state_->renderKey_buffer.size());
    for (size_t i = 0; i < total_keys; ++i) {
      renderQueue.AddRenderKey(state_->renderKey_buffer[i]);
    }
    // Passed by const reference to avoid unnecessary struct copies
    for (const RenderKey& k : state_->overflow_renderKey_buffer) {
      renderQueue.AddRenderKey(k);
    }

    // 3. Reset Counters and Clear Overflow
    state_->blam_count.store(0, std::memory_order_relaxed);
    state_->render_key_count.store(0, std::memory_order_relaxed);
    state_->overflow_blam_buffer.clear();
    state_->overflow_renderKey_buffer.clear();  // Fixed memory leak
  }

 private:
  struct State {
    std::vector<Entity> blam_buffer;
    std::vector<RenderKey> renderKey_buffer;

    // Split counters to prevent interwoven garbage data
    std::atomic<size_t> blam_count{0};
    std::atomic<size_t> render_key_count{0};

    std::mutex overflow_blam_mutex;
    std::mutex overflow_render_key_mutex;
    std::vector<Entity> overflow_blam_buffer;
    std::vector<RenderKey> overflow_renderKey_buffer;

    explicit State(const size_t capacity) {
      blam_buffer.resize(capacity);
      renderKey_buffer.resize(capacity);
    }
  };

  std::unique_ptr<State> state_;
};