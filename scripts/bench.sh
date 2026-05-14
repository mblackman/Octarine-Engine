#!/usr/bin/env bash
# Run OctarineEngine headlessly for a short period to capture TIMER: output.
#
# Usage: scripts/bench.sh [preset] [duration_seconds]
#   preset:   player-profile (default: player-profile)
#   duration: integer seconds (default: 8)
#
# The stress scene reaches steady state in ~2s. The default 8s run combined
# with the parser's --warmup 2 flag gives ~6s of pure plateau data.
# Bump the duration if you want more signal.
#
# Env:
#   OCT_BENCH_GAME   Path to a game dir containing config.ini.
#                    Default: <repo>/../Octarine-Engine-Example
#   OCT_BENCH_MODE   Startup mode passed to the game's Lua (selects which scene
#                    to auto-load). Default: stress. The example game also
#                    accepts main, map_editor, overlap.
#
# Exits 0 on the expected `timeout` kill (124) or a clean exit (0). Anything
# else (build failure, missing config, segfault) propagates.

set -euo pipefail

preset="${1:-player-profile}"
duration="${2:-8}"

case "$preset" in
  player-profile) config_dir=relwithdebinfo ;;
  *)
    echo "error: preset must be player-profile (got '$preset')" >&2
    echo "       other presets have OCTARINE_ENABLE_PROFILING=OFF, so no TIMER output." >&2
    exit 2
    ;;
esac

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "$script_dir/.." && pwd)"
build_dir="$repo_root/cmake-build-$preset"
binary="$build_dir/bin/$config_dir/OctarineEngine"

game_path="${OCT_BENCH_GAME:-$repo_root/../Octarine-Engine-Example}"
if [[ ! -d "$game_path" ]]; then
  echo "Notice: Example game not found at $game_path." >&2
  echo "Auto-cloning Octarine-Engine-Example for benchmarking..." >&2
  
  auto_clone_dir="$repo_root/.benchmark_game"
  if [[ ! -d "$auto_clone_dir" ]]; then
    git clone --depth 1 https://github.com/mblackman/Octarine-Engine-Example.git "$auto_clone_dir" >&2
  else
    # Ensure it's up to date if it already exists
    git -C "$auto_clone_dir" pull >&2
  fi
  game_path="$auto_clone_dir"
fi
game_path="$(cd "$game_path" && pwd)"
if [[ ! -f "$game_path/config.ini" ]]; then
  echo "error: $game_path is missing config.ini" >&2
  exit 2
fi

if [[ ! -f "$build_dir/CMakeCache.txt" ]]; then
  cmake --preset "$preset" >&2
fi
if [[ ! -x "$binary" ]]; then
  cmake --build "$build_dir" --target OctarineEngine -- -k 0 >&2
fi

mode="${OCT_BENCH_MODE:-stress}"

set +e
SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy \
  timeout "$duration" "$binary" --path "$game_path" --startup-mode "$mode"
status=$?
set -e

if [[ $status -eq 124 || $status -eq 0 ]]; then
  exit 0
fi
exit "$status"
