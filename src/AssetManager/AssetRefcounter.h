#pragma once

#include <map>
#include <string>

// Tracks per-asset acquire counts, decoupled from SDL and I/O.
// AssetManager uses these counts to drive loading (0 -> 1) and unloading (N -> 0).
class AssetRefcounter {
 public:
  [[nodiscard]] int Count(const std::string& id) const {
    const auto it = counts_.find(id);
    return it == counts_.end() ? 0 : it->second;
  }

  // True when the id currently holds at least one live reference.
  [[nodiscard]] bool Has(const std::string& id) const { return Count(id) > 0; }

  // Bump the reference count, returning the new value. A result of 1 marks the 0 -> 1 transition,
  // i.e. the caller should load the underlying handle now.
  int Increment(const std::string& id) { return ++counts_[id]; }

  // Force the count to 1. Used when adopting a handle that became resident outside the refcount
  // path (a legacy direct Add, or an atlas member that piggy-backs on its atlas's handle).
  void Adopt(const std::string& id) { counts_[id] = 1; }

  // Drop one reference. Returns the new count, erasing the entry when it hits zero. Returns -1 when
  // the id was never tracked so callers can treat that as a no-op (untracked ids are ignored).
  int Decrement(const std::string& id) {
    const auto it = counts_.find(id);
    if (it == counts_.end()) return -1;
    if (--it->second > 0) return it->second;
    counts_.erase(it);
    return 0;
  }

  void Clear() { counts_.clear(); }

 private:
  std::map<std::string, int> counts_;
};
