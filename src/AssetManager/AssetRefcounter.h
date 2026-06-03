#pragma once

#include <map>
#include <string>

// Per-asset acquire counts. Pure bookkeeping — no SDL, no catalog, no I/O — so the refcount policy
// can be unit-tested on its own. AssetManager owns the side effects: it drives the 0 -> 1 load and
// the N -> 0 unload transitions; this class only tracks the integer behind each id.
//
// Scene-reference handling (which ids a scene pulls in, and on-disk validation of them) lives in
// AssetManager::AcquireAll / Validate against the AssetCatalog, not here — those need the catalog,
// whereas the count itself does not.
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
