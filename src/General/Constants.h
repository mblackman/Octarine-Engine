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

static constexpr size_t kSystemCommandBufferSize = 1024;

// Sized to comfortably hold a frame's worth of render keys per parallel render system without
// hitting Channel<RenderKey>::overflow_buffer (which serializes on a mutex).
static constexpr size_t kRenderCommandBufferSize = 256 * 1024;

// Pre-sized capacity for RenderQueue. Multi-producer parallel systems write directly
// into pre-allocated slots via atomic fetch_add — overflowing is undefined. Size for
// peak frame load (stress tests can hit ~120K+).
constexpr static size_t kInitialRenderQueueCapacity = 256 * 1024;

static constexpr int kDebugUIBaseLayer = 1000;
}  // namespace Constants