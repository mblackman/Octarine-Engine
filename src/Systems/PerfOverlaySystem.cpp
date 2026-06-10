#include "Systems/PerfOverlaySystem.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdio>
#include <string>

#include "AssetManager/AssetManager.h"
#include "ECS/Registry.h"
#include "Game/GameConfig.h"
#include "General/Constants.h"
#include "General/Fonts/Roboto_Medium.h"
#include "General/Logger.h"
#include "General/PerfUtils.h"
#include "General/Utils.h"

namespace {
// Reserved catalog id for the overlay's embedded font. Underscore-prefixed so it can't collide with
// a project asset id.
constexpr char kDebugFontId[] = "__octarine_perf_overlay";
constexpr float kDebugFontSize = 16.0F;

constexpr float kMargin = 8.0F;   // inset of the text block from the chosen corner
constexpr float kPadding = 6.0F;  // background panel inset around the text block
constexpr float kLineGap = 2.0F;  // vertical gap between lines

constexpr int kTextBufSize = 32;                // per-line snprintf scratch
constexpr Uint8 kBgAlpha = 150;                 // translucency of the background panel
constexpr Uint8 kWhite = Constants::kUint8Max;  // labels are rastered white, then tinted at draw time

// FPS color thresholds: at or above green stays green, above yellow stays yellow, else red.
constexpr float kFpsGreenThreshold = 55.0F;
constexpr float kFpsYellowThreshold = 30.0F;
// Frame-time color thresholds (ms): at or below green stays green, below yellow stays yellow, else red.
constexpr float kFrameMsGreenThreshold = 18.0F;
constexpr float kFrameMsYellowThreshold = 33.0F;

struct Rgb {
  Uint8 r;
  Uint8 g;
  Uint8 b;
};

constexpr Rgb kGreen{60, 200, 90};
constexpr Rgb kYellow{230, 200, 50};
constexpr Rgb kRed{220, 70, 60};

struct Vec2 {
  float x;
  float y;
};

Rgb FpsColor(const float fps) {
  if (fps >= kFpsGreenThreshold) return kGreen;
  if (fps >= kFpsYellowThreshold) return kYellow;
  return kRed;
}

Rgb FrameMsColor(const float ms) {
  if (ms <= kFrameMsGreenThreshold) return kGreen;
  if (ms <= kFrameMsYellowThreshold) return kYellow;
  return kRed;
}

// Top-left origin of the text block for the configured corner of the render target. Falls back to
// top-left when the render output size is unavailable.
Vec2 BlockOrigin(const PerfOverlayCorner corner, SDL_Renderer* sdlRenderer, const float blockW, const float blockH) {
  int outW = 0;
  int outH = 0;
  if (!SDL_GetCurrentRenderOutputSize(sdlRenderer, &outW, &outH) || outW <= 0 || outH <= 0) {
    return {kMargin, kMargin};
  }
  const float rightX = static_cast<float>(outW) - kMargin - blockW;
  const float bottomY = static_cast<float>(outH) - kMargin - blockH;
  switch (corner) {
    case PerfOverlayCorner::TopRight:
      return {rightX, kMargin};
    case PerfOverlayCorner::BottomLeft:
      return {kMargin, bottomY};
    case PerfOverlayCorner::BottomRight:
      return {rightX, bottomY};
    case PerfOverlayCorner::TopLeft:
    default:
      return {kMargin, kMargin};
  }
}
}  // namespace

PerfOverlaySystem::PerfOverlaySystem() { fps_metrics_.reserve(kFpsBuffer); }

PerfOverlaySystem::~PerfOverlaySystem() {
  for (auto& line : lines_) {
    if (line.texture != nullptr) SDL_DestroyTexture(line.texture);
  }
}

TTF_Font* PerfOverlaySystem::EnsureFont(AssetManager& assetManager) {
  if (TTF_Font* font = assetManager.GetFont(kDebugFontId); font != nullptr) return font;
  if (font_init_attempted_) return nullptr;  // a prior registration failed; don't retry/respam
  font_init_attempted_ = true;
  assetManager.AddFontFromMemory(kDebugFontId, octarine::fonts::kRobotoMediumData, octarine::fonts::kRobotoMediumSize,
                                 kDebugFontSize);
  return assetManager.GetFont(kDebugFontId);
}

bool PerfOverlaySystem::UpdateLine(Line& line, TTF_Font* font, SDL_Renderer* sdlRenderer,
                                   const std::string& text) const {
  if (line.texture != nullptr && line.text == text) return true;  // cache hit: string unchanged

  SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), 0, SDL_Color{kWhite, kWhite, kWhite, kWhite});
  if (surface == nullptr) {
    Logger::Error("PerfOverlay: text render failed: " + std::string(SDL_GetError()));
    return false;
  }
  SDL_Texture* texture = SDL_CreateTextureFromSurface(sdlRenderer, surface);
  SDL_DestroySurface(surface);
  if (texture == nullptr) {
    Logger::Error("PerfOverlay: texture create failed: " + std::string(SDL_GetError()));
    return false;
  }

  if (line.texture != nullptr) SDL_DestroyTexture(line.texture);
  line.texture = texture;
  line.text = text;
  SDL_GetTextureSize(texture, &line.width, &line.height);
  return true;
}

// Rasterize the four FPS lines (current/avg/p95/p99) into slots 0-3. Returns false if any raster fails.
bool PerfOverlaySystem::UpdateFpsLines(TTF_Font* font, SDL_Renderer* sdlRenderer, const float fps, const float avgFps,
                                       const float fps95, const float fps99) {
  char fpsBuf[kTextBufSize];
  char avgFpsBuf[kTextBufSize];
  char fpsBufP95[kTextBufSize];
  char fpsBufP99[kTextBufSize];
  std::snprintf(fpsBuf, sizeof(fpsBuf), "FPS %5.1f", static_cast<double>(fps));
  std::snprintf(avgFpsBuf, sizeof(avgFpsBuf), "AVG FPS %5.1f", static_cast<double>(avgFps));
  std::snprintf(fpsBufP95, sizeof(fpsBufP95), "95P FPS %5.1f", static_cast<double>(fps95));
  std::snprintf(fpsBufP99, sizeof(fpsBufP99), "99P FPS %5.1f", static_cast<double>(fps99));

  return UpdateLine(lines_[0], font, sdlRenderer, fpsBuf) && UpdateLine(lines_[1], font, sdlRenderer, avgFpsBuf) &&
         UpdateLine(lines_[2], font, sdlRenderer, fpsBufP95) && UpdateLine(lines_[3], font, sdlRenderer, fpsBufP99);
}

void PerfOverlaySystem::RecordFpsSample(const float fps) {
  // Ring buffer of recent FPS samples: fill first, then overwrite oldest.
  if (fps_metrics_.size() < kFpsBuffer) {
    fps_metrics_.push_back(fps);
  } else {
    fps_metrics_[fps_metric_index_] = fps;
    fps_metric_index_ = (fps_metric_index_ + 1) % kFpsBuffer;
  }
}

void PerfOverlaySystem::Draw(Registry& registry, SDL_Renderer* sdlRenderer, const float deltaTime) {
  if (sdlRenderer == nullptr) return;
  auto& assetManager = registry.Get<AssetManager>();
  TTF_Font* font = EnsureFont(assetManager);
  if (font == nullptr) return;

  const auto& options = registry.Get<GameConfig>().GetEngineOptions();
  const bool showFps = HasFlag(options.perfOverlayMetrics, PerfOverlayMetrics::Fps);
  const bool showMs = HasFlag(options.perfOverlayMetrics, PerfOverlayMetrics::FrameTime);

  const float fps = deltaTime > 0.0F ? 1.0F / deltaTime : 0.0F;
  const float frameMs = deltaTime * 1000.0F;

  RecordFpsSample(fps);

  // Gather the active lines in draw order (slots 0-3 = FPS/avg/p95/p99, slot 4 = frame time).
  // Inactive metrics keep their cached texture but aren't rastered/drawn this frame.
  struct ActiveLine {
    std::size_t slot;
    Rgb color;
  };
  std::array<ActiveLine, kLineCount> active{};
  std::size_t count = 0;
  if (showFps) {
    const float avgFps = GetAverage(fps_metrics_);
    // Percentile lows: "95P" is the FPS exceeded 95% of the time (5th percentile of the samples),
    // matching the 1%/5% low convention of game perf overlays.
    const float fps95 = PerfUtils::GetPercentile(fps_metrics_, 0.05F);
    const float fps99 = PerfUtils::GetPercentile(fps_metrics_, 0.01F);

    if (!UpdateFpsLines(font, sdlRenderer, fps, avgFps, fps95, fps99)) return;
    active[count++] = {0, FpsColor(fps)};
    active[count++] = {1, FpsColor(avgFps)};
    active[count++] = {2, FpsColor(fps95)};
    active[count++] = {3, FpsColor(fps99)};
  }
  if (showMs) {
    char msBuf[kTextBufSize];
    std::snprintf(msBuf, sizeof(msBuf), "%5.2f ms", static_cast<double>(frameMs));
    if (!UpdateLine(lines_[4], font, sdlRenderer, msBuf)) return;
    active[count++] = {4, FrameMsColor(frameMs)};
  }
  if (count == 0) return;

  float blockW = 0.0F;
  float blockH = 0.0F;
  for (std::size_t i = 0; i < count; ++i) {
    const Line& line = lines_[active[i].slot];
    blockW = std::max(blockW, line.width);
    blockH += line.height + kLineGap;
  }
  blockH -= kLineGap;  // no trailing gap after the last line

  // Anchor the block to the configured corner of the current render target (the scene texture).
  const Vec2 origin = BlockOrigin(options.perfOverlayCorner, sdlRenderer, blockW, blockH);

  // Translucent background panel behind the text for legibility over any scene.
  SDL_SetRenderDrawBlendMode(sdlRenderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, kBgAlpha);
  const SDL_FRect bg{origin.x - kPadding, origin.y - kPadding, blockW + (2.0F * kPadding), blockH + (2.0F * kPadding)};
  SDL_RenderFillRect(sdlRenderer, &bg);

  float y = origin.y;
  for (std::size_t i = 0; i < count; ++i) {
    Line& line = lines_[active[i].slot];
    SDL_SetTextureColorMod(line.texture, active[i].color.r, active[i].color.g, active[i].color.b);
    const SDL_FRect dst{origin.x, y, line.width, line.height};
    SDL_RenderTexture(sdlRenderer, line.texture, nullptr, &dst);
    y += line.height + kLineGap;
  }
}
