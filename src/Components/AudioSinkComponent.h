#pragma once

#include <SDL3_mixer/SDL_mixer.h>

#include <cstdint>

struct AudioSinkComponent {
  MIX_Track* track = nullptr;
  std::uint32_t generation = 0;
  bool finished = false;

  AudioSinkComponent() = default;
  AudioSinkComponent(MIX_Track* t_track, std::uint32_t t_generation) : track(t_track), generation(t_generation) {}
};
