#pragma once

#include <glm/glm.hpp>

#include "ECS/Entity.h"

struct AudioListenerComponent {
  float maxDistance = 1000.0f;
  float rolloff = 1.0f;

  // Phase 4 Doppler. Factor 0 disables the shift, 1 is physical, >1 exaggerates.
  // speedOfSound is in world units / second; tune per project (default 343 ≈ m/s if
  // 1 world unit ≈ 1 m).
  float dopplerFactor = 1.0f;
  float speedOfSound = 343.0f;

  AudioListenerComponent() = default;
  explicit AudioListenerComponent(float t_maxDistance, float t_rolloff = 1.0f, float t_dopplerFactor = 1.0f,
                                  float t_speedOfSound = 343.0f)
      : maxDistance(t_maxDistance),
        rolloff(t_rolloff),
        dopplerFactor(t_dopplerFactor),
        speedOfSound(t_speedOfSound) {}
};

// Singleton cache populated each frame by UpdateListenerTransformSystem and read by
// SpatialAudioSystem / DopplerSystem. Decouples spatial-audio math from the listener-entity
// query so per-emitter callbacks can short-circuit without re-iterating archetypes.
// `valid` flips to false when no listener entity exists; emitters skip spatial logic in
// that case.
struct AudioListenerCache {
  Entity entity{};
  glm::vec2 position{0.0f, 0.0f};
  glm::vec2 velocity{0.0f, 0.0f};
  glm::vec2 forward{0.0f, 1.0f};
  glm::vec2 right{1.0f, 0.0f};
  float maxDistance = 1000.0f;
  float rolloff = 1.0f;
  float dopplerFactor = 1.0f;
  float speedOfSound = 343.0f;
  bool valid = false;
};
