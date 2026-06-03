# Contributing to Octarine Engine

Thanks for considering a contribution. This document covers the conventions
the project follows so reviews stay focused on the change itself rather than
on hygiene. If anything here is unclear or out of date, open an issue.

## Quick reference

- New to the codebase? Start with [`docs/QUICKSTART.md`](docs/QUICKSTART.md).
- Architectural background: [`docs/ecs-architecture.md`](docs/ecs-architecture.md),
  [`docs/lua-scripting.md`](docs/lua-scripting.md).
- Shipping artifacts: [`docs/device-builds.md`](docs/device-builds.md).

## Workflow

1. **Branch off `main`.** Feature branches live as long as they need to. Pull
   `main` before opening a PR so your diff is current.
2. **One concern per PR.** A bug fix, a feature, or a refactor — not all
   three. Bundled PRs slow reviews and bury regressions.
3. **Push the branch to `origin`, open a PR.** CI runs on every push and PR;
   land green or explain why.

Pull requests target `main`. Releases are cut by tagging `v*` on `main`; see
[`docs/device-builds.md`](docs/device-builds.md) § CI for what tag pushes
trigger.

## Building and testing

Build with CMake presets:

```bash
cmake --preset editor-debug
cmake --build --preset editor-debug
```

Engine tests are gated behind `OCTARINE_ENABLE_TESTS=ON`:

```bash
cmake --preset editor-debug -DOCTARINE_ENABLE_TESTS=ON
cmake --build --preset editor-debug
ctest --test-dir build/editor-debug --output-on-failure
```

Two tests live in `tests/`:

- **`LuaApiSmokeTest`** — replays `Game::Setup`'s Lua-binding sequence
  headlessly, asserts every component / module / system surface is reachable
  from Lua, and re-emits the EmmyLua stub at `lua_api.smoke.lua`. CI fails on
  drift, so commit the regenerated stub whenever a binding changes.
- **`AssetPipelineTest`** — exercises `.meta` sidecar parsing, catalog build,
  manifest round-trip, and validation against a fixture asset tree.

CI runs both on the Linux editor-release leg.

## Coding standards

### Format

`.clang-format` is foundational. Run it before committing — most editors do
this on save with the right config.

**Pinned version.** clang-format output differs between major versions, so CI
pins an exact one (`CLANG_FORMAT_VERSION` in `.github/workflows/build.yml`).
Match it locally — the simplest way is the pip wheel:

```
pipx install clang-format==18.1.8      # then: cmake --build <preset> --target format
```

**CI behavior.** Two jobs back this up:

- `lint (clang-format)` (in build.yml) — blocking gate; fails a PR whose changed
  files aren't formatted with the pinned version.
- `autoformat` (autoformat.yml) — for same-repo PRs, formats the changed files
  and pushes a fixup commit back to your branch automatically, so the gate goes
  green on its own. Fork PRs aren't auto-pushed; format locally instead.

> Maintainer setup: the auto-push uses a fine-grained PAT secret
> `AUTOFORMAT_PAT` (repo scope, `contents: write`). Without it the job still runs
> but its fixup commit won't re-trigger the required checks.

**Pre-commit hook (optional, recommended).** To catch formatting before it leaves
your machine — and skip the CI fixup round-trip — enable the tracked hook once per
clone:

```bash
git config core.hooksPath scripts/hooks
```

`scripts/hooks/pre-commit` runs clang-format (`--dry-run --Werror`) over your staged
`src/`/`tests/` C/C++ files using the same filter as the CI gate, and blocks the
commit on violations. Bypass a single commit with `git commit --no-verify`. It
prefers `clang-format-18` and falls back to `clang-format` with a version warning.
(Setting `core.hooksPath` points git at `scripts/hooks` for all hooks, so move any
existing local hooks there if you rely on them.)

### Lint

`.clang-tidy` enforces a curated check set. Two thresholds worth knowing:

- **Function length:** 120 lines.
- **Cognitive complexity:** 15 per function.

If you trip either, the right fix is almost always to extract a helper.
Suppressing with `// NOLINT` is reserved for genuinely unavoidable cases —
add a one-line comment justifying it.

**Local vs CI — where it's advisory, where it bites.** `.clang-tidy` deliberately
leaves `WarningsAsErrors` empty, so clang-tidy stays *advisory* locally: editor
squiggles and whole-tree `-DOCTARINE_ENABLE_LINTING=ON` builds surface warnings
without hard-failing on pre-existing debt in files you didn't touch. The
enforcement happens in CI, which promotes every check to an **error over your
PR's changed lines only** (via `clang-tidy-diff`). So a check can scroll past
locally as a warning yet fail the PR.

To get CI's exact verdict before you push:

```bash
cmake --preset editor-release          # emits build/editor-release/compile_commands.json
scripts/lint-diff.sh                   # tidies your changed lines, warnings-as-errors, vs origin/main
```

`scripts/lint-diff.sh [BASE_REF] [BUILD_DIR]` mirrors the CI gate line-for-line.
Match CI's pinned major version for identical results — `apt install clang-tidy-18`
(the script prefers `clang-tidy-18`, falling back to whatever `clang-tidy` is on
`PATH` with a warning).

### Includes

Includes under `src/` are **src-rooted**, never parent-relative. `src/` is on every
target's include path (via `cmake/octarine_library.cmake`), so spell a header by its
path from `src/`:

```cpp
#include "EventBus/EventBus.h"      // ✅ src-rooted
#include "../EventBus/EventBus.h"   // ❌ parent-relative — rejected by CI
```

Parent-relative paths break when a file moves and let the same header be spelled two
ways across the tree. The `lint` job (and `scripts/hooks/pre-commit`) fail on any
`#include "../...` under `src/`. Third-party/system includes (`<SDL3/SDL.h>`,
`"imgui.h"`) are unaffected.

### Naming

Match the surrounding code:

| Surface | Convention |
|---------|-----------|
| Class / struct / enum | `PascalCase` (`HealthComponent`) |
| Function / method | `PascalCase` (`CreateEntity`, `Update`) |
| Member variable | `snake_case_` (trailing underscore, e.g. `delta_time_`) |
| Local / parameter | `camelCase` (`deltaTime`, `entityLocation`) |
| Constant / `constexpr` | `kPascalCase` (`kDefaultWindowWidth`) |

(`.clang-tidy` configures `FunctionCase: camelBack`, but the codebase uses `PascalCase` for functions
and nothing enforces that key — follow the code.) If you find a genuine stylistic disagreement, raise
it in a separate PR rather than reformatting on the side of a feature.

### Registering a new `.cpp`

The engine is split into per-layer static libraries, so every new `.cpp` under
`src/` must be added to the matching layer's source list in `src/CMakeLists.txt`
(`octarine_core`, `_assets`, `_renderer`, `_lua`, `_systems`, `_editor`, or
`_engine`) — not the root `CMakeLists.txt`. Header-only additions don't need
this. Forgetting it means your file compiles locally but fails CI on the
editor-release leg.

### File organization

- New components: `src/Components/<Name>Component.h`. POD struct, no
  behavior beyond own-field methods (see [Lua scripting:
  adding component methods](docs/lua-scripting.md#8-adding-component-methods)).
- New systems: `src/Systems/<Name>System.{h,cpp}`. Register in `Game::Setup`.
- New Lua component bindings: `src/Lua/Bindings/<X>ComponentLuaBinding.h` +
  one line in `RegisterAllBindings.cpp`.
- New Lua module (free-function globals): `src/Lua/Modules/<X>ModuleLuaBinding.{h,cpp}` +
  one line in `RegisterAllModules.cpp` + add the `.cpp` to the `octarine_lua` list in `src/CMakeLists.txt`.
- New events: `src/Events/<X>Event.h`. Emit via `EmitEvent<X>`; subscribe in
  the consuming system's `Init`.

## Commit messages

Plain capitalized summary. No conventional-commit prefixes (`feat:`, `fix:`,
`chore:` etc.). Keep the subject under ~70 characters; use the body for
context if "why" isn't obvious from the diff.

Good:

```
Pack textures into bake-time atlases

Atlas packing happens during the bake step rather than at first-use so
shipping builds don't pay the cost on cold launch. Bin packer is from
stb_rect_pack; metadata is emitted alongside asset_manifest.lua.
```

Avoid:

- `feat: add atlas packing` — no conventional-commit prefix.
- `Implement atlas packing logic for textures` — verbose subject, no body.
- `Co-Authored-By: <AI tool>` trailers — **never** add these. No `Co-Authored-By` /
  `Co-authored-by` line crediting Claude or any AI tool, and no AI listed as commit author.
  This holds even if a tool's default behavior is to append one.

## Documentation

Tracked docs live under `docs/` (plus `README.md` and this file at the repo
root, and `android/README.md` for the Android host app). Two hard rules for
tracked docs and tracked source comments:

- **No references to `ai/` design plans.** That directory is local-only
  scratchpad (gitignored). Plans rot, get renamed, get archived — links to
  them go stale immediately. Inline the rationale instead ("parked on
  defer/ios pending an Apple Developer account", not "see ai/iOSDeferralPlan.md").
- **No references to local-only agent files** such as `CLAUDE.md`,
  `GEMINI.md`, or `.agent/`. Same reasoning.

If you add a doc, cross-link it from
[`docs/QUICKSTART.md`](docs/QUICKSTART.md) § Deeper reading so it isn't
orphaned.

## Reporting bugs and proposing features

Open a GitHub issue. For bugs, include:

- The preset you built with (`editor-debug`, `ship-release`, …).
- OS + compiler version.
- Repro steps. Minimal `game.lua` snippet if it's reproducible in script.
- Logs from the run's stdout/stderr (or `adb logcat -s Octarine OctarineLua`
  on Android).

For feature proposals, describe the problem before the solution — the
solution space is often wider than it looks.
