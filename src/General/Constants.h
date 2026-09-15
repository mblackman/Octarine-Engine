#pragma once
#include <SDL3/SDL_stdinc.h>

namespace Constants {
// Default target frame rate (0 = uncapped).
static constexpr int kFps = 60;
static constexpr float kTwo = 2.0f;
static constexpr Uint8 kUint8Max = 255;
static constexpr int kDefaultWindowWidth = 1280;
static constexpr int kDefaultWindowHeight = 720;

static constexpr int kDefaultEntityMask = 1;

static constexpr size_t kSystemCommandBufferSize = 1024;

// Minimum collision pair count to justify parallel classification across thread pool.
static constexpr size_t kCollisionResponseParallelThreshold = 2048;

// Per-system render command buffer capacity before falling back to mutexed overflow.
static constexpr size_t kRenderCommandBufferSize = 256 * 1024;

// Pre-allocated RenderQueue slot capacity for lock-free atomic multi-producer writes.
constexpr static size_t kInitialRenderQueueCapacity = 256 * 1024;

static constexpr int kDebugUIBaseLayer = 1000;

// Y-axis quantization band (world pixels) for texture batching. Set to 0.0f for strict painter's order.
static constexpr float kRenderBatchYBandPx = 32.0f;
}  // namespace Constants