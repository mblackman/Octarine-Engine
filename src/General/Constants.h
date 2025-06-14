#pragma once
#include <SDL3/SDL_stdinc.h>

namespace Constants {
static constexpr int kMillisecondsPerSecond = 1000;
static constexpr int kFps = 60;
static constexpr int kMillisecondsPerFrame = kMillisecondsPerSecond / kFps;
static constexpr float kHalf = 2.0f;
static constexpr Uint8 kUnt8Max = 255;
static constexpr int kDefaultWindowWidth = 1280;
static constexpr int kDefaultWindowHeight = 720;

static constexpr int kDefaultEntityMask = 1;
}  // namespace Constants