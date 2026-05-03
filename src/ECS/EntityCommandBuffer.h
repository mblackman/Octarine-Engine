#pragma once
#include <algorithm>
#include <atomic>
#include <memory>
#include <mutex>
#include <vector>

#include "Entity.h"
#include "Registry.h"

class EntityCommandBuffer {
 public:
  explicit EntityCommandBuffer(size_t capacity = 1024) : state_(std::make_unique<State>(capacity)) {}

  // Explicitly enable move semantics, explicitly delete copy semantics
  EntityCommandBuffer(EntityCommandBuffer&&) = default;
  EntityCommandBuffer& operator=(EntityCommandBuffer&&) = default;
  EntityCommandBuffer(const EntityCommandBuffer&) = delete;
  EntityCommandBuffer& operator=(const EntityCommandBuffer&) = delete;

  void QueueBlam(const Entity entity) {
    const size_t index = state_->count.fetch_add(1, std::memory_order_relaxed);

    if (index < state_->buffer.size()) {
      state_->buffer[index] = entity;
    } else {
      std::lock_guard<std::mutex> lock(state_->overflow_mutex);
      state_->overflow_buffer.push_back(entity);
    }
  }

  void Playback(Registry* registry) {
    const size_t total = std::min(state_->count.load(std::memory_order_relaxed), state_->buffer.size());

    for (size_t i = 0; i < total; ++i) {
      registry->QueueBlamEntity(state_->buffer[i]);
    }

    for (const Entity e : state_->overflow_buffer) {
      registry->QueueBlamEntity(e);
    }

    state_->count.store(0, std::memory_order_relaxed);
    state_->overflow_buffer.clear();
  }

 private:
  struct State {
    std::vector<Entity> buffer;
    std::atomic<size_t> count{0};

    std::mutex overflow_mutex;
    std::vector<Entity> overflow_buffer;

    explicit State(size_t capacity) { buffer.resize(capacity); }
  };

  std::unique_ptr<State> state_;
};