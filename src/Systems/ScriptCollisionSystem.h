#pragma once

#include <memory>

#include "Components/ScriptComponent.h"
#include "ECS/Registry.h"
#include "EventBus/EventBus.h"
#include "Events/CollisionBatchEvent.h"
#include "Events/CollisionExitBatchEvent.h"
#include "General/Logger.h"

class ScriptCollisionSystem {
 public:
  void Init(Registry* registry, const std::unique_ptr<EventBus>& eventBus) {
    registry_ = registry;
    enterSubscription_ = eventBus->SubscribeEvent<ScriptCollisionSystem, CollisionBatchEvent>(
        this, &ScriptCollisionSystem::OnCollisionBatch);
    exitSubscription_ = eventBus->SubscribeEvent<ScriptCollisionSystem, CollisionExitBatchEvent>(
        this, &ScriptCollisionSystem::OnCollisionExitBatch);
  }

  void OnCollisionBatch(const CollisionBatchEvent& event) {
    for (const auto& [a, b] : event.pairs) {
      FireEnterCallback(a, b);
      FireEnterCallback(b, a);
    }
  }

  void OnCollisionExitBatch(const CollisionExitBatchEvent& event) {
    for (const auto& [a, b] : event.pairs) {
      FireExitCallback(a, b);
      FireExitCallback(b, a);
    }
  }

 private:
  void FireEnterCallback(const Entity self, const Entity other) const {
    if (!registry_->HasComponent<ScriptComponent>(self)) {
      return;
    }
    auto& script = registry_->GetComponent<ScriptComponent>(self);
    if (script.onCollisionFunction == sol::lua_nil) {
      return;
    }
    if (auto result = script.onCollisionFunction(script.scriptTable, self, other); !result.valid()) {
      const sol::error err = result;
      Logger::ErrorLua(std::string(err.what()));
    }
  }

  void FireExitCallback(const Entity self, const Entity other) const {
    if (!registry_->HasComponent<ScriptComponent>(self)) {
      return;
    }
    auto& script = registry_->GetComponent<ScriptComponent>(self);
    if (script.onCollisionExitFunction == sol::lua_nil) {
      return;
    }
    if (auto result = script.onCollisionExitFunction(script.scriptTable, self, other); !result.valid()) {
      const sol::error err = result;
      Logger::ErrorLua(std::string(err.what()));
    }
  }

  Registry* registry_ = nullptr;
  EventBus::SubscriptionHandle enterSubscription_;
  EventBus::SubscriptionHandle exitSubscription_;
};
