#!/usr/bin/env bash
# octarine-macos-notarize.sh — codesign + notarize + staple a CPack-produced .dmg.
#
# CPack DragNDrop emits an unsigned .dmg containing OctarineEngine.app. Apple Gatekeeper rejects
# unsigned .apps on first-launch on a clean Mac, and unnotarized .apps are quarantined even when
# signed with a valid Developer ID Application cert. This script closes both gaps so the shipped
# .dmg opens without a Gatekeeper warning:
#
#   1. Mount the input .dmg, copy OctarineEngine.app to a scratch dir.
#   2. codesign --deep --options=runtime over the .app (hardened runtime, scripts/octarine-macos.entitlements).
#   3. Pack the .app into a .zip (notarytool requires zip/pkg/dmg, prefers zip for speed).
#   4. xcrun notarytool submit --wait against the zip.
#   5. xcrun stapler staple the .app (embeds the ticket so offline first-launch works).
#   6. hdiutil create a new .dmg from the signed+stapled .app.
#
# Inputs (env or args):
#   INPUT_DMG                  --input    path to the CPack-produced .dmg (required)
#   OUTPUT_DMG                 --output   path to write the signed+stapled .dmg (required)
#   APPLE_TEAM_ID              --team     10-char team id (required)
#   APPLE_ID_USERNAME          --apple-id Apple ID email for notarization (required)
#   APPLE_APP_SPECIFIC_PASSWORD --password app-specific password from appleid.apple.com (required)
#   SIGN_IDENTITY              --identity codesign identity; defaults to "Developer ID Application: <team>"
#   ENTITLEMENTS               --entitlements path to .entitlements plist; defaults to scripts/octarine-macos.entitlements
#
# Assumes the signing cert is already in the default keychain and the keychain is unlocked
# (workflow handles the keychain bootstrap before calling this script).

set -euo pipefail

script_dir="$(cd "$(dirname "$0")" && pwd)"
input_dmg=""
output_dmg=""
team_id="${APPLE_TEAM_ID:-}"
apple_id="${APPLE_ID_USERNAME:-}"
apple_password="${APPLE_APP_SPECIFIC_PASSWORD:-}"
sign_identity=""
entitlements="$script_dir/octarine-macos.entitlements"

while [ $# -gt 0 ]; do
    case "$1" in
        --input)        input_dmg="$2"; shift 2 ;;
        --output)       output_dmg="$2"; shift 2 ;;
        --team)         team_id="$2"; shift 2 ;;
        --apple-id)     apple_id="$2"; shift 2 ;;
        --password)     apple_password="$2"; shift 2 ;;
        --identity)     sign_identity="$2"; shift 2 ;;
        --entitlements) entitlements="$2"; shift 2 ;;
        -h|--help)
            sed -n '1,/^set -euo pipefail/p' "$0" | sed 's/^# \{0,1\}//'
            exit 0 ;;
        *) echo "octarine-macos-notarize: unknown arg '$1'" >&2; exit 2 ;;
    esac
done

if [ -z "$input_dmg" ] || [ -z "$output_dmg" ] || [ -z "$team_id" ] \
   || [ -z "$apple_id" ] || [ -z "$apple_password" ]; then
    echo "octarine-macos-notarize: missing required arg (--input/--output/--team/--apple-id/--password)" >&2
    exit 2
fi

if [ -z "$sign_identity" ]; then
    sign_identity="Developer ID Application: ($team_id)"
fi

[ -f "$input_dmg" ] || { echo "octarine-macos-notarize: input .dmg not found at $input_dmg" >&2; exit 1; }
[ -f "$entitlements" ] || { echo "octarine-macos-notarize: entitlements file not found at $entitlements" >&2; exit 1; }

scratch=$(mktemp -d)
mount_dir="$scratch/mount"
stage_dir="$scratch/stage"
mkdir -p "$mount_dir" "$stage_dir"
trap 'hdiutil detach "$mount_dir" -quiet 2>/dev/null || true; rm -rf "$scratch"' EXIT

echo "==> Mounting $input_dmg"
hdiutil attach "$input_dmg" -mountpoint "$mount_dir" -nobrowse -quiet

app_src=$(find "$mount_dir" -maxdepth 2 -name '*.app' -print -quit)
[ -n "$app_src" ] || { echo "octarine-macos-notarize: no .app inside $input_dmg" >&2; ls -la "$mount_dir"; exit 1; }
app_name=$(basename "$app_src")
echo "==> Found app: $app_name"

# ditto preserves symlinks/extended attrs/code-signing metadata. cp -R can quietly drop them.
ditto "$app_src" "$stage_dir/$app_name"
hdiutil detach "$mount_dir" -quiet

app_path="$stage_dir/$app_name"

echo "==> Codesigning $app_name with identity: $sign_identity"
codesign --deep --force --options=runtime --timestamp \
    --entitlements "$entitlements" \
    --sign "$sign_identity" \
    "$app_path"

echo "==> Verifying codesign"
codesign --verify --deep --strict --verbose=2 "$app_path"

# notarytool takes .zip / .pkg / .dmg. Zip is fastest to upload; the .app's signed bits ride along.
notarize_zip="$scratch/notarize.zip"
echo "==> Packing for notarytool: $notarize_zip"
ditto -c -k --keepParent "$app_path" "$notarize_zip"

echo "==> Submitting to notarytool (this can take several minutes)"
xcrun notarytool submit "$notarize_zip" \
    --apple-id "$apple_id" \
    --team-id "$team_id" \
    --password "$apple_password" \
    --wait

echo "==> Stapling notarization ticket"
xcrun stapler staple "$app_path"
xcrun stapler validate "$app_path"

echo "==> Building output .dmg: $output_dmg"
rm -f "$output_dmg"
hdiutil create "$output_dmg" \
    -volname "$(basename "${output_dmg%.dmg}")" \
    -srcfolder "$app_path" \
    -ov -format UDZO

echo "==> Sanity check: spctl assessment"
spctl -a -v --type execute "$app_path" || {
    echo "octarine-macos-notarize: spctl rejected the signed app — Gatekeeper would too" >&2
    exit 1
}

echo "PASS: $output_dmg is signed, notarized, and stapled."