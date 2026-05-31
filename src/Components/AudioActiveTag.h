#pragma once

// Empty tag struct attached to spatial audio emitters currently inside the listener's
// audible radius. Managed by AudioCullingSystem: added on enter, removed on exit. The
// sink-lifecycle work (halt + offset capture + sink removal) is also driven off the same
// transition so the per-frame Spatial/Doppler systems naturally skip culled entries when
// the sink is gone.
struct AudioActiveTag {};
