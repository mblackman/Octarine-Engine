#pragma once

#include <cstdint>
#include <string>

struct AudioSourceComponent {
  std::string clipId;
  float volume = 1.0f;
  float pitch = 1.0f;
  bool loop = false;
  bool playOnSpawn = true;
  bool despawnOnFinish = false;

  // Phase 3 spatial: when `spatial` is true, SpatialAudioSystem mutates the track's gain +
  // stereo pan each frame from the listener/emitter geometry. Non-spatial sources keep the
  // raw `volume` from AudioSystem and ignore the spatial system.
  bool spatial = false;
  float minDistance = 50.0f;
  float maxDistance = 1000.0f;

  // Phase 4 doppler: when `doppler` is true AND the entity has a RigidBodyComponent,
  // DopplerSystem applies a per-frame frequency ratio via MIX_SetTrackFrequencyRatio.
  // Independent of `spatial` — a non-spatial doppler source is allowed but unusual.
  bool doppler = false;

  // Phase 5 culling resume: -1 = no pending resume. AudioCullingSystem stashes
  // MIX_GetTrackPlaybackPosition here when it culls a playing emitter, and AudioSystem
  // seeks the new track to this offset (then resets to -1) when the emitter comes back
  // in range and a fresh sink spawns. Looping sources resume mid-loop seamlessly.
  std::int64_t playbackOffsetFrames = -1;

  explicit AudioSourceComponent(std::string t_clipId = "", float t_volume = 1.0f, float t_pitch = 1.0f,
                                bool t_loop = false, bool t_playOnSpawn = true, bool t_despawnOnFinish = false,
                                bool t_spatial = false, float t_minDistance = 50.0f, float t_maxDistance = 1000.0f,
                                bool t_doppler = false)
      : clipId(std::move(t_clipId)),
        volume(t_volume),
        pitch(t_pitch),
        loop(t_loop),
        playOnSpawn(t_playOnSpawn),
        despawnOnFinish(t_despawnOnFinish),
        spatial(t_spatial),
        minDistance(t_minDistance),
        maxDistance(t_maxDistance),
        doppler(t_doppler) {}
};
