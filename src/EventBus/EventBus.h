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
  virtual ~IEventCallback() = default;

  void Execute(Event& e) { CallEvent(e); }
};

template <typename TOwner, typename TEvent>
class EventCallback final : public IEventCallback {
  std::function<void(Event&)> invoker_;

 public:
  EventCallback(TOwner* ownerInstance, void (TOwner::*callbackFunction)(TEvent&)) {
    invoker_ = [ownerInstance, callbackFunction](Event& e) {
      std::invoke(callbackFunction, ownerInstance, static_cast<TEvent&>(e));
    };
  }

  EventCallback(TOwner* ownerInstance, void (TOwner::*callbackFunction)(const TEvent&)) {
    invoker_ = [ownerInstance, callbackFunction](Event& e) {
      std::invoke(callbackFunction, ownerInstance, static_cast<const TEvent&>(e));
    };
  }

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
  ~EventBus() { Logger::Info("Event bus destructed"); }

  void Reset() { subscribers_.clear(); }

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