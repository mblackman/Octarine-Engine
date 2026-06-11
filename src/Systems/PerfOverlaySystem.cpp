#include "Systems/PerfOverlaySystem.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdio>
#include <string>
#include <utility>

#include "AssetManager/AssetManager.h"
#include "ECS/Registry.h"
#include "Game/GameConfig.h"
#include "General/Constants.h"
#include "General/Fonts/Roboto_Medium.h"
#include "General/Logger.h"
#include "General/PerfUtils.h"
#include "General/SystemMemory.h"
#include "General/Utils.h"

namespace {
// Reserved catalog id for the overlay's embedded font. Underscore-prefixed so it can't collide with
// a project asset id.
constexpr char kDebugFontId[] = "__octarine_perf_overlay";
constexpr float kDebugFontSize = 16.0F;

constexpr float kMargin = 8.0F;                        // inset of the text block from the chosen corner
constexpr float kPadding = 6.0F;                       // background panel inset around the text block
constexpr float kLineGap = 2.0F;                       // vertical gap between rows
constexpr float kColGap = 14.0F;                       // minimum gap between the label column and the value column
constexpr float kSepPad = 3.0F;                        // vertical padding above/below a section separator hairline
constexpr float kSepBlockH = (2.0F * kSepPad) + 1.0F;  // total height a separator adds to the block

constexpr int kTextBufSize = 32;               // per-line snprintf scratch
constexpr float kMemoryRefreshSeconds = 0.5F;  // resident-memory poll cadence (it's a syscall)
constexpr double kBytesPerMb = 1024.0 * 1024.0;
constexpr Uint8 kBgAlpha = 150;                 // translucency of the background panel
constexpr Uint8 kBorderAlpha = 60;              // translucency of the panel border
constexpr Uint8 kSepAlpha = 50;                 // translucency of section separator hairlines
constexpr Uint8 kWhite = Constants::kUint8Max;  // text is rastered white, then tinted at draw time

// FPS color thresholds: at or above green stays green, above yellow stays yellow, else red.
constexpr float kFpsGreenThreshold = 55.0F;
constexpr float kFpsYellowThreshold = 30.0F;
// Frame-time color thresholds (ms): at or below green stays green, below yellow stays yellow, else red.
constexpr float kFrameMsGreenThreshold = 18.0F;
constexpr float kFrameMsYellowThreshold = 33.0F;

struct Vec2 {
  float x;
  float y;
};
}  // namespace

// Static row-tint palette. Labels stay dim so the colored values carry the signal.
namespace {
constexpr PerfOverlaySystem::Rgb kGreen{60, 200, 90};
constexpr PerfOverlaySystem::Rgb kYellow{230, 200, 50};
constexpr PerfOverlaySystem::Rgb kRed{220, 70, 60};
constexpr PerfOverlaySystem::Rgb kNeutral{225, 228, 230};  // values with no good/bad threshold
constexpr PerfOverlaySystem::Rgb kLabelTint{170, 175, 180};

PerfOverlaySystem::Rgb FpsColor(const float fps) {
  if (fps >= kFpsGreenThreshold) return kGreen;
  if (fps >= kFpsYellowThreshold) return kYellow;
  return kRed;
}

PerfOverlaySystem::Rgb FrameMsColor(const float ms) {
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

PerfOverlaySystem::PerfOverlaySystem() {
  fps_samples_.reserve(kSampleBuffer);
  ms_samples_.reserve(kSampleBuffer);
}

PerfOverlaySystem::~PerfOverlaySystem() {
  for (auto& row : rows_) {
    if (row.label.texture != nullptr) SDL_DestroyTexture(row.label.texture);
    if (row.value.texture != nullptr) SDL_DestroyTexture(row.value.texture);
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

bool PerfOverlaySystem::UpdateRow(const std::size_t slot, TTF_Font* font, SDL_Renderer* sdlRenderer, const char* label,
                                  const char* value) {
  Row& row = rows_[slot];
  return UpdateLine(row.label, font, sdlRenderer, label) && UpdateLine(row.value, font, sdlRenderer, value);
}

bool PerfOverlaySystem::AppendFpsRows(TTF_Font* font, SDL_Renderer* sdlRenderer, const float fps,
                                      std::array<ActiveRow, kRowCount>& active, std::size_t& count) {
  // Percentile lows: "P95" is the FPS exceeded 95% of the time (5th percentile of the samples),
  // matching the 1%/5% low convention of game perf overlays.
  const std::array<std::pair<const char*, float>, 5> entries{{{"FPS", fps},
                                                              {"AVG", GetAverage(fps_samples_)},
                                                              {"P50", PerfUtils::GetPercentile(fps_samples_, 0.5F)},
                                                              {"P95", PerfUtils::GetPercentile(fps_samples_, 0.05F)},
                                                              {"P99", PerfUtils::GetPercentile(fps_samples_, 0.01F)}}};
  char buf[kTextBufSize];
  for (std::size_t i = 0; i < entries.size(); ++i) {
    std::snprintf(buf, sizeof(buf), "%.1f", static_cast<double>(entries[i].second));
    if (!UpdateRow(kSlotFpsBase + i, font, sdlRenderer, entries[i].first, buf)) return false;
    active[count++] = {kSlotFpsBase + i, kSectionFps, FpsColor(entries[i].second)};
  }
  return true;
}

bool PerfOverlaySystem::AppendFrameRows(TTF_Font* font, SDL_Renderer* sdlRenderer, const float frameMs,
                                        std::array<ActiveRow, kRowCount>& active, std::size_t& count) {
  // Percentile high: "P95" is the frame time 95% of frames stay under.
  const std::array<std::pair<const char*, float>, 3> entries{
      {{"FRAME", frameMs}, {"AVG", GetAverage(ms_samples_)}, {"P95", PerfUtils::GetPercentile(ms_samples_, 0.95F)}}};
  char buf[kTextBufSize];
  for (std::size_t i = 0; i < entries.size(); ++i) {
    std::snprintf(buf, sizeof(buf), "%.2f ms", static_cast<double>(entries[i].second));
    if (!UpdateRow(kSlotFrameBase + i, font, sdlRenderer, entries[i].first, buf)) return false;
    active[count++] = {kSlotFrameBase + i, kSectionFrame, FrameMsColor(entries[i].second)};
  }
  return true;
}

bool PerfOverlaySystem::AppendWorldRows(TTF_Font* font, SDL_Renderer* sdlRenderer, Registry& registry,
                                        const EngineOptions& options, const float deltaTime,
                                        std::array<ActiveRow, kRowCount>& active, std::size_t& count) {
  char buf[kTextBufSize];
  if (HasFlag(options.perfOverlayMetrics, PerfOverlayMetrics::Entities)) {
    std::snprintf(buf, sizeof(buf), "%llu", static_cast<unsigned long long>(registry.GetUserEntityCount()));
    if (!UpdateRow(kSlotEntities, font, sdlRenderer, "ENTITIES", buf)) return false;
    active[count++] = {kSlotEntities, kSectionWorld, kNeutral};
  }
  if (HasFlag(options.perfOverlayMetrics, PerfOverlayMetrics::Memory)) {
    memory_refresh_timer_ -= deltaTime;
    if (memory_refresh_timer_ <= 0.0F) {
      resident_bytes_ = SystemMemory::GetResidentBytes();
      memory_refresh_timer_ = kMemoryRefreshSeconds;
    }
    std::snprintf(buf, sizeof(buf), "%.1f MB", static_cast<double>(resident_bytes_) / kBytesPerMb);
    if (!UpdateRow(kSlotMemory, font, sdlRenderer, "MEMORY", buf)) return false;
    active[count++] = {kSlotMemory, kSectionWorld, kNeutral};
  }
  return true;
}

void PerfOverlaySystem::RecordSample(const float fps, const float frameMs) {
  // Paired ring buffers of recent samples: fill first, then overwrite oldest.
  if (fps_samples_.size() < kSampleBuffer) {
    fps_samples_.push_back(fps);
    ms_samples_.push_back(frameMs);
  } else {
    fps_samples_[sample_index_] = fps;
    ms_samples_[sample_index_] = frameMs;
    sample_index_ = (sample_index_ + 1) % kSampleBuffer;
  }
}

void PerfOverlaySystem::Draw(Registry& registry, SDL_Renderer* sdlRenderer, const float deltaTime) {
  if (sdlRenderer == nullptr) return;
  auto& assetManager = registry.Get<AssetManager>();
  TTF_Font* font = EnsureFont(assetManager);
  if (font == nullptr) return;

  const auto& options = registry.Get<GameConfig>().GetEngineOptions();
  const float fps = deltaTime > 0.0F ? 1.0F / deltaTime : 0.0F;
  const float frameMs = deltaTime * 1000.0F;
  RecordSample(fps, frameMs);

  // Gather the active rows in draw order. Inactive metrics keep their cached textures but aren't
  // rastered/drawn this frame. Bail (draw nothing) if any raster fails.
  std::array<ActiveRow, kRowCount> active{};
  std::size_t count = 0;
  if (HasFlag(options.perfOverlayMetrics, PerfOverlayMetrics::Fps) &&
      !AppendFpsRows(font, sdlRenderer, fps, active, count))
    return;
  if (HasFlag(options.perfOverlayMetrics, PerfOverlayMetrics::FrameTime) &&
      !AppendFrameRows(font, sdlRenderer, frameMs, active, count))
    return;
  if (!AppendWorldRows(font, sdlRenderer, registry, options, deltaTime, active, count)) return;
  if (count == 0) return;

  // Measure: the block is two columns (widest label + widest value), rows stacked with a hairline
  // between sections. Right-aligning the value textures gives exact column alignment regardless of
  // glyph widths.
  float labelW = 0.0F;
  float valueW = 0.0F;
  float blockH = 0.0F;
  for (std::size_t i = 0; i < count; ++i) {
    const Row& row = rows_[active[i].slot];
    labelW = std::max(labelW, row.label.width);
    valueW = std::max(valueW, row.value.width);
    blockH += std::max(row.label.height, row.value.height) + kLineGap;
    if (i > 0 && active[i].section != active[i - 1].section) blockH += kSepBlockH;
  }
  blockH -= kLineGap;  // no trailing gap after the last row
  const float blockW = labelW + kColGap + valueW;

  // Anchor the block to the configured corner of the current render target (the scene texture).
  const Vec2 origin = BlockOrigin(options.perfOverlayCorner, sdlRenderer, blockW, blockH);

  // Translucent background panel + subtle border behind the text for legibility over any scene.
  SDL_SetRenderDrawBlendMode(sdlRenderer, SDL_BLENDMODE_BLEND);
  const SDL_FRect bg{origin.x - kPadding, origin.y - kPadding, blockW + (2.0F * kPadding), blockH + (2.0F * kPadding)};
  SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, kBgAlpha);
  SDL_RenderFillRect(sdlRenderer, &bg);
  SDL_SetRenderDrawColor(sdlRenderer, kWhite, kWhite, kWhite, kBorderAlpha);
  SDL_RenderRect(sdlRenderer, &bg);

  float y = origin.y;
  for (std::size_t i = 0; i < count; ++i) {
    if (i > 0 && active[i].section != active[i - 1].section) {
      const SDL_FRect sep{origin.x, y + kSepPad, blockW, 1.0F};
      SDL_SetRenderDrawColor(sdlRenderer, kWhite, kWhite, kWhite, kSepAlpha);
      SDL_RenderFillRect(sdlRenderer, &sep);
      y += kSepBlockH;
    }
    Row& row = rows_[active[i].slot];
    SDL_SetTextureColorMod(row.label.texture, kLabelTint.r, kLabelTint.g, kLabelTint.b);
    const SDL_FRect labelDst{origin.x, y, row.label.width, row.label.height};
    SDL_RenderTexture(sdlRenderer, row.label.texture, nullptr, &labelDst);

    SDL_SetTextureColorMod(row.value.texture, active[i].color.r, active[i].color.g, active[i].color.b);
    const SDL_FRect valueDst{origin.x + blockW - row.value.width, y, row.value.width, row.value.height};
    SDL_RenderTexture(sdlRenderer, row.value.texture, nullptr, &valueDst);

    y += std::max(row.label.height, row.value.height) + kLineGap;
  }
}
