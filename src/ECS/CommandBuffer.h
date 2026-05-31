#pragma once

#include <algorithm>
#include <atomic>
#include <functional>
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
// on the main thread (e.g. entity destruction).
class CommandBuffer {
 public:
  enum class CommandType { Blam, Despawn };
  struct EntityCommand {
    CommandType type;
    Entity entity;
  };
  using DeferredFn = std::function<void(Registry*)>;

  explicit CommandBuffer(size_t capacity = Constants::kSystemCommandBufferSize)
      : state_(std::make_unique<State>(capacity)) {}

  CommandBuffer(CommandBuffer&&) = default;
  CommandBuffer& operator=(CommandBuffer&&) = default;
  CommandBuffer(const CommandBuffer&) = delete;
  CommandBuffer& operator=(const CommandBuffer&) = delete;

  void EmplaceBlam(Entity entity) const { state_->commands.Emplace(EntityCommand{CommandType::Blam, entity}); }

  void EmplaceDespawn(Entity entity) const { state_->commands.Emplace(EntityCommand{CommandType::Despawn, entity}); }

  template <typename T>
  void AddComponent(Entity entity, T component) const {
    state_->deferred.Emplace(
        [entity, c = std::move(component)](Registry* r) mutable { r->AddComponent(entity, std::move(c)); });
  }

  template <typename T>
  void RemoveComponent(Entity entity) const {
    state_->deferred.Emplace([entity](Registry* r) { r->RemoveComponent<T>(entity); });
  }

  template <typename T>
  void AddTag(Entity entity) const {
    state_->deferred.Emplace([entity](Registry* r) { r->AddTag<T>(entity); });
  }

  template <typename T>
  void RemoveTag(Entity entity) const {
    state_->deferred.Emplace([entity](Registry* r) { r->RemoveTag<T>(entity); });
  }

  void Playback(Registry* registry) const;

 private:
  struct State {
    Channel<EntityCommand> commands;
    Channel<DeferredFn> deferred;
    explicit State(const size_t capacity) : commands(capacity), deferred(capacity) {}
  };

  std::unique_ptr<State> state_;
};
