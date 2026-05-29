#!/usr/bin/env bash
# Build a signed iOS .ipa from a game project.
#
# Pipeline: cmake configure (ship-ios-device preset) -> xcodebuild archive ->
# xcodebuild -exportArchive with a generated ExportOptions.plist -> copy .ipa
# to --output. The CMake side is already wired by octarine_package's iOS
# branch (Info.plist from project.ini, POST_BUILD copy of the staged project
# into the .app); this script owns the xcodebuild wrap that produces the
# distributable .ipa, which CPack does not handle on iOS.
#
# Usage: scripts/build-ios-ipa.sh --project <game-dir> --team <APPLE_TEAM_ID> [options]
#
# Required:
#   --project DIR     Game project (asset root with config.ini + project.ini)
#   --team ID         Apple Developer team identifier (10-char alphanumeric)
#
# Optional:
#   --identity STR    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY, e.g. "Apple Development"
#                     or "Apple Distribution: Your Studio (TEAMID)". Default lets
#                     Xcode pick from the keychain based on --method.
#   --method M        Export method: development (default) | ad-hoc | app-store |
#                     enterprise. Drives ExportOptions.plist + which identity
#                     xcodebuild grabs by default.
#   --build-dir DIR   CMake build directory. Default: build/ship-ios-device.
#   --output PATH     Final .ipa destination. Default: $build_dir/OctarineEngine.ipa.
#   --host-bake-exe P Host-native OctarineEngine for the PRE_BUILD asset bake.
#                     If unset the project must ship a committed asset_manifest.lua
#                     (the Android contract — see CLAUDE.md).
#   --name STR        Override project.ini: name (CMake -DOCTARINE_PACKAGE_NAME)
#   --id STR          Override project.ini: package_id (CFBundleIdentifier)
#   --version STR     Override project.ini: version_name (CFBundleShortVersionString)
#   --version-code N  Override project.ini: version_code (CFBundleVersion)
#   --vendor STR      Override project.ini: vendor
#   --description STR Override project.ini: description
#   --skip-configure  Skip the cmake configure step (assumes --build-dir already
#                     contains a generated Xcode project). Useful for re-archiving
#                     after a code change.
#   --unsigned-archive Produce an UNSIGNED .ipa (CODE_SIGNING_ALLOWED=NO on the
#                     archive, no exportArchive — pack Payload/<App>.app from the
#                     xcarchive directly). Won't install on a real device, but
#                     proves the device-target configure+archive end-to-end and
#                     exercises the script without secrets. --team becomes
#                     optional in this mode.
#   -h, --help        Show this help.
#
# Env (fallback for the corresponding flag — flag wins if both present):
#   OCTARINE_IOS_TEAM, OCTARINE_IOS_IDENTITY, OCTARINE_IOS_METHOD,
#   OCTARINE_PROJECT, OCTARINE_HOST_BAKE_EXE.
#
# Notes:
# - macOS only. xcodebuild + a Developer-installed signing cert + a provisioning
#   profile matching --team and --id are prerequisites.
# - "development" method only installs on team-registered UDIDs; use "ad-hoc"
#   for wider internal distribution and "app-store" for TestFlight/App Store.
# - The simulator build doesn't need this script — it produces a .app, not a
#   .ipa. Use `cmake --preset ship-ios-simulator` + `cmake --build` directly
#   (mirrors .github/workflows/ios.yml).

set -euo pipefail

# ---- Arg parsing ------------------------------------------------------------

usage() {
    sed -n '1,/^set -euo pipefail$/{/^set/!p;}' "${BASH_SOURCE[0]}"
}

project="${OCTARINE_PROJECT:-}"
team="${OCTARINE_IOS_TEAM:-}"
identity="${OCTARINE_IOS_IDENTITY:-}"
method="${OCTARINE_IOS_METHOD:-development}"
build_dir=""
output=""
host_bake_exe="${OCTARINE_HOST_BAKE_EXE:-}"
pkg_name=""
pkg_id=""
pkg_version=""
pkg_version_code=""
pkg_vendor=""
pkg_description=""
skip_configure=0
unsigned_archive=0

while [ $# -gt 0 ]; do
    case "$1" in
        --project)         project="$2"; shift 2 ;;
        --team)            team="$2"; shift 2 ;;
        --identity)        identity="$2"; shift 2 ;;
        --method)          method="$2"; shift 2 ;;
        --build-dir)       build_dir="$2"; shift 2 ;;
        --output)          output="$2"; shift 2 ;;
        --host-bake-exe)   host_bake_exe="$2"; shift 2 ;;
        --name)            pkg_name="$2"; shift 2 ;;
        --id)              pkg_id="$2"; shift 2 ;;
        --version)         pkg_version="$2"; shift 2 ;;
        --version-code)    pkg_version_code="$2"; shift 2 ;;
        --vendor)          pkg_vendor="$2"; shift 2 ;;
        --description)     pkg_description="$2"; shift 2 ;;
        --skip-configure)  skip_configure=1; shift ;;
        --unsigned-archive) unsigned_archive=1; shift ;;
        -h|--help)         usage; exit 0 ;;
        *) echo "build-ios-ipa: unknown arg: $1" >&2; usage >&2; exit 2 ;;
    esac
done

[ "$(uname -s)" = "Darwin" ] || { echo "build-ios-ipa: macOS only (uname=$(uname -s))" >&2; exit 1; }
[ -n "$project" ] || { echo "build-ios-ipa: --project is required" >&2; exit 2; }
[ -d "$project" ] || { echo "build-ios-ipa: project dir does not exist: $project" >&2; exit 2; }
if [ "$unsigned_archive" -eq 0 ]; then
    [ -n "$team" ] || { echo "build-ios-ipa: --team is required (Apple Developer team id; or pass --unsigned-archive)" >&2; exit 2; }
fi

case "$method" in
    development|ad-hoc|app-store|enterprise) ;;
    *) echo "build-ios-ipa: --method must be one of: development | ad-hoc | app-store | enterprise" >&2; exit 2 ;;
esac

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "$script_dir/.." && pwd)"
project="$(cd "$project" && pwd)"

: "${build_dir:=$repo_root/build/ship-ios-device}"
: "${output:=$build_dir/OctarineEngine.ipa}"

archive_path="$build_dir/OctarineEngine.xcarchive"
export_dir="$build_dir/_export"
export_plist="$build_dir/_ExportOptions.plist"

# ---- Configure --------------------------------------------------------------

# CMake cache vars passed through to the preset. Identity is wired via
# OCTARINE_IOS_DEVELOPMENT_TEAM / OCTARINE_IOS_CODE_SIGN_IDENTITY (consumed by
# octarine_package's iOS branch, see cmake/OctarinePackage.cmake).
cmake_args=(
    --preset ship-ios-device
    -DOCTARINE_PACKAGE_PROJECT="$project"
)
[ -n "$team" ]             && cmake_args+=(-DOCTARINE_IOS_DEVELOPMENT_TEAM="$team")
[ -n "$identity" ]         && cmake_args+=(-DOCTARINE_IOS_CODE_SIGN_IDENTITY="$identity")
[ -n "$host_bake_exe" ]    && cmake_args+=(-DOCTARINE_HOST_BAKE_EXE="$host_bake_exe")
[ -n "$pkg_name" ]         && cmake_args+=(-DOCTARINE_PACKAGE_NAME="$pkg_name")
[ -n "$pkg_id" ]           && cmake_args+=(-DOCTARINE_PACKAGE_ID="$pkg_id")
[ -n "$pkg_version" ]      && cmake_args+=(-DOCTARINE_PACKAGE_VERSION_NAME="$pkg_version")
[ -n "$pkg_version_code" ] && cmake_args+=(-DOCTARINE_PACKAGE_VERSION_CODE="$pkg_version_code")
[ -n "$pkg_vendor" ]       && cmake_args+=(-DOCTARINE_PACKAGE_VENDOR="$pkg_vendor")
[ -n "$pkg_description" ]  && cmake_args+=(-DOCTARINE_PACKAGE_DESCRIPTION="$pkg_description")

if [ "$skip_configure" -eq 0 ]; then
    echo "build-ios-ipa: configuring ($build_dir)"
    (cd "$repo_root" && cmake "${cmake_args[@]}")
else
    [ -d "$build_dir" ] || { echo "build-ios-ipa: --skip-configure but $build_dir does not exist" >&2; exit 2; }
fi

xcodeproj="$build_dir/OctarineEngine.xcodeproj"
[ -d "$xcodeproj" ] || { echo "build-ios-ipa: no Xcode project at $xcodeproj (configure failed?)" >&2; exit 1; }

# ---- Archive ----------------------------------------------------------------

# Clean any prior archive — xcodebuild won't overwrite, just appends "-1" etc.
rm -rf "$archive_path"

xcodebuild_args=(
    -project "$xcodeproj"
    -scheme OctarineEngine
    -configuration Release
    -destination 'generic/platform=iOS'
    -archivePath "$archive_path"
)
if [ "$unsigned_archive" -eq 1 ]; then
    # No signing on the archive. The build settings hop onto the xcodebuild line *after* the
    # action ("archive") per Xcode's convention. CODE_SIGNING_ALLOWED=NO is the umbrella switch;
    # CODE_SIGN_IDENTITY="" + CODE_SIGNING_REQUIRED=NO close off the codepaths that otherwise
    # still prompt Xcode to look for an identity (and fail without one).
    xcodebuild_args+=(CODE_SIGNING_ALLOWED=NO CODE_SIGNING_REQUIRED=NO CODE_SIGN_IDENTITY="")
else
    xcodebuild_args+=(DEVELOPMENT_TEAM="$team")
fi

echo "build-ios-ipa: archiving -> $archive_path (unsigned=$unsigned_archive)"
xcodebuild "${xcodebuild_args[@]}" archive

[ -d "$archive_path" ] || { echo "build-ios-ipa: archive missing at $archive_path" >&2; exit 1; }

# ---- Unsigned-archive shortcut ---------------------------------------------
# Skip exportArchive (which needs a signing identity) and pack the bare .app from inside the
# .xcarchive as a Payload/-style zip. Result is a valid .ipa shape — it won't install on a real
# device without resigning, but it's the standard "sideload-ready archive" and what altstore /
# sideloadly / Apple's own "Resign and install" tooling expect to see. Sanity-check the .app
# the archive produced before packing so a regression in the archive step shows here, not at
# .ipa unzip time.
if [ "$unsigned_archive" -eq 1 ]; then
    app_in_archive=$(find "$archive_path/Products/Applications" -maxdepth 1 -name '*.app' -type d -print -quit)
    [ -n "$app_in_archive" ] || { echo "build-ios-ipa: no .app in $archive_path/Products/Applications" >&2; exit 1; }

    payload_dir=$(mktemp -d -t octarine-ipa.XXXXXX)
    trap 'rm -rf "$payload_dir"' EXIT
    mkdir -p "$payload_dir/Payload"
    cp -R "$app_in_archive" "$payload_dir/Payload/"

    mkdir -p "$(dirname "$output")"
    rm -f "$output"
    (cd "$payload_dir" && zip -qr "$output" Payload)
    echo "build-ios-ipa: wrote $output (unsigned, $(du -h "$output" | cut -f1))"
    exit 0
fi

# ---- Export -----------------------------------------------------------------

# ExportOptions.plist drives xcodebuild -exportArchive. Per Apple's docs the
# `method` key is the primary switch; signingStyle=manual + provisioningProfiles
# would be needed for unattended CI signing, but the common interactive case
# (Xcode-managed profiles in the user's keychain) works with method+team alone.
echo "build-ios-ipa: writing $export_plist (method=$method)"
cat > "$export_plist" <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>method</key>
    <string>$method</string>
    <key>teamID</key>
    <string>$team</string>
    <key>signingStyle</key>
    <string>automatic</string>
    <key>stripSwiftSymbols</key>
    <true/>
    <key>compileBitcode</key>
    <false/>
</dict>
</plist>
EOF

rm -rf "$export_dir"
mkdir -p "$export_dir"

echo "build-ios-ipa: exporting .ipa -> $export_dir"
xcodebuild \
    -exportArchive \
    -archivePath "$archive_path" \
    -exportOptionsPlist "$export_plist" \
    -exportPath "$export_dir"

# xcodebuild names the .ipa after the scheme's product name; we configure with
# add_executable(OctarineEngine ...) so it lands as OctarineEngine.ipa. Glob
# defensively in case the project rename ever desyncs from the script.
ipa_path=$(find "$export_dir" -maxdepth 1 -name '*.ipa' -print -quit)
[ -n "$ipa_path" ] || { echo "build-ios-ipa: no .ipa produced in $export_dir" >&2; exit 1; }

mkdir -p "$(dirname "$output")"
cp "$ipa_path" "$output"
echo "build-ios-ipa: wrote $output ($(du -h "$output" | cut -f1))"
