#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <array>
#include <string>
#include <vector>

class Registry;
class AssetManager;

// Draws a lightweight FPS + frame-time overlay through the engine's own SDL renderer (no ImGui), so
// packaged/shipped builds (ImGui compiled out) can still surface basic perf metrics on screen. Owned
// by FrameLoop; keeps a tiny per-line text-texture cache across frames (re-rasterized only when the
// rendered string changes) and frees those textures on destruction. Each metric is color-coded by
// threshold via SDL_SetTextureColorMod at draw time, so a color change never forces a re-raster.
class PerfOverlaySystem
{
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

private:
    struct Line
    {
        std::string text;
        SDL_Texture* texture = nullptr;
        float width = 0.0F;
        float height = 0.0F;
    };

    // Rasterize `text` (white) into line.texture when line.text differs, updating its size. Returns
    // false on failure. The tint is applied separately at draw time.
    bool UpdateLine(Line& line, TTF_Font* font, SDL_Renderer* sdlRenderer, const std::string& text) const;

    // Resolve the embedded debug font, registering it into the asset manager on first use. Returns
    // nullptr (latched, no retry) if registration fails.
    TTF_Font* EnsureFont(AssetManager& assetManager);

    static constexpr int kLineCount = 5;
    std::array<Line, kLineCount> lines_{};
    // The embedded debug font is registered lazily on first Draw. Latched so a registration failure
    // isn't retried (and re-logged) every frame.
    bool font_init_attempted_ = false;

    // FPS buffer to track FPS trends.
    static constexpr int kFpsBuffer = 1000;
    std::vector<float> fps_metrics_;
    int fps_metric_index_ = 0;
};
