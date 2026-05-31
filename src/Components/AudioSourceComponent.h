#pragma once

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

  explicit AudioSourceComponent(std::string t_clipId = "", float t_volume = 1.0f, float t_pitch = 1.0f,
                                bool t_loop = false, bool t_playOnSpawn = true, bool t_despawnOnFinish = false,
                                bool t_spatial = false, float t_minDistance = 50.0f, float t_maxDistance = 1000.0f)
      : clipId(std::move(t_clipId)),
        volume(t_volume),
        pitch(t_pitch),
        loop(t_loop),
        playOnSpawn(t_playOnSpawn),
        despawnOnFinish(t_despawnOnFinish),
        spatial(t_spatial),
        minDistance(t_minDistance),
        maxDistance(t_maxDistance) {}
};
