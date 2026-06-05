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

// Below this collision-pair count, the collision-response classify pass (DamageSystem /
// ObstacleBounceSystem) runs serially. ThreadPool dispatch overhead outweighs the per-pair tag
// classification until a few thousand pairs; realistic scenes (~hundreds of pairs) stay serial,
// while pathological density (tens of thousands) takes the parallel path. Tunable.
static constexpr size_t kCollisionResponseParallelThreshold = 2048;

// Sized to comfortably hold a frame's worth of render keys per parallel render system without
// hitting Channel<RenderKey>::overflow_buffer (which serializes on a mutex).
static constexpr size_t kRenderCommandBufferSize = 256 * 1024;

// Pre-sized capacity for RenderQueue. Multi-producer parallel systems write directly
// into pre-allocated slots via atomic fetch_add — overflowing is undefined. Size for
// peak frame load (stress tests can hit ~120K+).
constexpr static size_t kInitialRenderQueueCapacity = 256 * 1024;

static constexpr int kDebugUIBaseLayer = 1000;

// Y-band size (world pixels) for grouping render keys before secondary sort by texture.
// Within a band, sprites sharing a texture sort adjacent so SDL3's internal renderer
// can coalesce them into a single batched draw. Larger bands batch harder but allow
// painter's-order swaps between sprites of different textures inside the band — pick
// a value close to one tile/character height for top-down scenes. Set to 0.0f to
// disable banding and restore strict per-pixel painter's order.
static constexpr float kRenderBatchYBandPx = 32.0f;
}  // namespace Constants