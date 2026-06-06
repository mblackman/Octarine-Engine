#!/usr/bin/env bash
# One-command change validator. Runs the same gates CI runs, in dependency order, and prints an
# unambiguous final verdict. An agent (or human) runs this after any change to know nothing broke.
#
# Usage: scripts/octarine-verify.sh [--fast|--full] [options]
#   --fast        (default) configure-if-needed -> build test targets -> ctest -> clang-format check
#   --full        also: build the engine, headless bake smoke, Lua-API drift check, clang-tidy
#   --keep-going  run every gate and report a summary instead of stopping at the first failure
#   --no-build    skip configure/build (reuse the existing build dir as-is)
#   --format-only run only the clang-format check on changed files
#   --tidy-strict treat clang-tidy findings as a failure (default: advisory/non-blocking)
#   -h, --help    show this help
#
# Pinned to the editor-release preset + OCTARINE_ENABLE_TESTS=ON so local green == CI green
# (.github/workflows/build.yml validates exactly this leg).
#
# Exit codes (distinct, so callers can branch):
#   0  all gates passed            13  bake smoke failed
#   2  environment/setup error     14  Lua-API artifact drift
#   10 configure failed            15  clang-format violations
#   11 build failed                16  clang-tidy findings (only with --tidy-strict)
#   12 ctest failed
#
# Env:
#   OCT_VERIFY_GAME  Path to a game dir (with config.ini) for the bake smoke.
#                    Default: <repo>/../Octarine-Engine-Example, else auto-cloned under build/.

set -uo pipefail

preset=editor-release
config_dir=release

mode=fast
keep_going=0
do_build=1
format_only=0
tidy_strict=0

while [[ $# -gt 0 ]]; do
  case "$1" in
    --fast) mode=fast ;;
    --full) mode=full ;;
    --keep-going) keep_going=1 ;;
    --no-build) do_build=0 ;;
    --format-only) format_only=1 ;;
    --tidy-strict) tidy_strict=1 ;;
    -h|--help) sed -n '2,30p' "$0" | sed 's/^# \{0,1\}//'; exit 0 ;;
    *) echo "error: unknown argument '$1' (try --help)" >&2; exit 2 ;;
  esac
  shift
done

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "$script_dir/.." && pwd)"
cd "$repo_root"
build_dir="$repo_root/build/$preset"
binary="$build_dir/bin/$config_dir/OctarineEngine"

# Distinct exit code recorded by the first failing gate (or the worst, under --keep-going).
first_fail_code=0
first_fail_name=""

fail() {  # fail <code> <name>
  local code="$1" name="$2"
  echo "  -> FAIL: $name [$code]" >&2
  if [[ $first_fail_code -eq 0 ]]; then
    first_fail_code="$code"
    first_fail_name="$name"
  fi
  if [[ $keep_going -eq 0 ]]; then
    verdict
    exit "$first_fail_code"
  fi
}

verdict() {
  if [[ $first_fail_code -eq 0 ]]; then
    echo "VERIFY OK ($mode)"
  else
    echo "VERIFY FAIL: $first_fail_name [$first_fail_code]" >&2
  fi
}

# List changed C/C++ files under src/ and tests/ (added/copied/modified/renamed vs HEAD). Falls
# back to the whole src/ + tests/ tree outside a git context.
changed_sources() {
  if git rev-parse --git-dir >/dev/null 2>&1; then
    git diff --name-only --diff-filter=ACMR HEAD -- 'src/*' 'tests/*' \
      | grep -E '\.(cpp|h|hpp)$' || true
  else
    find src tests -type f \( -name '*.cpp' -o -name '*.h' -o -name '*.hpp' \) 2>/dev/null || true
  fi
}

check_format() {
  local fmt; fmt="$(command -v clang-format || true)"
  if [[ -z "$fmt" ]]; then
    echo "  -> SKIP clang-format (not on PATH)" >&2
    return 0
  fi
  local files; mapfile -t files < <(changed_sources)
  if [[ ${#files[@]} -eq 0 ]]; then
    echo "  ok   clang-format (no changed sources)"
    return 0
  fi
  if "$fmt" --dry-run --Werror -style=file "${files[@]}"; then
    echo "  ok   clang-format (${#files[@]} files)"
  else
    echo "  (fix with: cmake --build $build_dir --target format)" >&2
    fail 15 "clang-format"
  fi
}

check_tidy() {
  local tidy; tidy="$(command -v clang-tidy || true)"
  if [[ -z "$tidy" ]]; then
    echo "  -> SKIP clang-tidy (not on PATH)" >&2
    return 0
  fi
  if [[ ! -f "$build_dir/compile_commands.json" ]]; then
    echo "  -> SKIP clang-tidy (no compile_commands.json — configure first)" >&2
    return 0
  fi
  local files; mapfile -t files < <(changed_sources | grep -E '\.cpp$' || true)
  if [[ ${#files[@]} -eq 0 ]]; then
    echo "  ok   clang-tidy (no changed .cpp files)"
    return 0
  fi
  if "$tidy" -p "$build_dir" "${files[@]}"; then
    echo "  ok   clang-tidy (${#files[@]} files)"
  elif [[ $tidy_strict -eq 1 ]]; then
    fail 16 "clang-tidy"
  else
    echo "  -> clang-tidy reported findings (advisory; pass --tidy-strict to gate)" >&2
  fi
}

# --- format-only fast exit ---------------------------------------------------
if [[ $format_only -eq 1 ]]; then
  echo "== clang-format (changed files) =="
  check_format
  verdict
  exit "$first_fail_code"
fi

# --- configure + build -------------------------------------------------------
test_targets=(
  OctarineLuaApiTest OctarineAssetPipelineTest OctarineProcessTest OctarineProjectIniTest
  OctarineEcsTest OctarineEcsHierarchyTest OctarineSystemLogicTest OctarineEventBusTest
  OctarineCollisionSystemTest OctarineCollisionResponseTest OctarineProjectileEmitSystemTest
  OctarineAssetRefcounterTest OctarineDevListenServerTest OctarineLuaBehaviorTest
  OctarineGameBakeTest
)

if [[ $do_build -eq 1 ]]; then
  if [[ ! -f "$build_dir/CMakeCache.txt" ]]; then
    echo "== configure ($preset) =="
    if ! cmake --preset "$preset" -DOCTARINE_ENABLE_TESTS=ON; then
      fail 10 "configure"
    fi
  fi
  echo "== build =="
  build_targets=("${test_targets[@]}")
  [[ "$mode" == "full" ]] && build_targets+=(OctarineEngine)
  target_args=()
  for t in "${build_targets[@]}"; do target_args+=(--target "$t"); done
  if ! cmake --build "$build_dir" "${target_args[@]}" --parallel; then
    fail 11 "build"
  fi
fi

# --- ctest -------------------------------------------------------------------
echo "== ctest =="
if ! ctest --test-dir "$build_dir" --output-on-failure; then
  fail 12 "ctest"
fi

# --- full-only gates ---------------------------------------------------------
if [[ "$mode" == "full" ]]; then
  echo "== bake smoke =="
  game_path="${OCT_VERIFY_GAME:-$repo_root/../Octarine-Engine-Example}"
  if [[ ! -d "$game_path" ]]; then
    clone_dir="$repo_root/build/.verify_game"
    if [[ ! -d "$clone_dir" ]]; then
      git clone --depth 1 https://github.com/mblackman/Octarine-Engine-Example.git "$clone_dir" >&2 \
        || { echo "  -> could not obtain an example project" >&2; fail 2 "bake-setup"; }
    else
      git -C "$clone_dir" pull >&2 || true
    fi
    game_path="$clone_dir"
  fi
  if [[ -d "$game_path" ]]; then
    game_path="$(cd "$game_path" && pwd)"
    if [[ ! -x "$binary" ]]; then
      echo "  -> engine binary not found at $binary (build --full first)" >&2
      fail 13 "bake"
    elif SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy "$binary" "$game_path" -m bake \
         && [[ -s "$game_path/asset_manifest.lua" ]]; then
      echo "  ok   bake produced a non-empty manifest"
    else
      fail 13 "bake"
    fi
  fi

  echo "== Lua API drift =="
  drift_paths=(lua_api.smoke.lua components.json modules.json)
  if git diff --quiet -- "${drift_paths[@]}"; then
    echo "  ok   no Lua API artifact drift"
  else
    echo "  -> regenerated stubs differ — commit them:" >&2
    git diff --stat -- "${drift_paths[@]}" >&2
    fail 14 "lua-api-drift"
  fi

  echo "== clang-tidy (changed files) =="
  check_tidy
fi

# --- clang-format (both modes) ----------------------------------------------
echo "== clang-format (changed files) =="
check_format

verdict
exit "$first_fail_code"
