#pragma once

#include <string>

// Shared helpers for the sandboxed Lua file-write surface (storage.* save data, editor-only
// project.*). Path containment mirrors AssetManager::GetFullPath's lexical traversal check, but
// is stricter: absolute inputs are rejected outright since callers only ever hand user/script
// paths, never catalog-resolved ones.
namespace file_sandbox {

// Resolve `relative` against `root` and return the joined full path. Returns an empty string
// when `root` is empty, `relative` is absolute, or the normalized result escapes `root`
// (`../` traversal). Purely lexical — does not consult the filesystem, so symlinks inside the
// root are trusted (the roots are app-owned directories).
std::string Resolve(const std::string& root, const std::string& relative);

// Write `contents` to `fullPath` atomically: write a sibling `.tmp` file, then rename it over
// the target so a crash mid-write never leaves a truncated file (matters for save data).
// Creates missing parent directories. Returns false on any failure, leaving no `.tmp` behind.
bool WriteFileAtomic(const std::string& fullPath, const std::string& contents);

}  // namespace file_sandbox
