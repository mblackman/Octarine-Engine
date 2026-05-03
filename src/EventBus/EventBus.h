#pragma once

#include <functional>  // Required for std::function
#include <list>
#include <map>
#include <memory>  // Required for std::unique_ptr and std::make_unique
#include <typeindex>

#include "../General/Logger.h"
#include "Event.h"  // Assuming Event is a base class or relevant type

class IEventCallback {
  virtual void CallEvent(Event& e) = 0;

 public:
  explicit IEventCallback(void* owner = nullptr) : owner_(owner) {}

  IEventCallback(const IEventCallback&) = delete;
  IEventCallback& operator=(const IEventCallback&) = delete;

  IEventCallback(IEventCallback&&) = delete;
  IEventCallback& operator=(IEventCallback&&) = delete;

  virtual ~IEventCallback() = default;

  void Execute(Event& e) { CallEvent(e); }

  [[nodiscard]] void* Owner() const { return owner_; }

 private:
  void* owner_;
};

template <typename TOwner, typename TEvent>
class EventCallback final : public IEventCallback {
  std::function<void(Event&)> invoker_;

 public:
  EventCallback(TOwner* ownerInstance, void (TOwner::*callbackFunction)(TEvent&))
      : IEventCallback(static_cast<void*>(ownerInstance)), invoker_([ownerInstance, callbackFunction](Event& e) {
          std::invoke(callbackFunction, ownerInstance, static_cast<TEvent&>(e));
        }) {}

  // Constructor for const TEvent&
  EventCallback(TOwner* ownerInstance, void (TOwner::*callbackFunction)(const TEvent&))
      : IEventCallback(static_cast<void*>(ownerInstance)), invoker_([ownerInstance, callbackFunction](Event& e) {
          std::invoke(callbackFunction, ownerInstance, static_cast<const TEvent&>(e));
        }) {}

  EventCallback(const EventCallback&) = delete;
  EventCallback& operator=(const EventCallback&) = delete;

  EventCallback(EventCallback&&) = delete;
  EventCallback& operator=(EventCallback&&) = delete;

  ~EventCallback() override = default;

  void CallEvent(Event& e) override {
    if (invoker_) {
      invoker_(e);
    }
  }
};

typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;

class EventBus {
  std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers_;

 public:
  EventBus() { Logger::Info("Event bus created"); }

  EventBus(const EventBus&) = delete;
  EventBus& operator=(const EventBus&) = delete;

  EventBus(EventBus&&) = delete;
  EventBus& operator=(EventBus&&) = delete;

  ~EventBus() { Logger::Info("Event bus destructed"); }

  template <typename TOwner, typename TEvent>
  void SubscribeEvent(TOwner* ownerInstance, void (TOwner::*callbackFunction)(TEvent&)) {
    const std::type_index eventTypeId = typeid(TEvent);
    if (!subscribers_[eventTypeId]) {
      subscribers_[eventTypeId] = std::make_unique<HandlerList>();
    }
    auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(ownerInstance, callbackFunction);
    subscribers_[eventTypeId]->push_back(std::move(subscriber));
  }

  template <typename TOwner, typename TEvent>
  void SubscribeEvent(TOwner* ownerInstance, void (TOwner::*callbackFunction)(const TEvent&)) {
    const std::type_index eventTypeId = typeid(TEvent);
    if (!subscribers_[eventTypeId]) {
      subscribers_[eventTypeId] = std::make_unique<HandlerList>();
    }
    auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(ownerInstance, callbackFunction);
    subscribers_[eventTypeId]->push_back(std::move(subscriber));
  }

  // Drop every subscription whose owner pointer matches `owner`. Call from a system's
  // destructor when its lifetime can end before the bus's, otherwise the bus would invoke
  // a dangling callback on the next emit.
  void UnsubscribeAll(void* owner) {
    if (!owner) return;
    for (auto& [_, handlers] : subscribers_) {
      if (!handlers) continue;
      handlers->remove_if([owner](const std::unique_ptr<IEventCallback>& cb) { return cb && cb->Owner() == owner; });
    }
  }

  template <typename TEvent, typename... TArgs>
  void EmitEvent(TArgs&&... args) {
    const std::type_index eventTypeId = typeid(TEvent);
    auto it = subscribers_.find(eventTypeId);
    if (it != subscribers_.end() && it->second) {
      TEvent event(std::forward<TArgs>(args)...);
      for (auto& callback_ptr : *(it->second)) {
        if (callback_ptr) {
          callback_ptr->Execute(event);
        }
      }
    }
  }
};