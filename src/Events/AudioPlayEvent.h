#pragma once

#include <string>

#include "../EventBus/Event.h"

struct AudioPlayEvent : Event {
  std::string clipId;
  float volume;

  AudioPlayEvent(std::string clipId, const float volume) : clipId(std::move(clipId)), volume(volume) {}
};
