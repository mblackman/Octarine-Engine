<#
.SYNOPSIS
  One-command change validator (PowerShell mirror of octarine-verify.sh).

.DESCRIPTION
  Runs the same gates CI runs, in dependency order, and prints an unambiguous final verdict.
  Pinned to the editor-release preset + OCTARINE_ENABLE_TESTS=ON so local green == CI green.

  Exit codes: 0 ok | 2 setup | 10 configure | 11 build | 12 ctest | 13 bake | 14 drift |
              15 clang-format | 16 clang-tidy (only with -TidyStrict)

.PARAMETER Mode
  fast (default): configure-if-needed -> build test targets -> ctest -> clang-format check.
  full: also build the engine, headless bake smoke, Lua-API drift check, clang-tidy.

.PARAMETER Env
  OCT_VERIFY_GAME: path to a game dir (with config.ini) for the bake smoke.
#>
[CmdletBinding()]
param(
  [ValidateSet('fast', 'full')] [string]$Mode = 'fast',
  [switch]$KeepGoing,
  [switch]$NoBuild,
  [switch]$FormatOnly,
  [switch]$TidyStrict
)

$ErrorActionPreference = 'Continue'
$preset = 'editor-release'
$configDir = 'release'

$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
Set-Location $repoRoot
$buildDir = Join-Path $repoRoot "build/$preset"
$binary = Join-Path $buildDir "bin/$configDir/OctarineEngine.exe"

$script:FirstFailCode = 0
$script:FirstFailName = ''

function Write-Verdict {
  if ($script:FirstFailCode -eq 0) { Write-Host "VERIFY OK ($Mode)" }
  else { Write-Error "VERIFY FAIL: $($script:FirstFailName) [$($script:FirstFailCode)]" }
}

function Invoke-Fail([int]$Code, [string]$Name) {
  Write-Error "  -> FAIL: $Name [$Code]"
  if ($script:FirstFailCode -eq 0) { $script:FirstFailCode = $Code; $script:FirstFailName = $Name }
  if (-not $KeepGoing) { Write-Verdict; exit $script:FirstFailCode }
}

function Get-ChangedSources {
  git rev-parse --git-dir *> $null
  if ($LASTEXITCODE -eq 0) {
    git diff --name-only --diff-filter=ACMR HEAD -- 'src/*' 'tests/*' |
      Where-Object { $_ -match '\.(cpp|h|hpp)$' }
  }
  else {
    Get-ChildItem -Recurse -Path src, tests -Include *.cpp, *.h, *.hpp -File |
      ForEach-Object { $_.FullName }
  }
}

function Test-Format {
  if (-not (Get-Command clang-format -ErrorAction SilentlyContinue)) {
    Write-Warning '  -> SKIP clang-format (not on PATH)'; return
  }
  $files = @(Get-ChangedSources)
  if ($files.Count -eq 0) { Write-Host '  ok   clang-format (no changed sources)'; return }
  & clang-format --dry-run --Werror -style=file @files
  if ($LASTEXITCODE -eq 0) { Write-Host "  ok   clang-format ($($files.Count) files)" }
  else { Write-Warning "  (fix with: cmake --build $buildDir --target format)"; Invoke-Fail 15 'clang-format' }
}

function Test-Tidy {
  if (-not (Get-Command clang-tidy -ErrorAction SilentlyContinue)) {
    Write-Warning '  -> SKIP clang-tidy (not on PATH)'; return
  }
  if (-not (Test-Path (Join-Path $buildDir 'compile_commands.json'))) {
    Write-Warning '  -> SKIP clang-tidy (no compile_commands.json)'; return
  }
  $files = @(Get-ChangedSources | Where-Object { $_ -match '\.cpp$' })
  if ($files.Count -eq 0) { Write-Host '  ok   clang-tidy (no changed .cpp files)'; return }
  & clang-tidy -p $buildDir @files
  if ($LASTEXITCODE -eq 0) { Write-Host "  ok   clang-tidy ($($files.Count) files)" }
  elseif ($TidyStrict) { Invoke-Fail 16 'clang-tidy' }
  else { Write-Warning '  -> clang-tidy reported findings (advisory; pass -TidyStrict to gate)' }
}

if ($FormatOnly) { Write-Host '== clang-format (changed files) =='; Test-Format; Write-Verdict; exit $script:FirstFailCode }

$testTargets = @(
  'OctarineLuaApiTest', 'OctarineAssetPipelineTest', 'OctarineProcessTest', 'OctarineProjectIniTest',
  'OctarineEcsTest', 'OctarineEcsHierarchyTest', 'OctarineSystemLogicTest', 'OctarineEventBusTest'
)

if (-not $NoBuild) {
  if (-not (Test-Path (Join-Path $buildDir 'CMakeCache.txt'))) {
    Write-Host "== configure ($preset) =="
    cmake --preset $preset -DOCTARINE_ENABLE_TESTS=ON
    if ($LASTEXITCODE -ne 0) { Invoke-Fail 10 'configure' }
  }
  Write-Host '== build =='
  $buildTargets = $testTargets
  if ($Mode -eq 'full') { $buildTargets += 'OctarineEngine' }
  $targetArgs = $buildTargets | ForEach-Object { '--target', $_ }
  cmake --build $buildDir @targetArgs --parallel
  if ($LASTEXITCODE -ne 0) { Invoke-Fail 11 'build' }
}

Write-Host '== ctest =='
ctest --test-dir $buildDir --output-on-failure
if ($LASTEXITCODE -ne 0) { Invoke-Fail 12 'ctest' }

if ($Mode -eq 'full') {
  Write-Host '== bake smoke =='
  $gamePath = if ($env:OCT_VERIFY_GAME) { $env:OCT_VERIFY_GAME } else { Join-Path $repoRoot '../Octarine-Engine-Example' }
  if (-not (Test-Path $gamePath)) {
    $cloneDir = Join-Path $repoRoot 'build/.verify_game'
    if (-not (Test-Path $cloneDir)) {
      git clone --depth 1 https://github.com/mblackman/Octarine-Engine-Example.git $cloneDir
      if ($LASTEXITCODE -ne 0) { Invoke-Fail 2 'bake-setup' }
    }
    else { git -C $cloneDir pull | Out-Null }
    $gamePath = $cloneDir
  }
  if (Test-Path $gamePath) {
    $gamePath = (Resolve-Path $gamePath).Path
    if (-not (Test-Path $binary)) { Write-Error "  -> engine binary not found at $binary"; Invoke-Fail 13 'bake' }
    else {
      $env:SDL_VIDEODRIVER = 'dummy'; $env:SDL_AUDIODRIVER = 'dummy'
      & $binary $gamePath -m bake
      $manifest = Join-Path $gamePath 'asset_manifest.lua'
      if ($LASTEXITCODE -eq 0 -and (Test-Path $manifest) -and (Get-Item $manifest).Length -gt 0) {
        Write-Host '  ok   bake produced a non-empty manifest'
      }
      else { Invoke-Fail 13 'bake' }
    }
  }

  Write-Host '== Lua API drift =='
  $driftPaths = @('lua_api.smoke.lua', 'components.json', 'modules.json')
  git diff --quiet -- $driftPaths
  if ($LASTEXITCODE -eq 0) { Write-Host '  ok   no Lua API artifact drift' }
  else { Write-Error '  -> regenerated stubs differ — commit them'; git diff --stat -- $driftPaths; Invoke-Fail 14 'lua-api-drift' }

  Write-Host '== clang-tidy (changed files) =='
  Test-Tidy
}

Write-Host '== clang-format (changed files) =='
Test-Format

Write-Verdict
exit $script:FirstFailCode
