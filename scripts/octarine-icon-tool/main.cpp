// octarine-icon-tool — tiny host CLI used by scripts/octarine-icons.cmake to resize
// the project's source icon and emit per-platform launcher images. Replaces a runtime
// dependency on ImageMagick (which CMake couldn't pull through vcpkg, so contributors
// + bare CI runners silently fell back to the SDL template icon).
//
// Build via scripts/octarine-icon-tool/CMakeLists.txt; nothing here ships in the
// engine artifacts.
//
// Commands:
//   octarine-icon-tool resize INPUT OUTPUT SIZE
//       Resize INPUT (any stb-readable format) to an SIZE x SIZE PNG. Aspect ratio
//       preserved by fitting inside the canvas and centering with transparent
//       padding — matches the prior `magick -gravity center -extent NxN` behavior.
//
//   octarine-icon-tool ico INPUT OUTPUT SIZE[,SIZE,...]
//       Emit a Windows .ico bundling each requested size as a PNG entry (the modern
//       PNG-in-ICO format that Vista+ accepts). Sizes are encoded little-endian per
//       the ICONDIR/ICONDIRENTRY layout; 256 is encoded as 0 per spec.
//
// Exit codes: 0 success, 1 runtime failure, 2 bad usage.

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_image_resize2.h"

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace {

struct LoadedImage {
    std::vector<unsigned char> rgba;
    int w = 0;
    int h = 0;
};

bool loadImage(const char* path, LoadedImage& out) {
    int w = 0;
    int h = 0;
    int comp = 0;
    unsigned char* data = stbi_load(path, &w, &h, &comp, 4);
    if (data == nullptr) {
        std::fprintf(stderr, "octarine-icon-tool: failed to load `%s`: %s\n",
                     path, stbi_failure_reason());
        return false;
    }
    out.w = w;
    out.h = h;
    out.rgba.assign(data, data + static_cast<size_t>(w) * static_cast<size_t>(h) * 4);
    stbi_image_free(data);
    return true;
}

// Resize preserving aspect ratio, centered on an NxN transparent RGBA canvas.
bool resizeToSquare(const LoadedImage& src, int n, std::vector<unsigned char>& out) {
    if (n <= 0 || src.w <= 0 || src.h <= 0) {
        std::fprintf(stderr, "octarine-icon-tool: bad dimensions (src=%dx%d, target=%d)\n",
                     src.w, src.h, n);
        return false;
    }
    const double scale = static_cast<double>(n) / std::max(src.w, src.h);
    const int fw = std::max(1, static_cast<int>(src.w * scale + 0.5));
    const int fh = std::max(1, static_cast<int>(src.h * scale + 0.5));

    std::vector<unsigned char> fit(static_cast<size_t>(fw) * static_cast<size_t>(fh) * 4);
    auto* ok = stbir_resize_uint8_srgb(
            src.rgba.data(), src.w, src.h, 0,
            fit.data(), fw, fh, 0,
            STBIR_RGBA);
    if (ok == nullptr) {
        std::fprintf(stderr, "octarine-icon-tool: stbir_resize failed\n");
        return false;
    }

    out.assign(static_cast<size_t>(n) * static_cast<size_t>(n) * 4, 0);
    const int x0 = (n - fw) / 2;
    const int y0 = (n - fh) / 2;
    for (int y = 0; y < fh; ++y) {
        std::memcpy(
                &out[(static_cast<size_t>(y0 + y) * n + x0) * 4],
                &fit[static_cast<size_t>(y) * fw * 4],
                static_cast<size_t>(fw) * 4);
    }
    return true;
}

bool writePngToFile(const char* path, int n, const std::vector<unsigned char>& rgba) {
    if (stbi_write_png(path, n, n, 4, rgba.data(), n * 4) == 0) {
        std::fprintf(stderr, "octarine-icon-tool: stbi_write_png failed for `%s`\n", path);
        return false;
    }
    return true;
}

struct MemBuffer {
    std::vector<unsigned char> data;
};

void memWriteCb(void* context, void* data, int size) {
    auto* buf = static_cast<MemBuffer*>(context);
    auto* bytes = static_cast<unsigned char*>(data);
    buf->data.insert(buf->data.end(), bytes, bytes + size);
}

bool writePngToMem(int n, const std::vector<unsigned char>& rgba, MemBuffer& out) {
    if (stbi_write_png_to_func(&memWriteCb, &out, n, n, 4, rgba.data(), n * 4) == 0) {
        std::fprintf(stderr, "octarine-icon-tool: stbi_write_png_to_func failed\n");
        return false;
    }
    return true;
}

void writeU16Le(std::FILE* f, uint16_t v) {
    unsigned char b[2] = {
            static_cast<unsigned char>(v & 0xFF),
            static_cast<unsigned char>((v >> 8) & 0xFF),
    };
    std::fwrite(b, 1, 2, f);
}

void writeU32Le(std::FILE* f, uint32_t v) {
    unsigned char b[4] = {
            static_cast<unsigned char>(v & 0xFF),
            static_cast<unsigned char>((v >> 8) & 0xFF),
            static_cast<unsigned char>((v >> 16) & 0xFF),
            static_cast<unsigned char>((v >> 24) & 0xFF),
    };
    std::fwrite(b, 1, 4, f);
}

bool writeIco(const char* path,
              const std::vector<MemBuffer>& imgs,
              const std::vector<int>& sizes) {
    std::FILE* f = std::fopen(path, "wb");
    if (f == nullptr) {
        std::fprintf(stderr, "octarine-icon-tool: failed to open `%s` for write\n", path);
        return false;
    }

    writeU16Le(f, 0);                                   // reserved
    writeU16Le(f, 1);                                   // type = ICO
    writeU16Le(f, static_cast<uint16_t>(imgs.size()));  // image count

    uint32_t dataOffset = 6 + 16u * static_cast<uint32_t>(imgs.size());
    for (size_t i = 0; i < imgs.size(); ++i) {
        const int s = sizes[i];
        // ICONDIRENTRY width/height: 0 encodes 256.
        const unsigned char dim = (s >= 256) ? 0 : static_cast<unsigned char>(s);
        std::fwrite(&dim, 1, 1, f);                                // width
        std::fwrite(&dim, 1, 1, f);                                // height
        const unsigned char zero = 0;
        std::fwrite(&zero, 1, 1, f);                               // color count (palette)
        std::fwrite(&zero, 1, 1, f);                               // reserved
        writeU16Le(f, 1);                                          // planes
        writeU16Le(f, 32);                                         // bpp
        writeU32Le(f, static_cast<uint32_t>(imgs[i].data.size())); // payload size
        writeU32Le(f, dataOffset);                                 // payload offset
        dataOffset += static_cast<uint32_t>(imgs[i].data.size());
    }
    for (const auto& m : imgs) {
        std::fwrite(m.data.data(), 1, m.data.size(), f);
    }
    std::fclose(f);
    return true;
}

int cmdResize(int argc, char** argv) {
    if (argc != 5) {
        std::fprintf(stderr, "usage: octarine-icon-tool resize INPUT OUTPUT SIZE\n");
        return 2;
    }
    LoadedImage src;
    if (!loadImage(argv[2], src)) {
        return 1;
    }
    const int n = std::atoi(argv[4]);
    if (n <= 0) {
        std::fprintf(stderr, "octarine-icon-tool: bad size `%s`\n", argv[4]);
        return 2;
    }
    std::vector<unsigned char> out;
    if (!resizeToSquare(src, n, out)) {
        return 1;
    }
    return writePngToFile(argv[3], n, out) ? 0 : 1;
}

int cmdIco(int argc, char** argv) {
    if (argc != 5) {
        std::fprintf(stderr, "usage: octarine-icon-tool ico INPUT OUTPUT SIZE[,SIZE,...]\n");
        return 2;
    }
    LoadedImage src;
    if (!loadImage(argv[2], src)) {
        return 1;
    }
    std::vector<int> sizes;
    const char* p = argv[4];
    while (*p != '\0') {
        char* end = nullptr;
        const long v = std::strtol(p, &end, 10);
        if (end == p || v <= 0 || v > 1024) {
            std::fprintf(stderr, "octarine-icon-tool: bad ico size near `%s`\n", p);
            return 2;
        }
        sizes.push_back(static_cast<int>(v));
        p = end;
        if (*p == ',') {
            ++p;
        }
    }
    if (sizes.empty()) {
        std::fprintf(stderr, "octarine-icon-tool: ico needs at least one size\n");
        return 2;
    }

    std::vector<MemBuffer> imgs(sizes.size());
    for (size_t i = 0; i < sizes.size(); ++i) {
        std::vector<unsigned char> rgba;
        if (!resizeToSquare(src, sizes[i], rgba)) {
            return 1;
        }
        if (!writePngToMem(sizes[i], rgba, imgs[i])) {
            return 1;
        }
    }
    return writeIco(argv[3], imgs, sizes) ? 0 : 1;
}

}  // namespace

int main(int argc, char** argv) {
    if (argc < 2) {
        std::fprintf(stderr, "usage: octarine-icon-tool {resize|ico} ...\n");
        return 2;
    }
    if (std::strcmp(argv[1], "resize") == 0) {
        return cmdResize(argc, argv);
    }
    if (std::strcmp(argv[1], "ico") == 0) {
        return cmdIco(argc, argv);
    }
    std::fprintf(stderr, "octarine-icon-tool: unknown command `%s`\n", argv[1]);
    return 2;
}
