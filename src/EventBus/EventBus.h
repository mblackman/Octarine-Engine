#pragma once

#include <cstdint>
#include <functional>  // Required for std::function
#include <list>
#include <map>
#include <memory>  // Required for std::shared_ptr / std::weak_ptr / std::unique_ptr
#include <typeindex>
#include <utility>

#include "../General/Logger.h"
#include "Event.h"  // Assuming Event is a base class or relevant type

// Each subscription is tagged with a process-unique id so a SubscriptionHandle can later find and
// drop exactly the one callback it owns. Replaces the old `void* owner` identity, which forced
// callers to re-find their callbacks by raw pointer in a destructor.
class IEventCallback {
  virtual void CallEvent(Event& e) = 0;

 public:
  explicit IEventCallback(std::uint64_t id) : id_(id) {}

  IEventCallback(const IEventCallback&) = delete;
  IEventCallback& operator=(const IEventCallback&) = delete;

  IEventCallback(IEventCallback&&) = delete;
  IEventCallback& operator=(IEventCallback&&) = delete;

  virtual ~IEventCallback() = default;

  void Execute(Event& e) { CallEvent(e); }

  [[nodiscard]] std::uint64_t Id() const { return id_; }

 private:
  std::uint64_t id_;
};

template <typename TOwner, typename TEvent>
class EventCallback final : public IEventCallback {
  std::function<void(Event&)> invoker_;

 public:
  EventCallback(std::uint64_t id, TOwner* ownerInstance, void (TOwner::*callbackFunction)(TEvent&))
      : IEventCallback(id), invoker_([ownerInstance, callbackFunction](Event& e) {
          std::invoke(callbackFunction, ownerInstance, static_cast<TEvent&>(e));
        }) {}

  // Constructor for const TEvent&
  EventCallback(std::uint64_t id, TOwner* ownerInstance, void (TOwner::*callbackFunction)(const TEvent&))
      : IEventCallback(id), invoker_([ownerInstance, callbackFunction](Event& e) {
          std::invoke(callbackFunction, ownerInstance, static_cast<const TEvent&>(e));
        }) {}

  EventCallback(const EventCallback&) = delete;
  EventCallback& operator=(const EventCallback&) = delete;

  EventCallback(EventCallback&&) = delete;
  EventCallback& operator=(EventCallback&&) = delete;

  ~EventCallback() override = default;

 private:
  void CallEvent(Event& e) override {
    if (invoker_) {
      invoker_(e);
    }
  }
};

typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;

class EventBus {
  // The dispatch table is held behind a shared_ptr so SubscriptionHandles can hold a weak_ptr to
  // it. A handle that outlives the bus then finds the table expired and unsubscribes to nothing —
  // shutdown order between the bus and its subscribers no longer matters.
  struct Dispatcher {
    std::map<std::type_index, HandlerList> subscribers;

    void Remove(const std::type_index& eventTypeId, std::uint64_t id) {
      const auto it = subscribers.find(eventTypeId);
      if (it == subscribers.end()) return;
      it->second.remove_if([id](const std::unique_ptr<IEventCallback>& cb) { return cb && cb->Id() == id; });
    }
  };

  std::shared_ptr<Dispatcher> dispatcher_ = std::make_shared<Dispatcher>();
  std::uint64_t next_id_ = 1;

 public:
  // RAII handle for a single subscription. Its destructor removes the callback from the bus, so a
  // subscriber just keeps the handle (or a vector of them) as a member and never has to unsubscribe
  // by hand. Move-only; a default-constructed or moved-from handle owns nothing. Resetting after the
  // bus has been destroyed is a safe no-op.
  class [[nodiscard]] SubscriptionHandle {
   public:
    SubscriptionHandle() = default;

    SubscriptionHandle(std::weak_ptr<Dispatcher> dispatcher, std::type_index eventTypeId, std::uint64_t id)
        : dispatcher_(std::move(dispatcher)), event_type_id_(eventTypeId), id_(id) {}

    SubscriptionHandle(const SubscriptionHandle&) = delete;
    SubscriptionHandle& operator=(const SubscriptionHandle&) = delete;

    SubscriptionHandle(SubscriptionHandle&& other) noexcept
        : dispatcher_(std::move(other.dispatcher_)), event_type_id_(other.event_type_id_), id_(other.id_) {
      other.dispatcher_.reset();
    }

    SubscriptionHandle& operator=(SubscriptionHandle&& other) noexcept {
      if (this != &other) {
        Reset();
        dispatcher_ = std::move(other.dispatcher_);
        event_type_id_ = other.event_type_id_;
        id_ = other.id_;
        other.dispatcher_.reset();
      }
      return *this;
    }

    ~SubscriptionHandle() { Reset(); }

    // Unsubscribe now instead of waiting for the destructor. Idempotent.
    void Reset() {
      if (const auto dispatcher = dispatcher_.lock()) {
        dispatcher->Remove(event_type_id_, id_);
      }
      dispatcher_.reset();
    }

    // True while the subscription is still live on a surviving bus.
    [[nodiscard]] bool Active() const { return !dispatcher_.expired(); }

   private:
    std::weak_ptr<Dispatcher> dispatcher_;
    std::type_index event_type_id_ = typeid(void);
    std::uint64_t id_ = 0;
  };

  EventBus() { Logger::Info("Event bus created"); }

  EventBus(const EventBus&) = delete;
  EventBus& operator=(const EventBus&) = delete;

  EventBus(EventBus&&) = delete;
  EventBus& operator=(EventBus&&) = delete;

  ~EventBus() { Logger::Info("Event bus destructed"); }

  template <typename TOwner, typename TEvent>
  [[nodiscard]] SubscriptionHandle SubscribeEvent(TOwner* ownerInstance, void (TOwner::*callbackFunction)(TEvent&)) {
    const std::type_index eventTypeId = typeid(TEvent);
    const std::uint64_t id = next_id_++;
    dispatcher_->subscribers[eventTypeId].push_back(
        std::make_unique<EventCallback<TOwner, TEvent>>(id, ownerInstance, callbackFunction));
    return SubscriptionHandle(dispatcher_, eventTypeId, id);
  }

  template <typename TOwner, typename TEvent>
  [[nodiscard]] SubscriptionHandle SubscribeEvent(TOwner* ownerInstance,
                                                  void (TOwner::*callbackFunction)(const TEvent&)) {
    const std::type_index eventTypeId = typeid(TEvent);
    const std::uint64_t id = next_id_++;
    dispatcher_->subscribers[eventTypeId].push_back(
        std::make_unique<EventCallback<TOwner, TEvent>>(id, ownerInstance, callbackFunction));
    return SubscriptionHandle(dispatcher_, eventTypeId, id);
  }

  template <typename TEvent, typename... TArgs>
  void EmitEvent(TArgs&&... args) {
    const std::type_index eventTypeId = typeid(TEvent);
    const auto it = dispatcher_->subscribers.find(eventTypeId);
    if (it != dispatcher_->subscribers.end()) {
      TEvent event(std::forward<TArgs>(args)...);
      for (auto& callback_ptr : it->second) {
        if (callback_ptr) {
          callback_ptr->Execute(event);
        }
      }
    }
  }
};
