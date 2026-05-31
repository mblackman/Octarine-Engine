#pragma once

#include <glm/glm.hpp>

#include "ECS/Entity.h"

struct AudioListenerComponent {
  float maxDistance = 1000.0f;
  float rolloff = 1.0f;

  AudioListenerComponent() = default;
  explicit AudioListenerComponent(float t_maxDistance, float t_rolloff = 1.0f)
      : maxDistance(t_maxDistance), rolloff(t_rolloff) {}
};

// Singleton cache populated each frame by UpdateListenerTransformSystem and read by
// SpatialAudioSystem. Decouples spatial-audio math from the listener-entity query so the
// per-emitter callback can short-circuit without re-iterating archetypes. `valid` flips to
// false when no listener entity exists; emitters skip spatial logic in that case.
struct AudioListenerCache {
  Entity entity{};
  glm::vec2 position{0.0f, 0.0f};
  glm::vec2 forward{0.0f, 1.0f};
  glm::vec2 right{1.0f, 0.0f};
  float maxDistance = 1000.0f;
  float rolloff = 1.0f;
  bool valid = false;
};
