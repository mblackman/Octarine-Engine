#pragma once

#include <glm/glm.hpp>

#include "../Components/AudioListenerComponent.h"
#include "../Components/GlobalTransformComponent.h"
#include "../ECS/Iterable.h"
#include "../ECS/Query.h"
#include "../ECS/Registry.h"
#include "../General/Logger.h"

// Bulk system: snapshots the (first) AudioListenerComponent entity each frame into the
// AudioListenerCache singleton on the Registry. Runs after TransformSystem so the cached
// position reflects this frame's resolved global transform. SpatialAudioSystem reads the
// cache once per emitter — no second archetype walk for the listener.
class UpdateListenerTransformSystem {
 public:
  void operator()(const ContextFacade& ctx, const Iterable& /*iter*/) {
    auto* registry = ctx.GetRegistry();
    auto& cache = registry->Get<AudioListenerCache>();
    cache.valid = false;
    cache.entity = Entity{};

    if (!query_) {
      query_ = registry->CreateQuery<GlobalTransformComponent, AudioListenerComponent>();
    }
    query_->Update();

    bool found = false;
    int listenerCount = 0;
    query_->ForEach([&](const Entity entity, const GlobalTransformComponent& transform,
                        const AudioListenerComponent& listener) {
      ++listenerCount;
      if (found) return;
      cache.entity = entity;
      cache.position = transform.position;
      // 2D top-down basis. When camera rotation lands later, derive forward/right from
      // the camera's world rotation here so the spatial math stays decoupled from any
      // listener-entity rotation (the "Listener Dilemma" decoupling).
      cache.forward = glm::vec2(0.0f, 1.0f);
      cache.right = glm::vec2(1.0f, 0.0f);
      cache.maxDistance = listener.maxDistance;
      cache.rolloff = listener.rolloff;
      cache.valid = true;
      found = true;
    });

    if (listenerCount > 1 && !warnedMultipleListeners_) {
      Logger::Warn("UpdateListenerTransformSystem: " + std::to_string(listenerCount) +
                   " AudioListenerComponent entities found; using the first one.");
      warnedMultipleListeners_ = true;
    }
  }

 private:
  std::unique_ptr<ComponentQuery<GlobalTransformComponent, AudioListenerComponent>> query_;
  bool warnedMultipleListeners_ = false;
};
