#include "General/FileWatcher.h"

#ifndef OCTARINE_SHIPPED

#include <system_error>

#include "General/Logger.h"

namespace fs = std::filesystem;

std::string FileWatcher::Canonical(const std::string& absPath) {
  std::error_code ec;
  // weakly_canonical so partial-existent paths still resolve (caller may register before the
  // file exists in some edge cases). On error, fall back to the input.
  auto canon = fs::weakly_canonical(fs::path(absPath), ec);
  if (ec) {
    return absPath;
  }
  return canon.string();
}

void FileWatcher::Track(const std::string& absPath) {
  const std::string key = Canonical(absPath);
  if (mtimes_.find(key) != mtimes_.end()) {
    return;
  }

  std::error_code ec;
  const auto mtime = fs::last_write_time(key, ec);
  if (ec) {
    Logger::Warn("FileWatcher: cannot stat '" + key + "': " + ec.message());
    return;
  }
  mtimes_.emplace(key, mtime);
}

void FileWatcher::Untrack(const std::string& absPath) {
  const std::string key = Canonical(absPath);
  mtimes_.erase(key);
}

std::vector<std::string> FileWatcher::Poll() {
  std::vector<std::string> dirty;
  for (auto& [path, lastMtime] : mtimes_) {
    std::error_code ec;
    const auto now = fs::last_write_time(path, ec);
    if (ec) {
      // File temporarily missing (atomic-save rename window). Skip this round, try again next
      // poll without updating the cached mtime so we'll catch the post-rename state.
      continue;
    }
    if (now != lastMtime) {
      lastMtime = now;
      dirty.push_back(path);
    }
  }
  return dirty;
}

#endif  // OCTARINE_SHIPPED
