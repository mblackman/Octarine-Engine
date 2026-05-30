#include "AssetManager/AtlasBaker.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_rect_pack.h"

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <map>
#include <vector>

#include "AssetManager/AssetCatalog.h"
#include "General/Logger.h"

namespace
{
    namespace fs = std::filesystem;

    struct LoadedSource
    {
        std::string memberId;
        std::vector<unsigned char> rgba;
        int w{0};
        int h{0};
    };

    std::string AtlasCatalogIdFor(const std::string& groupName) { return "__atlas_" + groupName; }

    // Pick the smallest square canvas, rounded up to a power of two and capped at the engine's
    // SDL_GPU-safe ceiling, into which every member of the group could in principle fit. This
    // is a starting point for stb_rect_pack — if packing fails at this size we keep doubling.
    int InitialCanvasDim(const std::vector<LoadedSource>& sources, int paddingPx, int maxDim)
    {
        long long totalArea = 0;
        int widest = 0;
        int tallest = 0;
        for (const auto& s : sources)
        {
            const int padW = s.w + paddingPx;
            const int padH = s.h + paddingPx;
            totalArea += static_cast<long long>(padW) * padH;
            widest = std::max(widest, s.w);
            tallest = std::max(tallest, s.h);
        }
        int dim = 64;
        while (static_cast<long long>(dim) * dim < totalArea || dim < widest || dim < tallest)
        {
            dim *= 2;
            if (dim > maxDim) return dim; // surface the failure to the caller
        }
        return dim;
    }
} // namespace

bool AtlasBaker::Run(AssetCatalog& catalog, const std::string& basePath) const
{
    // Group eligible textures by atlas name. Iteration order over std::map is id-sorted, so the
    // resulting groups (and the packing rect order inside each) are deterministic across runs.
    std::map<std::string, std::vector<std::string>> groups;
    for (const auto& [id, entry] : catalog.Entries())
    {
        if (entry.type != AssetType::Texture) continue;
        if (entry.noAtlas) continue;
        if (!entry.atlas.has_value() || entry.atlas->empty()) continue;
        groups[*entry.atlas].push_back(id);
    }
    if (groups.empty()) return true;

    const fs::path atlasOutDir = fs::path(basePath) / "_atlases";
    std::error_code ec;
    fs::create_directories(atlasOutDir, ec);
    if (ec)
    {
        Logger::Error("AtlasBaker: failed to create " + atlasOutDir.string() + ": " + ec.message());
        return false;
    }

    for (const auto& [groupName, memberIds] : groups)
    {
        // Load every member's pixels up front so the same vector drives the pack + the canvas
        // blit + the catalog mutation. Members that fail to load fail the entire group — the
        // bake is the right place to surface a bad PNG.
        std::vector<LoadedSource> sources;
        sources.reserve(memberIds.size());
        for (const auto& mid : memberIds)
        {
            const CatalogEntry* member = catalog.Find(mid);
            if (member == nullptr) continue;

            int w = 0;
            int h = 0;
            int comp = 0;
            unsigned char* data = stbi_load(member->fullPath.c_str(), &w, &h, &comp, 4);
            if (data == nullptr)
            {
                Logger::Error("AtlasBaker: failed to load source for atlas member '" + mid + "' (" +
                              member->fullPath + "): " + std::string(stbi_failure_reason() != nullptr ? stbi_failure_reason() : ""));
                return false;
            }
            LoadedSource src;
            src.memberId = mid;
            src.w = w;
            src.h = h;
            src.rgba.assign(data, data + static_cast<size_t>(w) * h * 4);
            stbi_image_free(data);
            sources.push_back(std::move(src));
        }
        if (sources.empty()) continue;

        // Pack. Start at the smallest power-of-two square that *could* hold the total area + the
        // widest/tallest sprite, then double until every sprite is placed or the cap is hit.
        const int padding = kDefaultPaddingPx;
        int dim = InitialCanvasDim(sources, padding, kDefaultMaxAtlasDim);

        std::vector<stbrp_rect> rects(sources.size());
        for (size_t i = 0; i < sources.size(); ++i)
        {
            rects[i].id = static_cast<int>(i);
            rects[i].w = sources[i].w + padding;
            rects[i].h = sources[i].h + padding;
        }

        bool packed = false;
        while (dim <= kDefaultMaxAtlasDim)
        {
            std::vector<stbrp_node> nodes(static_cast<size_t>(dim));
            stbrp_context ctx{};
            stbrp_init_target(&ctx, dim, dim, nodes.data(), static_cast<int>(nodes.size()));
            const int allPacked = stbrp_pack_rects(&ctx, rects.data(), static_cast<int>(rects.size()));
            if (allPacked != 0)
            {
                packed = true;
                break;
            }
            dim *= 2;
        }
        if (!packed)
        {
            Logger::Error("AtlasBaker: atlas '" + groupName + "' (" + std::to_string(sources.size()) +
                          " members) cannot fit inside " + std::to_string(kDefaultMaxAtlasDim) +
                          "px square. Split the group or shrink the source art.");
            return false;
        }

        // Allocate the canvas as transparent RGBA. Padding stays transparent, which is what gives
        // bilinear sampling something safe to interpolate against at slice edges.
        std::vector<unsigned char> canvas(static_cast<size_t>(dim) * dim * 4, 0);
        for (const auto& r : rects)
        {
            const LoadedSource& src = sources[r.id];
            for (int row = 0; row < src.h; ++row)
            {
                const size_t srcOffset = static_cast<size_t>(row) * src.w * 4;
                const size_t dstOffset =
                        (static_cast<size_t>(r.y + row) * dim + static_cast<size_t>(r.x)) * 4;
                std::memcpy(canvas.data() + dstOffset, src.rgba.data() + srcOffset,
                            static_cast<size_t>(src.w) * 4);
            }
        }

        const fs::path atlasOut = atlasOutDir / (groupName + ".png");
        if (stbi_write_png(atlasOut.string().c_str(), dim, dim, 4, canvas.data(), dim * 4) == 0)
        {
            Logger::Error("AtlasBaker: failed to write atlas PNG " + atlasOut.string());
            return false;
        }

        const std::string atlasId = AtlasCatalogIdFor(groupName);
        const std::string atlasFullPath = fs::absolute(atlasOut, ec).lexically_normal().string();

        // Mutate every member entry so the manifest carries the slice info, and AssetManager can
        // redirect texture lookups + the render system can offset src_rect.
        for (const auto& r : rects)
        {
            const std::string& mid = sources[r.id].memberId;
            SDL_FRect slice{};
            slice.x = static_cast<float>(r.x);
            slice.y = static_cast<float>(r.y);
            slice.w = static_cast<float>(sources[r.id].w);
            slice.h = static_cast<float>(sources[r.id].h);
            catalog.SetAtlasMembership(mid, atlasId, slice);
        }

        // Add the atlas as a first-class catalog entry. Type=Texture, no scale_mode preference
        // (the runtime keeps the project default — atlas members typically share scale settings).
        CatalogEntry atlasEntry;
        atlasEntry.type = AssetType::Texture;
        atlasEntry.id = atlasId;
        atlasEntry.fullPath = atlasFullPath;
        catalog.InsertOrReplace(std::move(atlasEntry));

        Logger::Info("AtlasBaker: packed atlas '" + groupName + "' (" + std::to_string(sources.size()) + " members, " +
                     std::to_string(dim) + "x" + std::to_string(dim) + ") -> " + atlasOut.string());
    }

    return true;
}
