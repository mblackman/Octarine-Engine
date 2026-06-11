#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

class Registry;
class AssetManager;
struct EngineOptions;

// Draws a lightweight perf overlay (FPS, frame time, world stats) through the engine's own SDL
// renderer (no ImGui), so packaged/shipped builds (ImGui compiled out) can still surface basic perf
// metrics on screen. Owned by FrameLoop; keeps a tiny per-row text-texture cache across frames
// (re-rasterized only when the rendered string changes) and frees those textures on destruction.
// Rows are laid out as two columns — dim label on the left, threshold-colored value right-aligned —
// grouped into sections separated by hairlines. Colors are applied via SDL_SetTextureColorMod at
// draw time, so a color change never forces a re-raster.
class PerfOverlaySystem {
 public:
  PerfOverlaySystem();
  PerfOverlaySystem(const PerfOverlaySystem&) = delete;
  PerfOverlaySystem& operator=(const PerfOverlaySystem&) = delete;
  PerfOverlaySystem(PerfOverlaySystem&&) = delete;
  PerfOverlaySystem& operator=(PerfOverlaySystem&&) = delete;
  ~PerfOverlaySystem();

  // Draw the overlay into the currently-bound render target (the scene texture, before EndScene).
  // No-op if the renderer is null or the embedded debug font can't be registered/loaded.
  void Draw(Registry& registry, SDL_Renderer* sdlRenderer, float deltaTime);

  struct Rgb {
    Uint8 r;
    Uint8 g;
    Uint8 b;
  };

 private:
  struct Line {
    std::string text;
    SDL_Texture* texture = nullptr;
    float width = 0.0F;
    float height = 0.0F;
  };

  // One overlay row: cached label texture (rasterized once) + cached value texture (re-rasterized
  // when the formatted value string changes).
  struct Row {
    Line label;
    Line value;
  };

  // A row selected for drawing this frame: which slot, which section (for separators), and the
  // tint applied to its value texture.
  struct ActiveRow {
    std::size_t slot;
    std::uint8_t section;
    Rgb color;
  };

  // Fixed row slots in lines-cache order. FPS section first, then frame-time, then world stats.
  static constexpr std::size_t kSlotFpsBase = 0;    // FPS / AVG / P50 / P95 / P99
  static constexpr std::size_t kSlotFrameBase = 5;  // FRAME / AVG / P95 (ms)
  static constexpr std::size_t kSlotEntities = 8;
  static constexpr std::size_t kSlotMemory = 9;
  static constexpr std::size_t kRowCount = 10;

  static constexpr std::uint8_t kSectionFps = 0;
  static constexpr std::uint8_t kSectionFrame = 1;
  static constexpr std::uint8_t kSectionWorld = 2;

  // Rasterize `text` (white) into line.texture when line.text differs, updating its size. Returns
  // false on failure. The tint is applied separately at draw time.
  bool UpdateLine(Line& line, TTF_Font* font, SDL_Renderer* sdlRenderer, const std::string& text) const;

  // Update both columns of a row slot. Returns false if either raster fails.
  bool UpdateRow(std::size_t slot, TTF_Font* font, SDL_Renderer* sdlRenderer, const char* label, const char* value);

  // Resolve the embedded debug font, registering it into the asset manager on first use. Returns
  // nullptr (latched, no retry) if registration fails.
  TTF_Font* EnsureFont(AssetManager& assetManager);

  // Rasterize + activate the FPS section rows (current/avg/p50/p95/p99). Returns false if any
  // raster fails.
  bool AppendFpsRows(TTF_Font* font, SDL_Renderer* sdlRenderer, float fps, std::array<ActiveRow, kRowCount>& active,
                     std::size_t& count);

  // Rasterize + activate the frame-time section rows (current/avg/p95 ms). Returns false if any
  // raster fails.
  bool AppendFrameRows(TTF_Font* font, SDL_Renderer* sdlRenderer, float frameMs,
                       std::array<ActiveRow, kRowCount>& active, std::size_t& count);

  // Rasterize + activate the world-stats section rows (entity count, resident memory) for the
  // metrics enabled in `options`. Returns false if any raster fails.
  bool AppendWorldRows(TTF_Font* font, SDL_Renderer* sdlRenderer, Registry& registry, const EngineOptions& options,
                       float deltaTime, std::array<ActiveRow, kRowCount>& active, std::size_t& count);

  // Push paired FPS / frame-ms samples into the fixed-size ring buffers backing the avg/percentile
  // readouts.
  void RecordSample(float fps, float frameMs);

  std::array<Row, kRowCount> rows_{};
  // The embedded debug font is registered lazily on first Draw. Latched so a registration failure
  // isn't retried (and re-logged) every frame.
  bool font_init_attempted_ = false;

  // Ring buffers of recent per-frame samples backing the avg/percentile readouts. Both share
  // sample_index_ — they are pushed together each frame.
  static constexpr std::size_t kSampleBuffer = 1000;
  std::vector<float> fps_samples_;
  std::vector<float> ms_samples_;
  std::size_t sample_index_ = 0;

  // Resident-memory readout is a syscall, so it's polled on a timer instead of every frame. The
  // timer starts expired so the first frame shows a real value.
  std::uint64_t resident_bytes_ = 0;
  float memory_refresh_timer_ = 0.0F;
};
