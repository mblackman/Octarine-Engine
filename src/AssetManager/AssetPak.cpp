#include "AssetManager/AssetPak.h"

#include <SDL3/SDL.h>

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <vector>

#include "AssetManager/AssetCatalog.h"
#include "General/Logger.h"

namespace {
constexpr char kMagic[4] = {'O', 'C', 'P', 'K'};
constexpr std::uint32_t kVersion = 1;

// Header is fixed-size; declared inline to keep the wire format adjacent to the I/O routines that
// produce + consume it. `reserved` is padding to a clean 32-byte header (8B magic+ver, 8B toc_off,
// 4B toc_count, 4B reserved).
#pragma pack(push, 1)
struct PakHeader {
  char magic[4];
  std::uint32_t version;
  std::uint64_t tocOffset;
  std::uint32_t tocCount;
  std::uint32_t reserved;
};
#pragma pack(pop)
static_assert(sizeof(PakHeader) == 24, "PakHeader must be exactly 24 bytes on disk");

// Re-root `fullPath` against `basePath` and normalize to forward slashes. Mirrors the path form
// AssetCatalog::WriteManifest emits, so the pak key + manifest `file` field are interchangeable.
std::string MakeRelPath(const std::string& fullPath, const std::string& basePath) {
  namespace fs = std::filesystem;
  std::error_code ec;
  fs::path rel = fs::relative(fs::path(fullPath), fs::path(basePath), ec);
  if (ec || rel.empty()) {
    rel = fs::path(fullPath);
  }
  std::string s = rel.generic_string();
  // Defensive: relative() can yield `..` on a non-prefixed input. Keep as-is — pack/unpack still
  // round-trip, the bake just lands a non-portable key (which the runtime can still look up).
  return s;
}
}  // namespace

AssetPak::~AssetPak() {
  if (data_ != nullptr) {
    SDL_free(data_);
    data_ = nullptr;
  }
}

bool AssetPak::Pack(const AssetCatalog& catalog, const std::string& outPath, const std::string& basePath,
                    const std::vector<std::string>& extraFiles,
                    const std::map<std::string, std::string>& pathOverrides) {
  std::ofstream out(outPath, std::ios::binary | std::ios::trunc);
  if (!out) {
    Logger::Error("AssetPak::Pack: failed to open " + outPath + " for writing");
    return false;
  }

  // Reserve space for the header — patched in once tocOffset is known.
  PakHeader header{};
  std::memcpy(header.magic, kMagic, 4);
  header.version = kVersion;
  out.write(reinterpret_cast<const char*>(&header), sizeof(header));

  struct StagedEntry {
    std::string relPath;
    std::uint64_t offset;
    std::uint64_t size;
  };
  std::vector<StagedEntry> staged;
  staged.reserve(catalog.Size());

  // Stream each asset's bytes into the output. Buffer per-entry rather than slurping every asset
  // into RAM — a project with a few hundred MB of assets shouldn't need that much during bake.
  constexpr std::size_t kCopyChunk = 64 * 1024;
  std::vector<char> buf(kCopyChunk);

  for (const auto& [id, entry] : catalog.Entries()) {
    const std::string relPath = MakeRelPath(entry.fullPath, basePath);
    // pathOverrides[relPath] swaps the source-of-bytes; the pak still stores under `relPath`,
    // so the runtime lookup is unchanged. Audio normalize uses this to ship the normalized
    // WAV without rewriting the asset tree.
    std::string sourcePath = entry.fullPath;
    if (auto it = pathOverrides.find(relPath); it != pathOverrides.end()) {
      sourcePath = it->second;
    }
    std::ifstream in(sourcePath, std::ios::binary);
    if (!in) {
      Logger::Error("AssetPak::Pack: failed to read " + sourcePath);
      out.close();
      std::error_code ec;
      std::filesystem::remove(outPath, ec);
      return false;
    }
    const std::uint64_t offset = static_cast<std::uint64_t>(out.tellp());
    std::uint64_t size = 0;
    while (in) {
      in.read(buf.data(), static_cast<std::streamsize>(buf.size()));
      const std::streamsize got = in.gcount();
      if (got > 0) {
        out.write(buf.data(), got);
        size += static_cast<std::uint64_t>(got);
      }
    }
    staged.push_back({relPath, offset, size});
  }

  // Append any extra (non-catalog) files in the same shape. Bake passes derived artifacts —
  // glyph atlases, audio bakes — here so they ride in the same pak.
  for (const std::string& extraPath : extraFiles) {
    const std::string rel = MakeRelPath(extraPath, basePath);
    const bool dup = std::any_of(staged.begin(), staged.end(), [&](const StagedEntry& s) { return s.relPath == rel; });
    if (dup) continue;
    std::ifstream in(extraPath, std::ios::binary);
    if (!in) {
      Logger::Error("AssetPak::Pack: failed to read extra " + extraPath);
      out.close();
      std::error_code ec;
      std::filesystem::remove(outPath, ec);
      return false;
    }
    const std::uint64_t offset = static_cast<std::uint64_t>(out.tellp());
    std::uint64_t size = 0;
    while (in) {
      in.read(buf.data(), static_cast<std::streamsize>(buf.size()));
      const std::streamsize got = in.gcount();
      if (got > 0) {
        out.write(buf.data(), got);
        size += static_cast<std::uint64_t>(got);
      }
    }
    staged.push_back({rel, offset, size});
  }

  // TOC.
  const std::uint64_t tocOffset = static_cast<std::uint64_t>(out.tellp());
  for (const auto& s : staged) {
    out.write(reinterpret_cast<const char*>(&s.offset), sizeof(s.offset));
    out.write(reinterpret_cast<const char*>(&s.size), sizeof(s.size));
    const std::uint16_t pathLen = static_cast<std::uint16_t>(s.relPath.size());
    out.write(reinterpret_cast<const char*>(&pathLen), sizeof(pathLen));
    out.write(s.relPath.data(), pathLen);
  }

  // Patch header w/ TOC info.
  header.tocOffset = tocOffset;
  header.tocCount = static_cast<std::uint32_t>(staged.size());
  out.seekp(0, std::ios::beg);
  out.write(reinterpret_cast<const char*>(&header), sizeof(header));
  out.close();

  if (!out) {
    Logger::Error("AssetPak::Pack: write error finalizing " + outPath);
    std::error_code ec;
    std::filesystem::remove(outPath, ec);
    return false;
  }
  Logger::Info("AssetPak::Pack: wrote " + std::to_string(staged.size()) + " entries to " + outPath);
  return true;
}

bool AssetPak::Open(const std::string& pakPath) {
  if (data_ != nullptr) {
    SDL_free(data_);
    data_ = nullptr;
    data_size_ = 0;
    entries_.clear();
  }

  // SDL_LoadFile resolves through SDL_IOFromFile, so this works inside an Android APK + a
  // desktop loose dir without callers needing to know which they're on.
  std::size_t sz = 0;
  void* buf = SDL_LoadFile(pakPath.c_str(), &sz);
  if (buf == nullptr) {
    Logger::Warn("AssetPak::Open: SDL_LoadFile failed for " + pakPath + ": " + std::string(SDL_GetError()));
    return false;
  }
  if (sz < sizeof(PakHeader)) {
    Logger::Error("AssetPak::Open: " + pakPath + " is shorter than the pak header");
    SDL_free(buf);
    return false;
  }
  PakHeader header{};
  std::memcpy(&header, buf, sizeof(header));
  if (std::memcmp(header.magic, kMagic, 4) != 0) {
    Logger::Error("AssetPak::Open: " + pakPath + " has wrong magic (not an OCPK file)");
    SDL_free(buf);
    return false;
  }
  if (header.version != kVersion) {
    Logger::Error("AssetPak::Open: " + pakPath + " is OCPK v" + std::to_string(header.version) + "; engine expects v" +
                  std::to_string(kVersion));
    SDL_free(buf);
    return false;
  }
  if (header.tocOffset > sz) {
    Logger::Error("AssetPak::Open: " + pakPath + " TOC offset past end of file");
    SDL_free(buf);
    return false;
  }

  const auto* tocCursor = static_cast<const std::uint8_t*>(buf) + header.tocOffset;
  const auto* tocEnd = static_cast<const std::uint8_t*>(buf) + sz;
  entries_.clear();
  for (std::uint32_t i = 0; i < header.tocCount; ++i) {
    if (tocCursor + sizeof(std::uint64_t) * 2 + sizeof(std::uint16_t) > tocEnd) {
      Logger::Error("AssetPak::Open: truncated TOC in " + pakPath);
      SDL_free(buf);
      entries_.clear();
      return false;
    }
    Entry e{};
    std::memcpy(&e.offset, tocCursor, sizeof(e.offset));
    tocCursor += sizeof(e.offset);
    std::memcpy(&e.size, tocCursor, sizeof(e.size));
    tocCursor += sizeof(e.size);
    std::uint16_t pathLen = 0;
    std::memcpy(&pathLen, tocCursor, sizeof(pathLen));
    tocCursor += sizeof(pathLen);
    if (tocCursor + pathLen > tocEnd) {
      Logger::Error("AssetPak::Open: TOC path overruns file in " + pakPath);
      SDL_free(buf);
      entries_.clear();
      return false;
    }
    std::string relPath(reinterpret_cast<const char*>(tocCursor), pathLen);
    tocCursor += pathLen;
    if (e.size > header.tocOffset || e.offset > header.tocOffset - e.size) {
      Logger::Error("AssetPak::Open: entry '" + relPath + "' overlaps TOC region");
      SDL_free(buf);
      entries_.clear();
      return false;
    }
    entries_.emplace(std::move(relPath), e);
  }

  data_ = buf;
  data_size_ = sz;
  Logger::Info("AssetPak::Open: loaded " + std::to_string(entries_.size()) + " entries from " + pakPath);
  return true;
}

bool AssetPak::Contains(const std::string& relPath) const { return entries_.find(relPath) != entries_.end(); }

SDL_IOStream* AssetPak::OpenIO(const std::string& relPath) const {
  if (data_ == nullptr) {
    return nullptr;
  }
  const auto it = entries_.find(relPath);
  if (it == entries_.end()) {
    return nullptr;
  }
  const auto* slice = static_cast<const std::uint8_t*>(data_) + it->second.offset;
  return SDL_IOFromConstMem(slice, it->second.size);
}
