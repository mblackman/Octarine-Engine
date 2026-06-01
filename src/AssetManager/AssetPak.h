#pragma once

#include <SDL3/SDL_iostream.h>

#include <cstdint>
#include <map>
#include <string>
#include <vector>

class AssetCatalog;

// One-file archive of every baked asset, opened as a single SDL_LoadFile blob at startup. Each
// entry is referenced by its project-relative path (forward slashes), matching the `file` field
// AssetCatalog::WriteManifest serializes — so a lookup driven from `fullPath - basePath` resolves
// cleanly to the same key the manifest uses.
//
// Format ("OCPK" v1, little-endian):
//   [16] magic="OCPK" | u32 version=1 | u64 toc_off | u32 toc_count | u32 reserved
//   [data...]   raw blob bytes concatenated in TOC order
//   [TOC...]    repeated `toc_count` times:
//                 u64 offset (from file start)
//                 u64 size
//                 u16 path_len
//                 char path[path_len]   (no trailing NUL)
//
// Whole-file load: the pak gets read into memory once via SDL_LoadFile and each Open() returns an
// SDL_IOFromConstMem slice over the cached buffer. Memory cost ≈ total baked asset bytes — fine
// for indie shipping budgets; if a project ever needs lazy reads, swap the impl to a custom
// SDL_IOStreamInterface backed by pread() and the lookup map stays unchanged.
class AssetPak {
 public:
  AssetPak() = default;
  AssetPak(const AssetPak&) = delete;
  AssetPak& operator=(const AssetPak&) = delete;
  AssetPak(AssetPak&&) = default;
  AssetPak& operator=(AssetPak&&) = default;
  ~AssetPak();

  // Pack every entry in `catalog` into a single .pak at `outPath`. Each entry's `fullPath` is
  // read and stored under its project-relative form (`fullPath` re-rooted against `basePath`,
  // forward-slashed). `extraFiles` lets the bake step include derived artifacts (glyph atlases,
  // audio bakes, etc.) that don't live in the asset catalog — each is read + stored under its
  // own project-relative form. `pathOverrides` re-routes a catalog entry's *source* bytes to an
  // alternative path while keeping the original relPath in the pak — used by the audio
  // normalize pass to swap normalized WAVs in for their originals at packing time without
  // mutating the source tree. Duplicates against catalog entries are silently dropped (first
  // write wins). Returns false on any I/O failure; partially-written paks are removed.
  static bool Pack(const AssetCatalog& catalog, const std::string& outPath, const std::string& basePath,
                   const std::vector<std::string>& extraFiles = {},
                   const std::map<std::string, std::string>& pathOverrides = {});

  // Open a pak file from disk into memory. Returns true on success. The instance owns the
  // backing buffer until destruction.
  bool Open(const std::string& pakPath);

  // True when Open() succeeded.
  [[nodiscard]] bool IsOpen() const { return data_ != nullptr; }

  // True when `relPath` (project-relative, forward slashes) is a stored entry.
  [[nodiscard]] bool Contains(const std::string& relPath) const;

  // Open a read-only SDL_IOStream over the pak entry at `relPath`. Returns nullptr if the
  // entry is absent — caller falls back to SDL_IOFromFile. The returned stream is backed by
  // the pak's owned buffer; the caller closes it as usual.
  [[nodiscard]] SDL_IOStream* OpenIO(const std::string& relPath) const;

  [[nodiscard]] std::size_t Size() const { return entries_.size(); }

 private:
  struct Entry {
    std::uint64_t offset;
    std::uint64_t size;
  };

  void* data_{nullptr};
  std::size_t data_size_{0};
  std::map<std::string, Entry> entries_;
};
