#!/usr/bin/env bash
# verify-icons.sh — end-to-end gate for the project.ini icon=> shipped-package pipeline.
#
# Closes the loop on a class of silent regression: project.ini sets `icon=`, octarine-icons.cmake
# runs at configure, *but* the generated mipmap/ic_launcher.png never lands in the AAB (a
# sourceSets misconfig, a Gradle merge order slip, anything else that would leave the SDL stock
# green robot in the shipped artifact). Compares the icon shipped in the AAB to the stock SDL
# template; *if the project declared an icon, they must differ*.
#
# When project.ini omits `icon=` (mid-bootstrap projects), the gate skips — fallback path is
# legitimate, not a regression.
#
# Usage:
#   verify-icons.sh --project <project-dir> --aab <path/to/app-release.aab>
#   verify-icons.sh --project <project-dir> --stock <path/to/stock/ic_launcher.png> \
#                   --aab <path/to/app-release.aab>
#
# Stock defaults to android/app/src/main/res/mipmap-xxxhdpi/ic_launcher.png relative to this
# script (the SDL-provided green robot we ship as the template fallback).

set -euo pipefail

script_dir="$(cd "$(dirname "$0")" && pwd)"
default_stock="$script_dir/../android/app/src/main/res/mipmap-xxxhdpi/ic_launcher.png"

project=""
aab=""
stock="$default_stock"

while [ $# -gt 0 ]; do
    case "$1" in
        --project) project="$2"; shift 2 ;;
        --aab)     aab="$2"; shift 2 ;;
        --stock)   stock="$2"; shift 2 ;;
        -h|--help)
            sed -n '1,/^set -euo pipefail/p' "$0" | sed 's/^# \{0,1\}//'
            exit 0 ;;
        *) echo "verify-icons: unknown arg '$1'" >&2; exit 2 ;;
    esac
done

if [ -z "$project" ] || [ -z "$aab" ]; then
    echo "verify-icons: --project and --aab are required" >&2
    exit 2
fi

ini="$project/project.ini"
if [ ! -f "$ini" ]; then
    echo "verify-icons: no project.ini at $ini — nothing to verify (fallback path will ship)."
    exit 0
fi

icon_line=$(grep -E '^[[:space:]]*icon[[:space:]]*=' "$ini" | tail -n 1 || true)
if [ -z "$icon_line" ]; then
    echo "verify-icons: project.ini has no icon= — fallback path is legitimate, skipping."
    exit 0
fi

if [ ! -f "$aab" ]; then
    echo "verify-icons: AAB not found at $aab" >&2
    exit 1
fi

# Pull every shipped mipmap-*/ic_launcher.png entry from the AAB. The base path varies between
# debug APK (res/mipmap-*) and release AAB (base/res/mipmap-*-v4/...); list+grep covers both.
shipped=$(unzip -l "$aab" | awk '/(res|base\/res)\/mipmap-[^\/]+\/ic_launcher\.png$/ {print $NF}')
if [ -z "$shipped" ]; then
    echo "verify-icons: AAB carries no ic_launcher.png entries — Gradle res merge broke" >&2
    unzip -l "$aab" | grep -i mipmap || true
    exit 1
fi

# The committed stock mipmap PNGs were dropped once the generator became the only source of
# truth (no more committed res/mipmap-*/ic_launcher.png). When no stock reference is available
# the divergence check is moot: every shipped ic_launcher.png is generator-emitted by
# construction. Just confirm the AAB carries non-empty entries.
if [ ! -f "$stock" ]; then
    tmp=$(mktemp -d)
    trap 'rm -rf "$tmp"' EXIT
    total=0
    for entry in $shipped; do
        total=$((total + 1))
        unzip -p "$aab" "$entry" > "$tmp/shipped.png"
        if [ ! -s "$tmp/shipped.png" ]; then
            echo "FAIL: $entry is empty — generator emitted a zero-byte PNG." >&2
            exit 1
        fi
        echo "ok:   $entry ($(stat -c%s "$tmp/shipped.png" 2>/dev/null || wc -c < "$tmp/shipped.png") bytes)"
    done
    echo "PASS: $total shipped mipmap/ic_launcher.png entries are non-empty (stock-template comparison skipped — no stock on disk)."
    exit 0
fi

stock_hash=$(sha256sum "$stock" | awk '{print $1}')
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT

mismatch=0
total=0
for entry in $shipped; do
    total=$((total + 1))
    unzip -p "$aab" "$entry" > "$tmp/shipped.png"
    h=$(sha256sum "$tmp/shipped.png" | awk '{print $1}')
    if [ "$h" != "$stock_hash" ]; then
        mismatch=$((mismatch + 1))
        echo "diff: $entry (sha256 $h) — project icon shipped"
    else
        echo "same: $entry — stock SDL template shipped"
    fi
done

# At least one density must differ from stock. Some densities might legitimately match (e.g. if
# the project icon happens to hash-collide on a tiny mdpi square — vanishingly unlikely with SHA256
# over a magick-resized 1024x1024 source) but all-match means the merge didn't pick up the
# generated res tree.
if [ "$mismatch" -eq 0 ]; then
    echo "FAIL: project.ini declares icon= but every shipped mipmap/ic_launcher.png matches the SDL template." >&2
    echo "      Generated icons under android/app/build/generated/octarine/res/ never reached the AAB." >&2
    exit 1
fi

echo "PASS: $mismatch of $total shipped mipmap/ic_launcher.png entries diverge from the stock template."