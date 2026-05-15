#pragma once

#include <string>

struct AudioSourceComponent {
  std::string clipId;
  float volume = 1.0f;
  float pitch = 1.0f;
  bool loop = false;
  bool playOnSpawn = true;
  bool despawnOnFinish = false;

  explicit AudioSourceComponent(std::string t_clipId = "", float t_volume = 1.0f, float t_pitch = 1.0f,
                                bool t_loop = false, bool t_playOnSpawn = true, bool t_despawnOnFinish = false)
      : clipId(std::move(t_clipId)),
        volume(t_volume),
        pitch(t_pitch),
        loop(t_loop),
        playOnSpawn(t_playOnSpawn),
        despawnOnFinish(t_despawnOnFinish) {}
};
