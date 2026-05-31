#Requires -Version 5.1
<#
.SYNOPSIS
    Scaffold per-project build scripts into <project>/scripts/.

.DESCRIPTION
    Parity with scripts/octarine-init-build.sh. Emits both bash and PowerShell flavors of
    build-android, build-desktop, and play, plus a scripts/README.md. Same scripts CI runs.

.EXAMPLE
    .\scripts\octarine-init-build.ps1 -ProjectDir C:\src\mygame
    .\scripts\octarine-init-build.ps1 -ProjectDir C:\src\mygame -Force
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)][string]$ProjectDir,
    [string]$EngineDir,
    [switch]$Force
)

$ErrorActionPreference = "Stop"

if (-not (Test-Path -LiteralPath $ProjectDir -PathType Container)) {
    throw "-ProjectDir not a directory: $ProjectDir"
}
$projectIniPath = Join-Path $ProjectDir 'project.ini'
if (-not (Test-Path -LiteralPath $projectIniPath)) {
    throw "no project.ini at $projectIniPath"
}

function Read-IniKey {
    param([string]$Path, [string]$Key)
    foreach ($line in Get-Content -LiteralPath $Path) {
        $trim = $line.Trim()
        if ($trim -eq '' -or $trim.StartsWith('#')) { continue }
        $idx = $trim.IndexOf('=')
        if ($idx -lt 0) { continue }
        $k = $trim.Substring(0, $idx).Trim()
        $v = $trim.Substring($idx + 1).Trim()
        if ($k -eq $Key) { return $v }
    }
    return ''
}

$Name      = Read-IniKey -Path $projectIniPath -Key 'name'
$PackageId = Read-IniKey -Path $projectIniPath -Key 'package_id'
if (-not $Name)      { throw "project.ini: missing 'name'" }
if (-not $PackageId) { throw "project.ini: missing 'package_id'" }

$outDir = Join-Path $ProjectDir 'scripts'
New-Item -ItemType Directory -Force -Path $outDir | Out-Null

$engineDefaultLiteral = if ($EngineDir) { $EngineDir } else { '' }

function Write-IfMissing {
    param([string]$Target, [string]$Content)
    if ((Test-Path -LiteralPath $Target) -and -not $Force) {
        Write-Host "skip (exists): $Target"
        return $false
    }
    Set-Content -LiteralPath $Target -Value $Content -Encoding UTF8 -NoNewline:$false
    Write-Host "wrote: $Target"
    return $true
}

# Generated-file bodies. Use single-quoted here-strings + manual placeholder substitution
# so PowerShell does not eat $var references intended for the emitted scripts.
function Expand-Template {
    param([string]$Template, [hashtable]$Map)
    foreach ($k in $Map.Keys) {
        $Template = $Template.Replace("@@$k@@", [string]$Map[$k])
    }
    return $Template
}

$buildAndroidSh = @'
#!/usr/bin/env bash
# Build the Android AAB (release) or APK (debug) for @@NAME@@.
# Calls the engine repo's gradlew with -Poctarine.projectDir pointing at this project.
# Mirrors .github/workflows/android.yml. Signing creds come from env (never project.ini).
set -euo pipefail

here="$(cd "$(dirname "$0")" && pwd)"
project_dir="$(cd "$here/.." && pwd)"
engine_dir="${OCTARINE_ENGINE_DIR:-@@ENGINE_DEFAULT@@}"
[[ -z "$engine_dir" ]] && engine_dir="$(cd "$here/../../Octarine-Engine" 2>/dev/null && pwd)"
[[ -z "$engine_dir" || ! -d "$engine_dir/android" ]] && { echo "engine dir not found; set OCTARINE_ENGINE_DIR" >&2; exit 1; }

mode="${1:-release}"
shift || true

cd "$engine_dir/android"
args=(-Poctarine.projectDir="$project_dir" --no-daemon --console=plain)
case "$mode" in
  release)
    args=(bundleRelease "${args[@]}" -Poctarine.abis=arm64-v8a,x86_64)
    if [[ -n "${OCTARINE_ANDROID_KEYSTORE_PATH:-}" ]]; then
      args+=(-Poctarine.storeFile="$OCTARINE_ANDROID_KEYSTORE_PATH"
             -Poctarine.storePassword="${OCTARINE_ANDROID_STORE_PASSWORD:-}"
             -Poctarine.keyAlias="${OCTARINE_ANDROID_KEY_ALIAS:-}"
             -Poctarine.keyPassword="${OCTARINE_ANDROID_KEY_PASSWORD:-}")
    fi
    ;;
  debug)
    args=(assembleDebug "${args[@]}")
    ;;
  *) echo "unknown mode: $mode (expected release|debug)" >&2; exit 2 ;;
esac
[[ -n "${OCTARINE_VERSION_NAME:-}" ]] && args+=(-Poctarine.versionName="$OCTARINE_VERSION_NAME")
[[ -n "${OCTARINE_VERSION_CODE:-}" ]] && args+=(-Poctarine.versionCode="$OCTARINE_VERSION_CODE")

./gradlew "${args[@]}" "$@"

if [[ "$mode" == "release" ]]; then
  echo "AAB: $engine_dir/android/app/build/outputs/bundle/release/app-release.aab"
else
  echo "APK: $engine_dir/android/app/build/outputs/apk/debug/app-debug.apk"
fi
'@

$buildAndroidPs1 = @'
# Build the Android AAB (release) or APK (debug) for @@NAME@@.
[CmdletBinding()]
param(
  [ValidateSet('release','debug')][string]$Mode = 'release',
  [Parameter(ValueFromRemainingArguments=$true)][string[]]$Extra
)
$ErrorActionPreference = 'Stop'

$here = Split-Path -Parent $MyInvocation.MyCommand.Path
$projectDir = (Resolve-Path (Join-Path $here '..')).Path
$engineDir = if ($env:OCTARINE_ENGINE_DIR) { $env:OCTARINE_ENGINE_DIR } else { '@@ENGINE_DEFAULT@@' }
if (-not $engineDir) {
  $engineDir = (Resolve-Path (Join-Path $here '..\..\Octarine-Engine') -ErrorAction SilentlyContinue).Path
}
if (-not $engineDir -or -not (Test-Path (Join-Path $engineDir 'android'))) {
  Write-Error "engine dir not found; set OCTARINE_ENGINE_DIR"; exit 1
}

Push-Location (Join-Path $engineDir 'android')
try {
  $args = @("-Poctarine.projectDir=$projectDir", '--no-daemon', '--console=plain')
  if ($Mode -eq 'release') {
    $args = @('bundleRelease') + $args + @('-Poctarine.abis=arm64-v8a,x86_64')
    if ($env:OCTARINE_ANDROID_KEYSTORE_PATH) {
      $args += @(
        "-Poctarine.storeFile=$env:OCTARINE_ANDROID_KEYSTORE_PATH",
        "-Poctarine.storePassword=$env:OCTARINE_ANDROID_STORE_PASSWORD",
        "-Poctarine.keyAlias=$env:OCTARINE_ANDROID_KEY_ALIAS",
        "-Poctarine.keyPassword=$env:OCTARINE_ANDROID_KEY_PASSWORD")
    }
  } else {
    $args = @('assembleDebug') + $args
  }
  if ($env:OCTARINE_VERSION_NAME) { $args += "-Poctarine.versionName=$env:OCTARINE_VERSION_NAME" }
  if ($env:OCTARINE_VERSION_CODE) { $args += "-Poctarine.versionCode=$env:OCTARINE_VERSION_CODE" }
  if ($Extra) { $args += $Extra }

  $gradlew = if ($IsWindows) { '.\gradlew.bat' } else { './gradlew' }
  & $gradlew @args
  if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

  if ($Mode -eq 'release') {
    Write-Host "AAB: $engineDir/android/app/build/outputs/bundle/release/app-release.aab"
  } else {
    Write-Host "APK: $engineDir/android/app/build/outputs/apk/debug/app-debug.apk"
  }
} finally { Pop-Location }
'@

$buildDesktopSh = @'
#!/usr/bin/env bash
# Build a shippable desktop package for @@NAME@@ via the engine's ship-release preset + cpack.
# Mirrors .github/workflows/package.yml. Output lands in <engine>/build/ship-release/.
set -euo pipefail

here="$(cd "$(dirname "$0")" && pwd)"
project_dir="$(cd "$here/.." && pwd)"
engine_dir="${OCTARINE_ENGINE_DIR:-@@ENGINE_DEFAULT@@}"
[[ -z "$engine_dir" ]] && engine_dir="$(cd "$here/../../Octarine-Engine" 2>/dev/null && pwd)"
[[ -z "$engine_dir" || ! -f "$engine_dir/CMakeLists.txt" ]] && { echo "engine dir not found; set OCTARINE_ENGINE_DIR" >&2; exit 1; }

preset="${OCTARINE_PRESET:-ship-release}"

cd "$engine_dir"
extra=()
[[ -n "${OCTARINE_VERSION_NAME:-}" ]] && extra+=(-DOCTARINE_PACKAGE_VERSION_NAME="$OCTARINE_VERSION_NAME")
[[ -n "${OCTARINE_VERSION_CODE:-}" ]] && extra+=(-DOCTARINE_PACKAGE_VERSION_CODE="$OCTARINE_VERSION_CODE")

cmake --preset "$preset" -DOCTARINE_PACKAGE_PROJECT="$project_dir" "${extra[@]}"
cmake --build "build/$preset" --config Release --parallel
cmake --build "build/$preset" --target package

echo "Packages in: $engine_dir/build/$preset/"
ls -la "$engine_dir/build/$preset/" | grep -Ei '\.(zip|tar\.gz|dmg)$' || true
'@

$buildDesktopPs1 = @'
# Build a shippable desktop package for @@NAME@@ via ship-release preset + cpack.
[CmdletBinding()]
param([string]$Preset)
$ErrorActionPreference = 'Stop'
if (-not $Preset) { $Preset = if ($env:OCTARINE_PRESET) { $env:OCTARINE_PRESET } else { 'ship-release' } }

$here = Split-Path -Parent $MyInvocation.MyCommand.Path
$projectDir = (Resolve-Path (Join-Path $here '..')).Path
$engineDir = if ($env:OCTARINE_ENGINE_DIR) { $env:OCTARINE_ENGINE_DIR } else { '@@ENGINE_DEFAULT@@' }
if (-not $engineDir) {
  $engineDir = (Resolve-Path (Join-Path $here '..\..\Octarine-Engine') -ErrorAction SilentlyContinue).Path
}
if (-not $engineDir -or -not (Test-Path (Join-Path $engineDir 'CMakeLists.txt'))) {
  Write-Error "engine dir not found; set OCTARINE_ENGINE_DIR"; exit 1
}

Push-Location $engineDir
try {
  $extra = @()
  if ($env:OCTARINE_VERSION_NAME) { $extra += "-DOCTARINE_PACKAGE_VERSION_NAME=$env:OCTARINE_VERSION_NAME" }
  if ($env:OCTARINE_VERSION_CODE) { $extra += "-DOCTARINE_PACKAGE_VERSION_CODE=$env:OCTARINE_VERSION_CODE" }

  & cmake --preset $Preset "-DOCTARINE_PACKAGE_PROJECT=$projectDir" @extra
  if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
  & cmake --build "build/$Preset" --config Release --parallel
  if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
  & cmake --build "build/$Preset" --target package
  if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

  Write-Host "Packages in: $engineDir/build/$Preset/"
  Get-ChildItem "$engineDir/build/$Preset/" -File |
    Where-Object { $_.Extension -in '.zip','.gz','.dmg' } |
    Format-Table Name, Length
} finally { Pop-Location }
'@

$playSh = @'
#!/usr/bin/env bash
# Launch the engine binary against @@NAME@@ for a quick dev play.
set -euo pipefail

here="$(cd "$(dirname "$0")" && pwd)"
project_dir="$(cd "$here/.." && pwd)"
engine_dir="${OCTARINE_ENGINE_DIR:-@@ENGINE_DEFAULT@@}"
[[ -z "$engine_dir" ]] && engine_dir="$(cd "$here/../../Octarine-Engine" 2>/dev/null && pwd)"

bin="${OCTARINE_ENGINE_BIN:-}"
if [[ -z "$bin" ]]; then
  for candidate in \
    "$engine_dir/build/editor-debug/bin/debug/OctarineEngine" \
    "$engine_dir/build/editor-debug/bin/OctarineEngine" \
    "$engine_dir/build/player-debug/bin/debug/OctarineEngine" \
    "$engine_dir/build/player-debug/bin/OctarineEngine"
  do
    if [[ -x "$candidate" ]]; then bin="$candidate"; break; fi
  done
fi
[[ -z "$bin" ]] && { echo "no engine binary; build editor-debug or set OCTARINE_ENGINE_BIN" >&2; exit 1; }

exec "$bin" "$project_dir" "$@"
'@

$playPs1 = @'
# Launch the engine binary against @@NAME@@ for a quick dev play.
[CmdletBinding()]
param([Parameter(ValueFromRemainingArguments=$true)][string[]]$Extra)
$ErrorActionPreference = 'Stop'

$here = Split-Path -Parent $MyInvocation.MyCommand.Path
$projectDir = (Resolve-Path (Join-Path $here '..')).Path
$engineDir = if ($env:OCTARINE_ENGINE_DIR) { $env:OCTARINE_ENGINE_DIR } else { '@@ENGINE_DEFAULT@@' }
if (-not $engineDir) {
  $engineDir = (Resolve-Path (Join-Path $here '..\..\Octarine-Engine') -ErrorAction SilentlyContinue).Path
}

$bin = $env:OCTARINE_ENGINE_BIN
if (-not $bin) {
  $candidates = @(
    "$engineDir/build/editor-debug/bin/debug/OctarineEngine.exe",
    "$engineDir/build/editor-debug/bin/OctarineEngine.exe",
    "$engineDir/build/player-debug/bin/debug/OctarineEngine.exe",
    "$engineDir/build/player-debug/bin/OctarineEngine.exe",
    "$engineDir/build/editor-debug/bin/debug/OctarineEngine",
    "$engineDir/build/editor-debug/bin/OctarineEngine"
  )
  $bin = $candidates | Where-Object { Test-Path $_ } | Select-Object -First 1
}
if (-not $bin) { Write-Error "no engine binary; build editor-debug or set OCTARINE_ENGINE_BIN"; exit 1 }

& $bin $projectDir @Extra
exit $LASTEXITCODE
'@

$readme = @'
# @@NAME@@ — build scripts

Generated by `octarine-init-build`. Same scripts CI runs; edit freely.

## Quick run

| Script | Does |
|--------|------|
| `play.sh` / `play.ps1` | Launch engine pointed at this project (dev) |
| `build-desktop.sh` / `.ps1` | `cmake --preset ship-release` + `cpack` for the host OS |
| `build-android.sh` / `.ps1` | `gradlew bundleRelease` (default) or `debug` mode for an APK |

## Required environment

| Var | Used by | Notes |
|-----|---------|-------|
| `OCTARINE_ENGINE_DIR` | all | Engine repo path. Fallback: `<project>/../Octarine-Engine`. |
| `OCTARINE_ENGINE_BIN` | play | Explicit binary path; otherwise searched under engine `build/editor-debug` / `build/player-debug`. |
| `OCTARINE_PRESET` | build-desktop | Override default `ship-release` preset. |
| `OCTARINE_VERSION_NAME` / `OCTARINE_VERSION_CODE` | build-* | Override identity at build time. Falls through to `project.ini`. |
| `OCTARINE_ANDROID_KEYSTORE_PATH` + `_STORE_PASSWORD` + `_KEY_ALIAS` + `_KEY_PASSWORD` | build-android (release) | Signing creds. Never commit. Release falls back to the debug key if unset. |

## Regenerate

Re-run the engine's scaffolder:

```sh
"$OCTARINE_ENGINE_DIR/scripts/octarine-init-build.sh" --project-dir "$(pwd)" [--force]
```

Without `--force`, existing files are preserved (so hand-edits survive).
'@

$map = @{ NAME = $Name; ENGINE_DEFAULT = $engineDefaultLiteral }

$files = @(
    @{ Path = (Join-Path $outDir 'build-android.sh');  Content = (Expand-Template $buildAndroidSh  $map) },
    @{ Path = (Join-Path $outDir 'build-android.ps1'); Content = (Expand-Template $buildAndroidPs1 $map) },
    @{ Path = (Join-Path $outDir 'build-desktop.sh');  Content = (Expand-Template $buildDesktopSh  $map) },
    @{ Path = (Join-Path $outDir 'build-desktop.ps1'); Content = (Expand-Template $buildDesktopPs1 $map) },
    @{ Path = (Join-Path $outDir 'play.sh');           Content = (Expand-Template $playSh          $map) },
    @{ Path = (Join-Path $outDir 'play.ps1');          Content = (Expand-Template $playPs1         $map) },
    @{ Path = (Join-Path $outDir 'README.md');         Content = (Expand-Template $readme          $map) }
)

foreach ($f in $files) {
    Write-IfMissing -Target $f.Path -Content $f.Content | Out-Null
}

Write-Host "Done. Scripts under: $outDir"
