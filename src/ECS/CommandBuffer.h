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
  enum class CommandType { Blam, Despawn };
  struct Command {
    CommandType type;
    Entity entity;
  };

  explicit CommandBuffer(size_t capacity = Constants::kSystemCommandBufferSize)
      : state_(std::make_unique<State>(capacity)) {}

  CommandBuffer(CommandBuffer&&) = default;
  CommandBuffer& operator=(CommandBuffer&&) = default;
  CommandBuffer(const CommandBuffer&) = delete;
  CommandBuffer& operator=(const CommandBuffer&) = delete;

  void EmplaceBlam(Entity entity) const { state_->commands.Emplace(Command{CommandType::Blam, entity}); }

  void EmplaceDespawn(Entity entity) const { state_->commands.Emplace(Command{CommandType::Despawn, entity}); }

  void Playback(Registry* registry) const;

 private:
  struct State {
    Channel<Command> commands;
    explicit State(const size_t capacity) : commands(capacity) {}
  };

  std::unique_ptr<State> state_;
};
