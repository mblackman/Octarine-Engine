# Commenting Guide

This guide defines commenting standards for Octarine Engine. The goal is to keep comments concise, natural, informative, and consistent across the entire codebase.

---

## Philosophy: Code First, Comments for Context

Code should be written to be as self-explanatory as possible through clear naming, descriptive types, and explicit structure. Comments are not a substitute for readable code; they exist to provide context that the compiler and type system cannot convey.

**Ask before writing a comment:**
- Does this explain *why* something is done this way, or does it merely restate *what* the code does?
- Does this capture a non-obvious constraint, precondition, or hardware invariant?
- Would a competent C++ developer already know this by reading the signature?

If the code alone is clear, **do not write a comment**.

---

## Core Rules

### 1. Never state the obvious

Avoid comments that repeat function names, types, or standard language idioms.

```cpp
// ❌ Bad: Redundant
// Check if entity is alive
bool IsAlive(Entity entity) const;

// Set the window title
void SetTitle(const std::string& title);

// Loop over all entities
for (const auto& entity : entities) { ... }

// ✅ Good: Self-documenting code needs no comment
bool IsAlive(Entity entity) const;
void SetTitle(const std::string& title);
for (const auto& entity : entities) { ... }
```

### 2. Document the "Why" and Non-Obvious Invariants

Use comments to explain constraints, trade-offs, thread-safety assumptions, or coordinate/unit expectations that are not obvious from types alone.

```cpp
// ❌ Bad: Describes mechanics without reasoning
// Convert degrees to radians and call texture rotated
const float deg = cmd.rotation * octarine::kRadiansToDegrees;
SDL_RenderTextureRotated(...);

// ✅ Good: Documents domain coordinate/unit conversion
// Rotation is stored in radians; SDL expects degrees.
const float deg = cmd.rotation * octarine::kRadiansToDegrees;
SDL_RenderTextureRotated(...);
```

### 3. Keep comments concise and direct

Write like a pragmatic engineer communicating with another engineer. Avoid conversational filler, marketing adjectives ("blazing fast"), defensive justifications ("pure bookkeeping - no SDL!"), and multi-paragraph essays.

```cpp
// ❌ Bad: Conversational, defensive, verbose essay
// Per-asset acquire counts. Pure bookkeeping — no SDL, no catalog, no I/O — so the refcount policy
// can be unit-tested on its own. AssetManager owns the side effects: it drives the 0 -> 1 load and
// the N -> 0 unload transitions; this class only tracks the integer behind each id. Scene-reference
// handling lives in AssetManager::AcquireAll / Validate against the AssetCatalog, not here...
class AssetRefcounter { ... };

// ✅ Good: Crisp, one-line summary
// Tracks per-asset reference counts for resource lifetime management.
class AssetRefcounter { ... };
```

### 4. No project-management or roadmap relics

Never reference pull requests, task tickets, development milestones, internal roadmaps, or design documents outside the repository.

- ❌ `// Stage 5 PR-C of ai/EditorBuildAndDeployPlan.md`
- ❌ `// Tiny loop is fine — frames are small in PR-A (<= 12 bytes).`
- ❌ `// BuildConfigUnificationPlan phase 3`
- ❌ `// Phase 4 slot — flex containers`

If an implementation is intentionally partial or deferred, use a standard `TODO`:
- ✅ `// TODO: Support flex container layout when container hierarchy lands.`

### 5. Document active constraints, not past bugs

Do not write retrospective bug reports or changelogs inside source files. If a bug revealed a tricky edge case or hardware constraint, document the constraint itself so future developers do not regress it.

```cpp
// ❌ Bad: Historical changelog / bug story
// Fixes issue where calling this twice crashed because the old pointer wasn't nulled out.
// We used to cache this in POD component but that caused issues when switching scenes.
void Release();

// ✅ Good: Documents current safety invariant
// Safe to call multiple times; clears handle on release.
void Release();
```

### 6. Public header documentation

For public classes, systems, and non-trivial interface methods, provide a single-line summary explaining the role or contract. Document non-obvious return values or preconditions.

```cpp
// ❌ Bad: Trivial parameter parroting
/// @brief Allocates the scene texture
/// @param sdlRenderer Pointer to renderer
/// @param width Width in pixels
/// @param height Height in pixels
/// @return True on success
bool CreateScene(SDL_Renderer* sdlRenderer, int width, int height);

// ✅ Good: Succinct technical contract
// Allocates the off-screen scene render target. Returns false on allocation failure.
bool CreateScene(SDL_Renderer* sdlRenderer, int width, int height);
```

### 7. Domain math, protocols, and hardware quirks

Detailed technical comments *are* encouraged when explaining complex domain-specific algorithms (e.g. ITU-R BS.1770 audio loudness filtering, spatial audio panning geometry, radix sort bit-packing, or platform-specific OS quirks like DPAPI vs. Keychain).

Keep these explanations focused on the mathematics, protocol layout, or hardware behavior—not personal commentary.

---

## Quick Reference Summary

| Situation | Guideline |
|-----------|-----------|
| Standard getters / setters | **No comment**. Self-documenting. |
| Obvious control flow | **No comment**. |
| Coordinate space or units (radians, degrees, world vs screen) | **Comment briefly** stating unit expectations. |
| Non-obvious thread-safety constraints | **Comment** thread expectations (`Must run on main thread`). |
| Protocol / binary wire format | **Comment** with compact ASCII diagram or field layout. |
| Work remaining | `// TODO: <actionable summary>` |
| Suppressions (`NOLINT`) | One-line comment explaining the reason for suppression. |
| External plans or PR tokens | **Forbidden**. |
