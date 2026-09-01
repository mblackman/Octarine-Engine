#pragma once

#include "Components/LifetimeComponent.h"
#include "ECS/Context.h"
#include "ECS/Query.h"

class LifetimeSystem {
 public:
  void operator()(const ContextFacade& ctx, const Iterable& /*iter*/) {
    auto* registry = ctx.GetRegistry();
    const auto deltaTime = ctx.GetDeltaTime();
    EnsureInitialized(registry);
    lifetimeQuery_->Update();

    lifetimeQuery_->ForEach([deltaTime, registry](const Entity entity, LifetimeComponent& lifetime) {
      lifetime.Decrease(deltaTime);

      if (!lifetime.IsAlive()) {
        registry->QueueDespawnEntity(entity);
      }
    });
  }

 private:
  using LifetimeQuery = ComponentQuery<LifetimeComponent>;

  std::unique_ptr<LifetimeQuery> lifetimeQuery_;
  Entity lifetimeEntity_ = {};

  void EnsureInitialized(Registry* registry) {
    if (lifetimeQuery_) return;
    lifetimeQuery_ = registry->CreateQuery<LifetimeComponent>();
    lifetimeEntity_ = registry->Component<LifetimeComponent>();
  }
};
