// Unit checks for AssetRefcounter — the pure acquire-count bookkeeping split out of AssetManager
// in Stage 10. The point of the split is that this policy needs neither SDL nor a renderer, so the
// test links nothing but the standard library. gtest-free; exit code = failed-check count.
// Registered with ctest as AssetRefcounterTest.

#include "AssetManager/AssetRefcounter.h"
#include "TestHarness.h"

using octarine::test::Check;
using octarine::test::CheckEq;

int main() {
  {
    AssetRefcounter rc;
    CheckEq(rc.Count("a"), 0, "untracked id counts as 0");
    Check(!rc.Has("a"), "untracked id is not Has()");
    CheckEq(rc.Decrement("a"), -1, "Decrement of untracked id returns -1 (no-op signal)");
  }

  {
    // 0 -> 1 transition signals the caller to load; subsequent bumps do not.
    AssetRefcounter rc;
    CheckEq(rc.Increment("tex"), 1, "first Increment yields 1 (load transition)");
    Check(rc.Has("tex"), "id is Has() after Increment");
    CheckEq(rc.Increment("tex"), 2, "second Increment yields 2");
    CheckEq(rc.Increment("tex"), 3, "third Increment yields 3");
    CheckEq(rc.Count("tex"), 3, "Count reflects the three references");
  }

  {
    // Decrement walks back down; the entry is erased exactly at zero (the unload transition).
    AssetRefcounter rc;
    rc.Increment("snd");
    rc.Increment("snd");
    CheckEq(rc.Decrement("snd"), 1, "Decrement from 2 yields 1 (still referenced)");
    CheckEq(rc.Decrement("snd"), 0, "Decrement from 1 yields 0 (unload transition)");
    CheckEq(rc.Count("snd"), 0, "count is 0 after reaching zero");
    Check(!rc.Has("snd"), "id is no longer Has() after reaching zero");
    CheckEq(rc.Decrement("snd"), -1, "Decrement past zero returns -1 (untracked again)");
  }

  {
    // Adopt forces an already-resident handle to count 1 regardless of prior state.
    AssetRefcounter rc;
    rc.Adopt("font");
    CheckEq(rc.Count("font"), 1, "Adopt sets count to 1");
    rc.Increment("font");
    rc.Adopt("font");
    CheckEq(rc.Count("font"), 1, "Adopt resets a higher count back to 1");
  }

  {
    // Independent ids do not interfere; Clear drops everything.
    AssetRefcounter rc;
    rc.Increment("a");
    rc.Increment("b");
    rc.Increment("b");
    CheckEq(rc.Count("a"), 1, "id 'a' isolated from 'b'");
    CheckEq(rc.Count("b"), 2, "id 'b' isolated from 'a'");
    rc.Clear();
    CheckEq(rc.Count("a"), 0, "Clear drops 'a'");
    CheckEq(rc.Count("b"), 0, "Clear drops 'b'");
  }

  return octarine::test::ReportSummary("AssetRefcounterTest");
}
