#pragma once

#include <functional>
#include <list>
#include <map>
#include <typeindex>

#include "../General/Logger.h"
#include "Event.h"

class IEventCallback {
private:
  virtual void CallEvent(Event& e) = 0;

public:
  virtual ~IEventCallback() = default;

  void Execute(Event& e) { CallEvent(e); }
};

template <typename TOwner, typename TEvent>
class EventCallback : public IEventCallback {
private:
  typedef void (TOwner::*CallbackFunction)(TEvent&);

  TOwner* owner_instance_;
  CallbackFunction callback_function_;

  void CallEvent(Event& e) override {
    std::invoke(callback_function_, owner_instance_, static_cast<TEvent&>(e));
  }

public:
  EventCallback(TOwner* ownerInstance,
                const CallbackFunction callbackFunction) :
    owner_instance_(ownerInstance), callback_function_(callbackFunction) {
  }

  ~EventCallback() override = default;
};

typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;

class EventBus {
private:
  std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers_;

public:
  EventBus() { Logger::Info("Event bus created"); }

  ~EventBus() { Logger::Info("Event bus destructed"); }

  void Reset() { subscribers_.clear(); }

  template <typename TOwner, typename TEvent>
  void SubscribeEvent(TOwner* ownerInstance,
                      void (TOwner::*callbackFunction)(TEvent&)) {
    if (!subscribers_[typeid(TEvent)].get()) {
      subscribers_[typeid(TEvent)] = std::make_unique<HandlerList>();
    }
    auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(
        ownerInstance, callbackFunction);
    subscribers_[typeid(TEvent)]->push_back(std::move(subscriber));
  }

  template <typename TEvent, typename... TArgs>
  void EmitEvent(TArgs&&... args) {
    if (const auto handlers = subscribers_[typeid(TEvent)].get()) {
      for (auto& it : *handlers) {
        auto handler = it.get();
        TEvent event(std::forward<TArgs>(args)...);
        handler->Execute(event);
      }
    }
  }
};