#pragma once
#include <SDL3/SDL_stdinc.h>

namespace Constants {
static constexpr int kMillisecondsPerSecond = 1000;
static constexpr int kFps = 60;
static constexpr int kMillisecondsPerFrame = kMillisecondsPerSecond / kFps;
static constexpr float kTwo = 2.0f;
static constexpr Uint8 kUint8Max = 255;
static constexpr int kDefaultWindowWidth = 1280;
static constexpr int kDefaultWindowHeight = 720;

static constexpr int kDefaultEntityMask = 1;

static constexpr int kSystemCommandBufferSize = 1024;

static constexpr int kDebugUIBaseLayer = 1000;
}  // namespace Constants