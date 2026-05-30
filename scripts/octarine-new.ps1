#Requires -Version 5.1
<#
.SYNOPSIS
    Scaffold a new Octarine project (project.ini + config.ini + game.lua + asset dirs).

.DESCRIPTION
    Mirrors scripts/octarine-new.sh. Drop-in for Windows-native dev environments where
    bash isn't on PATH. Produces a directory the engine can run directly
    (OctarineEngine <dir>) and that 'cmake --preset ship-release' can package.

.EXAMPLE
    .\scripts\octarine-new.ps1 -Name "My Game" -PackageId com.studio.mygame -Dir C:\src\mygame
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)][string]$Name,
    [Parameter(Mandatory = $true)][string]$PackageId,
    [Parameter(Mandatory = $true)][string]$Dir,
    [string]$VersionName = "0.1.0",
    [int]$VersionCode = 1
)

$ErrorActionPreference = "Stop"

if ($PackageId -notmatch '^[a-z][a-z0-9_]*(\.[a-z][a-z0-9_]*)+$') {
    throw "-PackageId must be reverse-DNS (e.g. com.studio.mygame): got '$PackageId'"
}

if (Test-Path $Dir) {
    $item = Get-Item -LiteralPath $Dir
    if (-not $item.PSIsContainer) {
        throw "-Dir exists and is a file, not a directory: $Dir"
    }
    if ((Get-ChildItem -LiteralPath $Dir -Force | Measure-Object).Count -gt 0) {
        throw "-Dir already exists and is not empty: $Dir"
    }
} else {
    New-Item -ItemType Directory -Path $Dir | Out-Null
}

New-Item -ItemType Directory -Force -Path (Join-Path $Dir "assets") | Out-Null
New-Item -ItemType Directory -Force -Path (Join-Path $Dir "THIRD_PARTY_LICENSES.d") | Out-Null

$projectIni = @"
# Game identity for packaging. Read by desktop CPack, Android Gradle, iOS Info.plist (later).
# Single source of truth across every platform. Flat key=value, no sections.

name         = $Name
package_id   = $PackageId
version_name = $VersionName
version_code = $VersionCode
"@

$configIni = @"
# Engine runtime config. Loaded at startup before game.lua.
Title=$Name
LogLevel=info
StartupScript=game.lua
DefaultScalingMode=nearest
DefaultWindowWidth=1280
DefaultWindowHeight=720
"@

$gameLua = @"
-- $Name — entry point loaded via config.ini StartupScript.
log("Hello from $Name!")
"@

$licensesReadme = @'
Drop additional third-party attribution files (one .txt per dependency) into this
directory. Packaged builds append them to the engine's shipped THIRD_PARTY_LICENSES.txt.
'@

$gitignore = @'
# Engine-generated runtime/editor state
imgui.ini
editor_prefs.ini
preferences.ini
*_prefs.ini

# Asset bake outputs + caches
asset_manifest.lua
*.meta.cache

# Local build dirs
build/
.cache/
'@

$readme = @"
# $Name

Built with the Octarine engine.

## Run (dev)

``````sh
OctarineEngine $Dir
``````

## Package a release

``````sh
cmake --preset ship-release -DOCTARINE_PACKAGE_PROJECT=$Dir
cmake --build --preset ship-release --target package
``````

Output bundles land under ``build/ship-release/``.
"@

# Windows PowerShell 5.1's `Set-Content -Encoding utf8` writes a BOM, which the engine's
# INI parser and Lua's loader both reject. Use the .NET API w/ a no-BOM UTF8 encoder.
$utf8NoBom = New-Object System.Text.UTF8Encoding($false)
function Write-Text([string]$Path, [string]$Content) {
    [System.IO.File]::WriteAllText($Path, $Content, $utf8NoBom)
}

Write-Text (Join-Path $Dir "project.ini")                       $projectIni
Write-Text (Join-Path $Dir "config.ini")                        $configIni
Write-Text (Join-Path $Dir "game.lua")                          $gameLua
Write-Text (Join-Path $Dir "THIRD_PARTY_LICENSES.d/README.txt") $licensesReadme
Write-Text (Join-Path $Dir ".gitignore")                        $gitignore
Write-Text (Join-Path $Dir "README.md")                         $readme

Write-Output "Scaffolded '$Name' at $Dir"
