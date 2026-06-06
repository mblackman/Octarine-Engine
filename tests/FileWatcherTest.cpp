// Unit checks for FileWatcher — the generic mtime-polling watcher shared by ScriptHotReload and
// AssetHotReload. Pure filesystem, no SDL / Lua / renderer, so it runs headless. gtest-free; exit
// code = failed-check count. Registered with ctest as FileWatcherTest.
//
// mtime granularity varies by filesystem, so we drive change detection by explicitly advancing
// last_write_time rather than relying on a real second-resolution clock between writes.

#include "General/FileWatcher.h"
#include "TestHarness.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>

using octarine::test::Check;
using octarine::test::CheckEq;
namespace fs = std::filesystem;

namespace {
std::string Canon(const fs::path& p) {
  std::error_code ec;
  auto c = fs::weakly_canonical(p, ec);
  return ec ? p.string() : c.string();
}

void Write(const fs::path& p, const std::string& contents) {
  std::ofstream out(p, std::ios::binary | std::ios::trunc);
  out << contents;
}

// Force the file's mtime forward so Poll() sees a change deterministically.
void BumpMtime(const fs::path& p) {
  std::error_code ec;
  auto t = fs::last_write_time(p, ec);
  fs::last_write_time(p, t + std::chrono::seconds(2), ec);
}
}  // namespace

int main() {
  const fs::path dir = fs::temp_directory_path() / "octarine-filewatcher-test";
  std::error_code ec;
  fs::remove_all(dir, ec);
  fs::create_directories(dir, ec);
  const fs::path file = dir / "watched.txt";
  Write(file, "v1");

  {
    FileWatcher fw;
    Check(fw.Empty(), "fresh watcher is Empty()");

    fw.Track(file.string());
    CheckEq(fw.TrackedCount(), static_cast<std::size_t>(1), "Track adds one entry");
    Check(!fw.Empty(), "watcher not Empty() after Track");

    CheckEq(fw.Poll().size(), static_cast<std::size_t>(0), "Poll with no change returns nothing");

    BumpMtime(file);
    const auto dirty = fw.Poll();
    CheckEq(dirty.size(), static_cast<std::size_t>(1), "Poll reports the changed file");
    if (!dirty.empty()) {
      CheckEq(dirty.front(), Canon(file), "dirty path is canonical");
    }

    CheckEq(fw.Poll().size(), static_cast<std::size_t>(0), "change is consumed — next Poll is clean");

    // Idempotent Track: re-tracking the same path does not double-count.
    fw.Track(file.string());
    CheckEq(fw.TrackedCount(), static_cast<std::size_t>(1), "re-Track of same path is idempotent");

    fw.Untrack(file.string());
    Check(fw.Empty(), "Untrack removes the entry");
    CheckEq(fw.Poll().size(), static_cast<std::size_t>(0), "Poll after Untrack returns nothing");
  }

  {
    // Tracking a nonexistent file is a no-op (stat fails, logged, stays untracked).
    FileWatcher fw;
    fw.Track((dir / "does-not-exist.txt").string());
    CheckEq(fw.TrackedCount(), static_cast<std::size_t>(0), "Track of missing file stays untracked");
  }

  {
    // A file that disappears between Track and Poll is skipped without error and re-armed: once it
    // reappears with a newer mtime, the next Poll reports it.
    FileWatcher fw;
    const fs::path transient = dir / "transient.txt";
    Write(transient, "a");
    fw.Track(transient.string());
    fs::remove(transient, ec);
    CheckEq(fw.Poll().size(), static_cast<std::size_t>(0), "vanished file is skipped, not errored");
    Write(transient, "b");
    BumpMtime(transient);
    CheckEq(fw.Poll().size(), static_cast<std::size_t>(1), "reappeared file is reported on next Poll");
  }

  fs::remove_all(dir, ec);
  return octarine::test::ReportSummary("FileWatcherTest");
}
