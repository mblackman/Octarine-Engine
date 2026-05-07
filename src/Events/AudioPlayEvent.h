#pragma once

#include <string>

#include "../EventBus/Event.h"

struct AudioPlayEvent : Event {
  std::string clipId;
  float volume;

  AudioPlayEvent(std::string t_clipId, const float t_volume) : clipId(std::move(t_clipId)), volume(t_volume) {}
};
