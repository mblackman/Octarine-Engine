#pragma once

#include <SDL3_mixer/SDL_mixer.h>

#include <cstdint>

struct AudioSinkComponent {
  MIX_Track* track = nullptr;
  std::uint32_t generation = 0;
  bool finished = false;

  // Cached state for SpatialAudioSystem + DopplerSystem so they only call MIX_Set* when
  // the computed value changed. Initial sentinels (lastGain < 0, lastPan outside [-1, 1],
  // lastRatio < 0) force the first frame to apply regardless. `stereoApplied` separately
  // tracks whether MIX_SetTrackStereo has a non-null override currently set; flipping a
  // source from spatial→non-spatial calls MIX_SetTrackStereo(nullptr) once and clears it.
  float lastGain = -1.0f;
  float lastPan = 2.0f;
  float lastRatio = -1.0f;
  bool stereoApplied = false;

  AudioSinkComponent() = default;
  AudioSinkComponent(MIX_Track* t_track, std::uint32_t t_generation) : track(t_track), generation(t_generation) {}
};
