#!/usr/bin/env bash
# Reproduce CI's blocking clang-tidy gate locally.
#
# CI ("clang-tidy (changed lines, blocking)" in .github/workflows/build.yml) promotes every
# clang-tidy diagnostic on the lines your branch *changed* to an error, while ignoring pre-existing
# debt on untouched lines. .clang-tidy itself keeps WarningsAsErrors empty so editor integration and
# whole-tree `-DOCTARINE_ENABLE_LINTING=ON` builds stay advisory — so the only place that strict,
# changed-lines verdict exists is CI. This script runs that exact verdict on your machine so you can
# confirm the lint gate passes before pushing.
#
# Usage:
#   scripts/lint-diff.sh [BASE_REF] [BUILD_DIR]
#     BASE_REF   git ref to diff against (default: origin/main, then main)
#     BUILD_DIR  dir containing compile_commands.json (default: build/editor-release)
#
# Requires a configured build that emitted compile_commands.json (editor-release matches CI).
# Prefers clang-tidy-18 to match CI's pinned major version; falls back to whatever clang-tidy is on
# PATH with a warning, since diagnostics can differ across clang-tidy versions.
set -euo pipefail

cd "$(git rev-parse --show-toplevel)"

base="${1:-}"
build_dir="${2:-build/editor-release}"

# Resolve a base ref. `git diff A...HEAD` already diffs from the merge-base of A and HEAD, so this
# only ever surfaces your branch's own changes — exactly what CI passes as the PR base.
if [ -z "$base" ]; then
  if git rev-parse --verify --quiet origin/main >/dev/null; then
    base="origin/main"
  elif git rev-parse --verify --quiet main >/dev/null; then
    base="main"
  else
    echo "lint-diff: no origin/main or main found; pass a BASE_REF explicitly." >&2
    exit 2
  fi
fi

if [ ! -f "$build_dir/compile_commands.json" ]; then
  echo "lint-diff: $build_dir/compile_commands.json not found." >&2
  echo "  Configure a build first, e.g.:  cmake --preset editor-release" >&2
  exit 2
fi

if command -v clang-tidy-18 >/dev/null 2>&1; then
  tidy_bin=clang-tidy-18
elif command -v clang-tidy >/dev/null 2>&1; then
  tidy_bin=clang-tidy
  echo "lint-diff: clang-tidy-18 not found; using '$tidy_bin'. CI pins clang-tidy-18, so results may differ." >&2
else
  echo "lint-diff: no clang-tidy on PATH. Install clang-tidy-18 to match CI." >&2
  exit 2
fi

changed="$(git diff --name-only --diff-filter=ACMR "$base"...HEAD -- 'src/*' 'tests/*' | grep -E '\.cpp$' || true)"
if [ -z "$changed" ]; then
  echo "lint-diff: no changed .cpp files under src/ or tests/ — nothing to lint."
  exit 0
fi
printf 'lint-diff: tidying changed lines in:\n%s\n' "$(printf '  %s\n' $changed)"

# clang-tidy-diff.py ships inside the llvm-18 toolchain (the apt clang-tidy-18 copy carries the
# -warnings-as-errors flag CI relies on). Fall back to the upstream `main` copy, which also defines
# that flag — released tags (≤ llvm 20) and older system copies (e.g. llvm-14) do NOT, and silently
# misapplying it would let violations pass. Prefer the llvm-18 copy.
diff_tool="$(find /usr/lib/llvm-18 -name 'clang-tidy-diff.py' 2>/dev/null | head -1 || true)"
if [ -z "$diff_tool" ]; then
  diff_tool="$(mktemp -t clang-tidy-diff.XXXXXX.py)"
  trap 'rm -f "$diff_tool"' EXIT
  curl -fsSL https://raw.githubusercontent.com/llvm/llvm-project/main/clang-tools-extra/clang-tidy/tool/clang-tidy-diff.py -o "$diff_tool"
fi

# Guard: never run a clang-tidy-diff.py that lacks -warnings-as-errors — without it, diagnostics on
# changed lines would NOT be promoted to errors and the gate would falsely pass.
if ! grep -q -- '-warnings-as-errors' "$diff_tool"; then
  echo "lint-diff: $diff_tool has no -warnings-as-errors support; it would falsely pass." >&2
  echo "  Install the apt clang-tidy-18 toolchain (provides a compatible clang-tidy-diff.py)." >&2
  exit 2
fi

# -p1 strips the a/ b/ diff prefixes; -path points at compile_commands.json; the diff's -U0 hunks
# limit diagnostics to touched lines; any diagnostic there is promoted to an error and fails the run.
git diff -U0 "$base"...HEAD -- 'src/*' 'tests/*' \
  | python3 "$diff_tool" -p1 -path "$build_dir" \
      -clang-tidy-binary "$tidy_bin" -warnings-as-errors='*'
