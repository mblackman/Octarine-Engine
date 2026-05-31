#!/usr/bin/env bash
# Scaffold per-project build scripts into <project>/scripts/.
# Same scripts CI runs: build-android (gradle), build-desktop (cmake ship-release + cpack),
# play (launch engine pointed at project). Both bash and PowerShell flavors emitted so a project
# scaffolded on Windows runs unmodified on Linux CI and vice versa.
#
# Usage:
#   octarine-init-build.sh --project-dir <path> [--engine-dir <path>] [--force]
#
# The emitted scripts default the engine repo path to OCTARINE_ENGINE_DIR (env) or
# <project>/../Octarine-Engine (sibling layout). Override with --engine-dir on the
# generated script.
set -euo pipefail

usage() {
  cat <<'EOF'
Usage: octarine-init-build.sh --project-dir <path> [--engine-dir <path>] [--force]

Required:
  --project-dir   Path to project (must contain project.ini)

Optional:
  --engine-dir    Engine repo path baked into emitted scripts as fallback default
                  (default: <project>/../Octarine-Engine relative resolution at run time).
  --force         Overwrite existing emitted scripts. Default skips files that already exist.
  -h, --help      Show this message
EOF
}

project_dir=""
engine_dir=""
force=0

while [[ $# -gt 0 ]]; do
  case "$1" in
    --project-dir) project_dir="$2"; shift 2 ;;
    --engine-dir)  engine_dir="$2"; shift 2 ;;
    --force)       force=1; shift ;;
    -h|--help)     usage; exit 0 ;;
    *) echo "unknown arg: $1" >&2; usage >&2; exit 2 ;;
  esac
done

[[ -z "$project_dir" ]] && { echo "--project-dir required" >&2; exit 2; }
[[ -d "$project_dir" ]] || { echo "--project-dir not a directory: $project_dir" >&2; exit 1; }
[[ -f "$project_dir/project.ini" ]] || { echo "no project.ini at $project_dir/project.ini" >&2; exit 1; }

# Minimal project.ini parse: name + package_id required (matches Gradle/CMake validators).
read_ini() {
  local key="$1" file="$2"
  # grep first line matching `<key> = ...`, strip leading `key = ` and trailing whitespace.
  grep -E "^[[:space:]]*${key}[[:space:]]*=" "$file" \
    | head -n1 \
    | sed -E "s/^[[:space:]]*${key}[[:space:]]*=[[:space:]]*//; s/[[:space:]]+$//"
}

name="$(read_ini name "$project_dir/project.ini")"
package_id="$(read_ini package_id "$project_dir/project.ini")"
[[ -z "$name" ]]       && { echo "project.ini: missing 'name'" >&2; exit 1; }
[[ -z "$package_id" ]] && { echo "project.ini: missing 'package_id'" >&2; exit 1; }

out_dir="$project_dir/scripts"
mkdir -p "$out_dir"

write_if() {
  local target="$1"
  if [[ -e "$target" && $force -eq 0 ]]; then
    echo "skip (exists): $target"
    return 1
  fi
  return 0
}

mark_exec() {
  chmod +x "$1" 2>/dev/null || true
}

engine_default_sh='${OCTARINE_ENGINE_DIR:-$(cd "$here/../../Octarine-Engine" 2>/dev/null && pwd)}'
engine_default_ps1='if ($env:OCTARINE_ENGINE_DIR) { $env:OCTARINE_ENGINE_DIR } else { (Resolve-Path (Join-Path $here "..\..\Octarine-Engine") -ErrorAction SilentlyContinue).Path }'

# -- build-android.sh ---------------------------------------------------------
target="$out_dir/build-android.sh"
if write_if "$target"; then
  cat > "$target" <<EOF
#!/usr/bin/env bash
# Build the Android AAB (release) or APK (debug) for $name.
# Calls the engine repo's gradlew with -Poctarine.projectDir pointing at this project.
# Mirrors .github/workflows/android.yml. Signing creds come from env (never project.ini).
set -euo pipefail

here="\$(cd "\$(dirname "\$0")" && pwd)"
project_dir="\$(cd "\$here/.." && pwd)"
engine_dir="\${OCTARINE_ENGINE_DIR:-${engine_dir:-}}"
[[ -z "\$engine_dir" ]] && engine_dir="$engine_default_sh"
[[ -z "\$engine_dir" || ! -d "\$engine_dir/android" ]] && { echo "engine dir not found; set OCTARINE_ENGINE_DIR" >&2; exit 1; }

mode="\${1:-release}"  # release | debug
shift || true

cd "\$engine_dir/android"
args=(-Poctarine.projectDir="\$project_dir" --no-daemon --console=plain)
case "\$mode" in
  release)
    args=(bundleRelease "\${args[@]}" -Poctarine.abis=arm64-v8a,x86_64)
    if [[ -n "\${OCTARINE_ANDROID_KEYSTORE_PATH:-}" ]]; then
      args+=(-Poctarine.storeFile="\$OCTARINE_ANDROID_KEYSTORE_PATH"
             -Poctarine.storePassword="\${OCTARINE_ANDROID_STORE_PASSWORD:-}"
             -Poctarine.keyAlias="\${OCTARINE_ANDROID_KEY_ALIAS:-}"
             -Poctarine.keyPassword="\${OCTARINE_ANDROID_KEY_PASSWORD:-}")
    fi
    ;;
  debug)
    args=(assembleDebug "\${args[@]}")
    ;;
  *) echo "unknown mode: \$mode (expected release|debug)" >&2; exit 2 ;;
esac
[[ -n "\${OCTARINE_VERSION_NAME:-}" ]] && args+=(-Poctarine.versionName="\$OCTARINE_VERSION_NAME")
[[ -n "\${OCTARINE_VERSION_CODE:-}" ]] && args+=(-Poctarine.versionCode="\$OCTARINE_VERSION_CODE")

./gradlew "\${args[@]}" "\$@"

if [[ "\$mode" == "release" ]]; then
  echo "AAB: \$engine_dir/android/app/build/outputs/bundle/release/app-release.aab"
else
  echo "APK: \$engine_dir/android/app/build/outputs/apk/debug/app-debug.apk"
fi
EOF
  mark_exec "$target"
  echo "wrote: $target"
fi

# -- build-android.ps1 --------------------------------------------------------
target="$out_dir/build-android.ps1"
if write_if "$target"; then
  cat > "$target" <<EOF
# Build the Android AAB (release) or APK (debug) for $name.
# Parity with build-android.sh.
[CmdletBinding()]
param(
  [ValidateSet('release','debug')][string]\$Mode = 'release',
  [Parameter(ValueFromRemainingArguments=\$true)][string[]]\$Extra
)
\$ErrorActionPreference = 'Stop'

\$here = Split-Path -Parent \$MyInvocation.MyCommand.Path
\$projectDir = (Resolve-Path (Join-Path \$here '..')).Path
\$engineDir = if (\$env:OCTARINE_ENGINE_DIR) { \$env:OCTARINE_ENGINE_DIR } else { '${engine_dir:-}' }
if (-not \$engineDir) { \$engineDir = $engine_default_ps1 }
if (-not \$engineDir -or -not (Test-Path (Join-Path \$engineDir 'android'))) {
  Write-Error "engine dir not found; set OCTARINE_ENGINE_DIR"; exit 1
}

Push-Location (Join-Path \$engineDir 'android')
try {
  \$args = @("-Poctarine.projectDir=\$projectDir", '--no-daemon', '--console=plain')
  if (\$Mode -eq 'release') {
    \$args = @('bundleRelease') + \$args + @('-Poctarine.abis=arm64-v8a,x86_64')
    if (\$env:OCTARINE_ANDROID_KEYSTORE_PATH) {
      \$args += @(
        "-Poctarine.storeFile=\$env:OCTARINE_ANDROID_KEYSTORE_PATH",
        "-Poctarine.storePassword=\$env:OCTARINE_ANDROID_STORE_PASSWORD",
        "-Poctarine.keyAlias=\$env:OCTARINE_ANDROID_KEY_ALIAS",
        "-Poctarine.keyPassword=\$env:OCTARINE_ANDROID_KEY_PASSWORD")
    }
  } else {
    \$args = @('assembleDebug') + \$args
  }
  if (\$env:OCTARINE_VERSION_NAME) { \$args += "-Poctarine.versionName=\$env:OCTARINE_VERSION_NAME" }
  if (\$env:OCTARINE_VERSION_CODE) { \$args += "-Poctarine.versionCode=\$env:OCTARINE_VERSION_CODE" }
  if (\$Extra) { \$args += \$Extra }

  \$gradlew = if (\$IsWindows) { '.\\gradlew.bat' } else { './gradlew' }
  & \$gradlew @args
  if (\$LASTEXITCODE -ne 0) { exit \$LASTEXITCODE }

  if (\$Mode -eq 'release') {
    Write-Host "AAB: \$engineDir/android/app/build/outputs/bundle/release/app-release.aab"
  } else {
    Write-Host "APK: \$engineDir/android/app/build/outputs/apk/debug/app-debug.apk"
  }
} finally { Pop-Location }
EOF
  echo "wrote: $target"
fi

# -- build-desktop.sh ---------------------------------------------------------
target="$out_dir/build-desktop.sh"
if write_if "$target"; then
  cat > "$target" <<EOF
#!/usr/bin/env bash
# Build a shippable desktop package for $name via the engine's ship-release preset + cpack.
# Mirrors .github/workflows/package.yml. Output lands in <engine>/build/ship-release/.
set -euo pipefail

here="\$(cd "\$(dirname "\$0")" && pwd)"
project_dir="\$(cd "\$here/.." && pwd)"
engine_dir="\${OCTARINE_ENGINE_DIR:-${engine_dir:-}}"
[[ -z "\$engine_dir" ]] && engine_dir="$engine_default_sh"
[[ -z "\$engine_dir" || ! -f "\$engine_dir/CMakeLists.txt" ]] && { echo "engine dir not found; set OCTARINE_ENGINE_DIR" >&2; exit 1; }

preset="\${OCTARINE_PRESET:-ship-release}"

cd "\$engine_dir"
extra=()
[[ -n "\${OCTARINE_VERSION_NAME:-}" ]] && extra+=(-DOCTARINE_PACKAGE_VERSION_NAME="\$OCTARINE_VERSION_NAME")
[[ -n "\${OCTARINE_VERSION_CODE:-}" ]] && extra+=(-DOCTARINE_PACKAGE_VERSION_CODE="\$OCTARINE_VERSION_CODE")

cmake --preset "\$preset" -DOCTARINE_PACKAGE_PROJECT="\$project_dir" "\${extra[@]}"
cmake --build "build/\$preset" --config Release --parallel
cmake --build "build/\$preset" --target package

echo "Packages in: \$engine_dir/build/\$preset/"
ls -la "\$engine_dir/build/\$preset/" | grep -Ei '\\.(zip|tar\\.gz|dmg)\$' || true
EOF
  mark_exec "$target"
  echo "wrote: $target"
fi

# -- build-desktop.ps1 --------------------------------------------------------
target="$out_dir/build-desktop.ps1"
if write_if "$target"; then
  cat > "$target" <<EOF
# Build a shippable desktop package for $name via ship-release preset + cpack.
[CmdletBinding()]
param([string]\$Preset)
\$ErrorActionPreference = 'Stop'
if (-not \$Preset) { \$Preset = if (\$env:OCTARINE_PRESET) { \$env:OCTARINE_PRESET } else { 'ship-release' } }

\$here = Split-Path -Parent \$MyInvocation.MyCommand.Path
\$projectDir = (Resolve-Path (Join-Path \$here '..')).Path
\$engineDir = if (\$env:OCTARINE_ENGINE_DIR) { \$env:OCTARINE_ENGINE_DIR } else { '${engine_dir:-}' }
if (-not \$engineDir) { \$engineDir = $engine_default_ps1 }
if (-not \$engineDir -or -not (Test-Path (Join-Path \$engineDir 'CMakeLists.txt'))) {
  Write-Error "engine dir not found; set OCTARINE_ENGINE_DIR"; exit 1
}

Push-Location \$engineDir
try {
  \$extra = @()
  if (\$env:OCTARINE_VERSION_NAME) { \$extra += "-DOCTARINE_PACKAGE_VERSION_NAME=\$env:OCTARINE_VERSION_NAME" }
  if (\$env:OCTARINE_VERSION_CODE) { \$extra += "-DOCTARINE_PACKAGE_VERSION_CODE=\$env:OCTARINE_VERSION_CODE" }

  & cmake --preset \$Preset "-DOCTARINE_PACKAGE_PROJECT=\$projectDir" @extra
  if (\$LASTEXITCODE -ne 0) { exit \$LASTEXITCODE }
  & cmake --build "build/\$Preset" --config Release --parallel
  if (\$LASTEXITCODE -ne 0) { exit \$LASTEXITCODE }
  & cmake --build "build/\$Preset" --target package
  if (\$LASTEXITCODE -ne 0) { exit \$LASTEXITCODE }

  Write-Host "Packages in: \$engineDir/build/\$Preset/"
  Get-ChildItem "\$engineDir/build/\$Preset/" -File |
    Where-Object { \$_.Extension -in '.zip','.gz','.dmg' } |
    Format-Table Name, Length
} finally { Pop-Location }
EOF
  echo "wrote: $target"
fi

# -- play.sh ------------------------------------------------------------------
target="$out_dir/play.sh"
if write_if "$target"; then
  cat > "$target" <<EOF
#!/usr/bin/env bash
# Launch the engine binary against $name for a quick dev play.
# Resolves the binary from OCTARINE_ENGINE_BIN or, if unset, the engine repo's editor-debug build.
set -euo pipefail

here="\$(cd "\$(dirname "\$0")" && pwd)"
project_dir="\$(cd "\$here/.." && pwd)"
engine_dir="\${OCTARINE_ENGINE_DIR:-${engine_dir:-}}"
[[ -z "\$engine_dir" ]] && engine_dir="$engine_default_sh"

bin="\${OCTARINE_ENGINE_BIN:-}"
if [[ -z "\$bin" ]]; then
  for candidate in \\
    "\$engine_dir/build/editor-debug/bin/debug/OctarineEngine" \\
    "\$engine_dir/build/editor-debug/bin/OctarineEngine" \\
    "\$engine_dir/build/player-debug/bin/debug/OctarineEngine" \\
    "\$engine_dir/build/player-debug/bin/OctarineEngine"
  do
    if [[ -x "\$candidate" ]]; then bin="\$candidate"; break; fi
  done
fi
[[ -z "\$bin" ]] && { echo "no engine binary; build editor-debug or set OCTARINE_ENGINE_BIN" >&2; exit 1; }

exec "\$bin" "\$project_dir" "\$@"
EOF
  mark_exec "$target"
  echo "wrote: $target"
fi

# -- play.ps1 -----------------------------------------------------------------
target="$out_dir/play.ps1"
if write_if "$target"; then
  cat > "$target" <<EOF
# Launch the engine binary against $name for a quick dev play.
[CmdletBinding()]
param([Parameter(ValueFromRemainingArguments=\$true)][string[]]\$Extra)
\$ErrorActionPreference = 'Stop'

\$here = Split-Path -Parent \$MyInvocation.MyCommand.Path
\$projectDir = (Resolve-Path (Join-Path \$here '..')).Path
\$engineDir = if (\$env:OCTARINE_ENGINE_DIR) { \$env:OCTARINE_ENGINE_DIR } else { '${engine_dir:-}' }
if (-not \$engineDir) { \$engineDir = $engine_default_ps1 }

\$bin = \$env:OCTARINE_ENGINE_BIN
if (-not \$bin) {
  \$candidates = @(
    "\$engineDir/build/editor-debug/bin/debug/OctarineEngine.exe",
    "\$engineDir/build/editor-debug/bin/OctarineEngine.exe",
    "\$engineDir/build/player-debug/bin/debug/OctarineEngine.exe",
    "\$engineDir/build/player-debug/bin/OctarineEngine.exe",
    "\$engineDir/build/editor-debug/bin/debug/OctarineEngine",
    "\$engineDir/build/editor-debug/bin/OctarineEngine"
  )
  \$bin = \$candidates | Where-Object { Test-Path \$_ } | Select-Object -First 1
}
if (-not \$bin) { Write-Error "no engine binary; build editor-debug or set OCTARINE_ENGINE_BIN"; exit 1 }

& \$bin \$projectDir @Extra
exit \$LASTEXITCODE
EOF
  echo "wrote: $target"
fi

# -- scripts/README.md --------------------------------------------------------
target="$out_dir/README.md"
if write_if "$target"; then
  cat > "$target" <<EOF
# $name — build scripts

Generated by \`octarine-init-build\`. Same scripts CI runs; edit freely.

## Quick run

| Script | Does |
|--------|------|
| \`play.sh\` / \`play.ps1\` | Launch engine pointed at this project (dev) |
| \`build-desktop.sh\` / \`.ps1\` | \`cmake --preset ship-release\` + \`cpack\` for the host OS |
| \`build-android.sh\` / \`.ps1\` | \`gradlew bundleRelease\` (default) or \`debug\` mode for an APK |

## Required environment

| Var | Used by | Notes |
|-----|---------|-------|
| \`OCTARINE_ENGINE_DIR\` | all | Engine repo path. Fallback: \`<project>/../Octarine-Engine\`. |
| \`OCTARINE_ENGINE_BIN\` | play | Explicit binary path; otherwise searched under engine \`build/editor-debug\` / \`build/player-debug\`. |
| \`OCTARINE_PRESET\` | build-desktop | Override default \`ship-release\` preset. |
| \`OCTARINE_VERSION_NAME\` / \`OCTARINE_VERSION_CODE\` | build-* | Override identity at build time. Falls through to \`project.ini\`. |
| \`OCTARINE_ANDROID_KEYSTORE_PATH\` + \`_STORE_PASSWORD\` + \`_KEY_ALIAS\` + \`_KEY_PASSWORD\` | build-android (release) | Signing creds. Never commit. Release falls back to the debug key if unset. |

## Regenerate

Re-run the engine's scaffolder:

\`\`\`sh
"\$OCTARINE_ENGINE_DIR/scripts/octarine-init-build.sh" --project-dir "\$(pwd)" [--force]
\`\`\`

Without \`--force\`, existing files are preserved (so hand-edits survive).
EOF
  echo "wrote: $target"
fi

echo "Done. Scripts under: $out_dir"
