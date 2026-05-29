# Device Builds

End-to-end reference for producing shippable artifacts on every platform Octarine
targets: Windows, Linux, macOS (per-arch and universal), Android (APK + AAB), and
iOS (simulator `.app` + device `.ipa`, signed or unsigned).

This document covers:

- The conceptual model — how the same code path produces a runnable artifact on
  every platform.
- How to build each artifact locally, by platform.
- How CI builds them for every push.
- How identity (name, bundle id, version) flows from a single `project.ini`
  file into every platform's package metadata.
- How icons / splash screens are generated from one source PNG.
- Signing and distribution prerequisites.
- Troubleshooting the most common failure modes.

If you just want to ship something today, jump to **Quick start** at the bottom.

---

## 1. The model

Every device build is the same engine binary plus the same staged game project,
packed in the platform's native bundle shape:

```
<package or bundle>/
    OctarineEngine[.exe / .app / libmain.so]   # engine binary, per platform
    asset_manifest.lua                  # baked at package time — id -> relative path + metadata
    config.ini                          # GameConfig
    game.lua                            # startup script
    fonts/ images/ sounds/ scripts/ …   # whatever the project ships
```

`SDL_GetBasePath()` returns the location of that bundle on every platform
(the exe dir on desktop, `Contents/Resources/` inside a `.app` on macOS, the
`.app` root on iOS, and the APK asset root via SDL's `AAssetManager` shim on
Android). The engine resolves *every* asset path against this base, so the
same C++ code works in all four environments without per-platform branches in
the asset loaders.

The two pieces that make this work end to end:

1. **`OCTARINE_SHIPPED`** is a build-time define. When on, the engine loads
   `asset_manifest.lua` instead of scanning the filesystem. The scan code
   path is not viable inside a read-only APK or `.app`, so every device build
   must set this. The `ship-release` CMake preset sets it; Android and iOS
   force it on inside `CMakeLists.txt`; `octarine_package` re-forces it on the
   packaged target as a guardrail.
2. **`asset_manifest.lua`** is produced by `OctarineEngine <project> -m bake`.
   The bake runs the project headless, executes its startup script to gather
   every referenced asset id, validates each one, and emits a dofile-able Lua
   table with paths relative to the project root. On desktop CPack runs this
   as an install-time step — a broken asset reference fails the package. On
   Android and iOS the bake cannot run on the host (cross-compiled binary
   can't execute), so the **project commits its `asset_manifest.lua`** or
   passes a host-native binary via `-Poctarine.bakeExe=` / CMake
   `-DOCTARINE_HOST_BAKE_EXE=`.

Read `CLAUDE.md` for the broader engine architecture; this doc is strictly
about how the build system reaches a shippable artifact.

---

## 2. Project identity — one file, every platform

Every game project ships a flat key=value `project.ini` at its root:

```ini
name         = My Game
package_id   = com.studio.mygame   # reverse-DNS; Android/iOS bundle id, NSIS publisher
version_name = 0.3.1               # CFBundleShortVersionString, AGP versionName
version_code = 12                  # CFBundleVersion, AGP versionCode (integer)
vendor       = Studio Name         # MACOSX_BUNDLE_COPYRIGHT, CPack vendor
description  = Short tagline.      # MACOSX_BUNDLE_INFO_STRING, CPack description
icon         = images/icon.png     # 1024x1024 PNG, project-relative (see § 4)
splash_color = #0a0a18             # iOS LaunchScreen background; defaults to #000000
```

The same file drives:

- `cmake/OctarinePackage.cmake` (`octarine_read_project_ini` + `_octarine_resolve_identity`)
  — desktop CPack and the iOS `.app` Info.plist.
- `android/app/build.gradle` (`identityProp` helper) — Android `applicationId`,
  `versionCode`, `versionName`, launcher label.
- `scripts/octarine-icons.cmake` — generates icons and splash assets from
  `icon=` / `splash_color=`.

**Precedence is the same in every driver: CLI flag > `project.ini` > built-in
default.** This lets CI bump a hotfix version with one flag without touching
the file. The CLI flag names mirror each other:

| Identity key  | CMake CLI                            | Gradle CLI                |
|---------------|--------------------------------------|---------------------------|
| `name`        | `-DOCTARINE_PACKAGE_NAME`            | `-Poctarine.appName`      |
| `package_id`  | `-DOCTARINE_PACKAGE_ID`              | `-Poctarine.applicationId`|
| `version_name`| `-DOCTARINE_PACKAGE_VERSION_NAME`    | `-Poctarine.versionName`  |
| `version_code`| `-DOCTARINE_PACKAGE_VERSION_CODE`    | `-Poctarine.versionCode`  |
| `vendor`      | `-DOCTARINE_PACKAGE_VENDOR`          | _(unused)_                |
| `description` | `-DOCTARINE_PACKAGE_DESCRIPTION`     | _(unused)_                |

A shipping build with no `project.ini` warns and falls through to the example
identity (handy for bootstrap). A `project.ini` that exists but is missing a
required key or has a malformed `package_id` (must be reverse-DNS) **fails at
configure time** on both CMake and Gradle. Same validation rules in both
drivers — see `_octarine_validate_identity` in `OctarinePackage.cmake` and the
mirror block at the top of `app/build.gradle`.

---

## 3. The bake step — `asset_manifest.lua`

```
OctarineEngine <project-dir> -m bake
```

Headless run that:

1. Builds the asset catalog by recursively scanning `<project-dir>`.
2. Runs the project's startup script (`config.ini` `StartupScript=`).
3. Scenes drive `acquire_scene_assets` / `load_asset`, both of which validate
   their references against the catalog instead of uploading textures.
4. Writes `<project-dir>/asset_manifest.lua` — one row per asset, with the
   `file` path relative to the project root.

**Exit code is nonzero** if any reference is unresolved. This is the CI gate
for shipping builds. The desktop CPack path runs this at install time inside
`cmake --build … --target package`, so a broken reference fails the package
rather than the runtime.

On Android and iOS the bake cannot run on the host (cross-compiled binary).
Options:

- **Commit the manifest** to the project repo. Cheap, always works, matches
  what most games already do.
- **Pass `-Poctarine.bakeExe=…` (Gradle) or `-DOCTARINE_HOST_BAKE_EXE=…` (CMake
  iOS)** pointing at a host-native `OctarineEngine` binary. Gradle's
  `stageOctarineAssets` runs it as a `doLast`; iOS adds a `PRE_BUILD` step
  before the bundle is signed.

If neither holds, iOS fails at CMake configure (`FATAL_ERROR`) with a clear
message rather than producing a `.app` that dies at first scene load.

The `--use-manifest` CLI flag forces a non-shipped engine binary to load the
manifest, for verifying the manifest-load branch from a dev build.

---

## 4. Icons + splash

Set `icon=path/to/icon.png` (1024×1024 PNG) and optionally
`splash_color=#rrggbb` in `project.ini`. The shared
`scripts/octarine-icons.cmake` produces every platform's icon ladder + splash
metadata from that single input:

- **Android:** `mipmap-mdpi/hdpi/xhdpi/xxhdpi/xxxhdpi/ic_launcher.png` (legacy
  square), `mipmap-*/ic_launcher_foreground.png` + `mipmap-anydpi-v26/
  ic_launcher.xml` (Android 8+ adaptive), and `values-v31/octarine_splash.xml`
  wiring `windowSplashScreenAnimatedIcon` / `windowSplashScreenBackground`
  for the Android 12+ splash API.
- **iOS:** `Assets.xcassets/AppIcon.appiconset/` with the full 20/29/40/60/76/
  83.5/1024 ladder, and a `LaunchScreen.storyboard` whose background uses
  `splash_color` (default `#000000`).
- **Desktop:** Windows `.ico` (NSIS installer + EXE), macOS `.icns` (placed in
  `Contents/Resources/`, referenced via `CFBundleIconFile`), Linux `.png`
  (placed beside the binary for `.desktop` `Icon=` consumption).

**ImageMagick is required.** Install `magick` (or the older `convert`) on
`PATH`. With ImageMagick missing or `icon=` unset, the script skip-warns and
the build ships platform defaults: Android keeps the green-robot launcher,
iOS gets no icon (Xcode warns) and a black launch screen, desktop ships no
icon. This is fine for early bootstrap projects but is **not Apple App
Store-acceptable** — Apple rejects submissions without an `AppIcon` set.

Adaptive icons crop the outer ~16% of the source PNG. Leave safe padding so
the cropped circle on Android matches the legacy square on Android 7.

---

## 5. Desktop — Windows / Linux / macOS

Shipping config = `ship-release` preset. It sets `OCTARINE_SHIPPED=ON` plus
the no-editor / no-ImGui / no-profiling flags. The macOS universal variant is
`ship-mac-universal` (arm64 + x86_64 via the `universal-osx` overlay triplet —
macOS host only).

### One-shot package

```bash
# From the engine repo root, against a sibling game project:
cmake --preset ship-release -DOCTARINE_PACKAGE_PROJECT=/path/to/MyGame
cmake --build build/ship-release --config Release --parallel
cmake --build build/ship-release --target package
```

Output lands at:

- **Windows:** `build/ship-release/<name>-<version>-win64.zip`
  (and an NSIS installer if you set `-DOCTARINE_PACKAGE_NSIS=ON` and have a
  working `makensis` on `PATH`)
- **Linux:** `build/ship-release/<name>-<version>-Linux.tar.gz`
- **macOS:** `build/ship-release/<name>-<version>-Darwin.dmg`
- **macOS universal:** same, but configure with `--preset ship-mac-universal`
  and the artifact lands under `build/ship-mac-universal/`.

The `package` target chains: bake the manifest into the staged project (the
CI gate), copy the staged project beside the binary, bundle runtime libs +
the MSVC CRT (Windows) or transitive dylibs (macOS) / `.so`s (Linux),
configure CPack, run CPack. See `cmake/OctarinePackage.cmake` for the
orchestration; the function entry point is `octarine_package(<target>
PROJECT <dir>)` and the wiring is gated on the top-level
`OCTARINE_PACKAGE_PROJECT` cache variable.

### What's in the bundle

- **Windows ZIP**: flat layout, engine `.exe` + every transitive DLL vcpkg
  app-local-deployed (SDL3, image/mixer/ttf, freetype, libpng, jpeg, zlib, …)
  + MSVC runtime DLLs sourced from the compiler's own redist (NOT the
  ambient VS install). The CRT DLLs ship beside the EXE so Windows' loader
  prefers them over older copies in `System32`.
- **Linux TGZ**: flat layout. With the default static `x64-linux` triplet,
  no `.so`s ride along (vcpkg produces archives that the engine links
  statically). With a dynamic triplet or `SHARED` SDL3, dylibs end up beside
  the binary and `INSTALL_RPATH=$ORIGIN` points the loader at them.
- **macOS DMG**: `<TargetName>.app/Contents/MacOS/<binary>` + project at
  `Contents/Resources/`. vcpkg dylibs (if any) get bundled into
  `Contents/Frameworks/`, with `INSTALL_RPATH=@executable_path/../Frameworks`.

### Verify locally

```bash
# Unpack/mount the artifact in a fresh dir and run with NO arguments:
./OctarineEngine
# Expected log line:
# AssetCatalog: baked manifest found, loading (scan skipped)
# AssetCatalog: loaded N entries from manifest
```

If the engine logs `scanning filesystem` instead, `OCTARINE_SHIPPED` didn't
reach the compile — check the preset and that you used `octarine_package`.

### NSIS installer (Windows, opt-in)

```bash
cmake --preset ship-release -DOCTARINE_PACKAGE_PROJECT=/path/to/MyGame \
      -DOCTARINE_PACKAGE_NSIS=ON
cmake --build build/ship-release --target package
```

NSIS is **opt-in, not auto-detected**: a stray non-functional `makensis` on
`PATH` (e.g. a Chocolatey shim) would otherwise make plain `cpack` fail
before producing the reliable ZIP. Only flip the flag on a machine with a
working NSIS install.

---

## 6. Android — APK + AAB

### Prerequisites

| Tool          | Pinned version                                              |
|---------------|-------------------------------------------------------------|
| JDK           | 17 (AGP 8.7.3)                                              |
| Android SDK   | `platforms;android-35`, `build-tools;35.0.0`                |
| Android NDK   | `ndk;28.2.13676358` — must match `ndkVersion` in `build.gradle` |
| CMake         | `cmake;3.22.1` (SDK-managed) or ≥ 3.15 on `PATH`            |
| vcpkg         | `VCPKG_ROOT` env var pointing at the checkout               |
| ImageMagick   | `magick`/`convert` on `PATH`, only if `icon=` is set        |

`ANDROID_NDK_HOME` is required, not optional. vcpkg's port builds use their
own android toolchain that finds the NDK *only* through this env var; the
`VCPKG_CHAINLOAD_TOOLCHAIN_FILE` that `build.gradle` passes covers the
engine's own configure but not vcpkg's dependency builds. Without it the
build dies early with `Could not find android ndk`.

Full install + env setup is in `android/README.md`. The short PowerShell version:

```powershell
$sdk = "$env:LOCALAPPDATA\Android\Sdk"
& "$sdk\cmdline-tools\latest\bin\sdkmanager.bat" `
  "platform-tools" "platforms;android-35" "build-tools;35.0.0" `
  "ndk;28.2.13676358" "cmake;3.22.1"
[Environment]::SetEnvironmentVariable("ANDROID_HOME", $sdk, "User")
[Environment]::SetEnvironmentVariable("ANDROID_NDK_HOME", "$sdk\ndk\28.2.13676358", "User")
```

### Build it

```powershell
cd android
.\gradlew.bat assembleDebug `
  -Poctarine.projectDir=C:\path\to\MyGame
# -> app\build\outputs\apk\debug\app-debug.apk
```

Release variants:

```powershell
.\gradlew.bat assembleRelease -Poctarine.projectDir=…   # signed APK
.\gradlew.bat bundleRelease   -Poctarine.projectDir=…   # signed AAB (Play upload)
```

First run builds every vcpkg android dep from source (SDL3, freetype, lua,
…). Allow 20–60 minutes cold; subsequent runs hit vcpkg's cache.

### How it works

The host app under `android/` is a thin wrapper:

- `app/build.gradle` invokes `externalNativeBuild` against the **repo-root
  `CMakeLists.txt`**, building target `OctarineEngine` as a `SHARED` library
  called `libmain.so`. The `if (ANDROID)` block in `CMakeLists.txt` forces
  `OCTARINE_SHIPPED=ON`, drops `-march=native` / `-Werror`, links `android`
  + `log`, and skips the desktop CPack code paths.
- vcpkg android triplets are static, so SDL3 + image/mixer/ttf statically
  link into `libmain.so`. There is no separate `libSDL3.so` —
  `SDLActivity.getLibraries()` returns only `{"main"}`.
- `stageOctarineAssets` copies the game project (excluding scratch like
  `*.meta`, `editor_prefs.ini`, `preferences.ini`, `imgui.ini`, `.git`) into
  the APK assets dir, where SDL's `AAssetManager` shim resolves it via
  `SDL_IOFromFile`. The committed `asset_manifest.lua` rides along.
- `generateOctarineIcons` runs `scripts/octarine-icons.cmake` to produce
  `mipmap-*` + adaptive XML + the Android 12 splash theme.

### Per-project knobs (`-Poctarine.*`)

| Property                       | Default                             | Notes                                       |
|--------------------------------|-------------------------------------|---------------------------------------------|
| `-Poctarine.projectDir`        | sibling `Octarine-Engine-Example`   | Project staged into the APK assets.         |
| `-Poctarine.abi`               | `arm64-v8a`                         | Target ABI; **triplet auto-derives**.       |
| `-Poctarine.abis`              | _unset_                             | Comma-list for multi-ABI shipping AAB.      |
| `-Poctarine.triplet`           | derived from `abi`                  | vcpkg triplet override.                     |
| `-Poctarine.minSdk`            | `28`                                | Floor pinned by the vcpkg triplet API level.|
| `-Poctarine.ndkVersion`        | `28.2.13676358`                     | Must match `ndkVersion` and installed NDK.  |
| `-Poctarine.bakeExe`           | _unset_                             | Host-native `OctarineEngine` to re-bake.    |
| `-Poctarine.vcpkgRoot`         | `$env:VCPKG_ROOT`                   | Fallback if the env var is unset.           |
| `-Poctarine.minify`            | `false`                             | Probe R8 (keeps `proguard-rules.pro`).      |
| `-Poctarine.storeFile`         | _unset → debug key_                 | Release keystore. See § 6.4.                |
| `-Poctarine.storePassword`     |                                     |                                             |
| `-Poctarine.keyAlias`          |                                     |                                             |
| `-Poctarine.keyPassword`       |                                     |                                             |
| Identity overrides             | see § 2                             | `-Poctarine.applicationId/appName/version*` |

### Emulator note

Default ABI is `arm64-v8a` (physical arm64 phones). Stock desktop emulators
are `x86_64` and won't run an arm64 APK. One-flag change:

```powershell
.\gradlew.bat assembleDebug -Poctarine.abi=x86_64
# triplet auto-derives to x64-android
```

Some emulator images run arm64 binaries through Berberis (the Pixel images
do); on those the default arm64-v8a APK is fine.

### Multi-ABI AAB (shipping)

The shipping AAB carries three ABIs so Play can deliver per-device split
APKs. AGP's `externalNativeBuild` can only carry one vcpkg triplet per
variant, so multi-ABI bypasses it with custom tasks that fan out per-ABI
cmake invocations and stage each `libmain.so` under
`build/octarineJni/<abi>/`. Trigger with `-Poctarine.abis`:

```powershell
.\gradlew.bat bundleRelease `
  -Poctarine.projectDir=C:\path\to\MyGame `
  -Poctarine.abis=arm64-v8a,armeabi-v7a,x86_64 `
  -Poctarine.storeFile=… (etc)
```

`bundle { abi { enableSplit = true } }` in `app/build.gradle` is on by
default, so Play delivers per-ABI APKs from the same AAB. Single-ABI mode
keeps the IDE-friendly `externalNativeBuild` path.

### Release signing

Play Store requires an `.aab` signed with an **upload key** consistent across
every update — losing the keystore means you can never update that listing
again. Treat it like a secret with no expiry: back it up the day you
generate it.

Generate once:

```powershell
keytool -genkeypair -v `
  -keystore octarine-upload.jks `
  -alias octarine-upload `
  -keyalg RSA -keysize 2048 -validity 9125 `
  -storetype JKS
```

Local signed build — put the four `-Poctarine.*` properties in
`~/.gradle/gradle.properties` (gitignored, machine-scoped):

```properties
octarine.storeFile=C:\\absolute\\path\\to\\octarine-upload.jks
octarine.storePassword=…
octarine.keyAlias=octarine-upload
octarine.keyPassword=…
```

Verify the signing cert with `apksigner verify --print-certs`:

```powershell
& "$env:LOCALAPPDATA\Android\Sdk\build-tools\35.0.0\apksigner.bat" verify --print-certs `
  app\build\outputs\bundle\release\app-release.aab
```

CI signing — set GitHub Actions secrets `OCTARINE_ANDROID_KEYSTORE` (base64
of the `.jks`), `OCTARINE_ANDROID_STORE_PASSWORD`,
`OCTARINE_ANDROID_KEY_ALIAS`, `OCTARINE_ANDROID_KEY_PASSWORD`. The
`android.yml` workflow decodes the keystore per run and forwards the
properties to Gradle. With the secrets unset (fork PRs), `bundleRelease`
falls back to the debug key — still a useful build-passes signal, not
Play-uploadable.

### R8 / minification

`minifyEnabled = false` on the release build type by default.
`proguard-rules.pro` already keeps the SDL JNI surface plus a safety-net
`native <methods>;` rule, so flipping it on is a probe rather than a leap of
faith. Run with `-Poctarine.minify=true` once, spot-check the stripped JNI
symbols survived (`llvm-nm -D libmain.so | grep Java_org_libsdl_app_`), and
re-enable per project once startup works on a real device. Size win is
modest — only the Java side shrinks; SDL3 + engine are in `libmain.so`,
untouched.

### sol2 / NDK clang workaround

`build.gradle` passes `-DVCPKG_OVERLAY_PORTS=vcpkg-overlay-ports`, which
overrides the stock `sol2` port. The NDK r28 clang (19.0.1) hits
[LLVM #91362](https://github.com/llvm/llvm-project/issues/91362) when binding
sol2 usertype member variables, failing the whole scripting surface. The
overlay re-installs sol2 with the (optimization-only) `noexcept(…)` stripped
from 10 call sites. One-time sol2 rebuild on first configure; nothing else
changes. Desktop builds don't use the overlay.

Do **not** toggle `SOL_NOEXCEPT_FUNCTION_TYPE=0` instead — desyncs sol2's
traits from `__cpp_noexcept_function_type` and breaks differently.

---

## 7. iOS — simulator `.app` + device `.ipa`

### Prerequisites

- macOS host (Xcode toolchain). Linux/Windows cannot cross-compile iOS.
- Xcode 15.4+ (`sudo xcode-select -s /Applications/Xcode.app`).
- Autotools chain for vcpkg ports (`brew install autoconf automake
  autoconf-archive libtool pkg-config`). freetype / harfbuzz / libvorbis
  configure scripts need them.
- `VCPKG_ROOT` env var pointing at the vcpkg checkout.
- ImageMagick on `PATH` if `icon=` is set.

For a **signed device build** you additionally need (see § 7.4):

- Apple Developer Program membership ($99/yr).
- An installed signing identity (`.p12`) in your keychain.
- A provisioning profile matching the `package_id`.

### Presets

- `ship-ios-simulator` — Xcode generator, `CMAKE_SYSTEM_NAME=iOS`,
  `CMAKE_OSX_SYSROOT=iphonesimulator`, triplet `arm64-ios-simulator` (on
  Intel macs swap to `x64-ios-simulator` + `CMAKE_OSX_ARCHITECTURES=x86_64`).
- `ship-ios-device` — same shape but `iphoneos` sysroot, `arm64-ios` triplet.

Both inherit from the hidden `ios-base` preset which sets the shipping
flags + `CMAKE_OSX_DEPLOYMENT_TARGET=14.0`.

### Simulator `.app`

```bash
cmake --preset ship-ios-simulator \
      -DOCTARINE_PACKAGE_PROJECT=/path/to/MyGame
cmake --build build/ship-ios-simulator --config Release -- CODE_SIGNING_ALLOWED=NO
```

Output: `build/ship-ios-simulator/Release-iphonesimulator/<Target>.app`.
Install + launch in a booted simulator:

```bash
xcrun simctl boot "iPhone 16"
xcrun simctl install booted build/ship-ios-simulator/Release-iphonesimulator/OctarineEngine.app
xcrun simctl launch booted com.studio.mygame
xcrun simctl spawn booted log stream --predicate 'subsystem == "com.octarine.engine"'
```

iOS routes engine logs through `os_log` (subsystem `com.octarine.engine`,
categories `Octarine` / `OctarineLua`) since there's no terminal attached to
stdout. Watch them in Console.app or via `log stream` as above.

### Device `.ipa` — `scripts/build-ios-ipa.sh`

This script wraps the `xcodebuild archive` + `-exportArchive` pipeline that
turns the device-target `.app` into a distributable `.ipa`. CMake/CPack
doesn't do this — `xcodebuild` owns archiving on iOS.

```bash
scripts/build-ios-ipa.sh \
  --project /path/to/MyGame \
  --team   ABCDE12345 \
  --method development \
  --output build/MyGame.ipa
```

Flags:

| Flag                       | Purpose                                                                   |
|----------------------------|---------------------------------------------------------------------------|
| `--project DIR` *(req)*    | Game project root.                                                        |
| `--team ID`                | Apple Developer team id; required unless `--unsigned-archive`.            |
| `--identity STR`           | `CODE_SIGN_IDENTITY` (e.g. `Apple Distribution: Studio (TEAMID)`).        |
| `--method M`               | `development` *(default)* / `ad-hoc` / `app-store` / `enterprise`.        |
| `--build-dir DIR`          | Default: `build/ship-ios-device`.                                         |
| `--output PATH`            | Final `.ipa` destination.                                                 |
| `--host-bake-exe PATH`     | Host-native `OctarineEngine` for the PRE_BUILD asset bake.                |
| `--skip-configure`         | Re-archive without re-running CMake configure (faster iteration).         |
| `--unsigned-archive`       | Skip `exportArchive`; pack `Payload/<App>.app` from the `.xcarchive`.     |
| `--name / --id / --version / --version-code / --vendor / --description` | Override `project.ini` identity. |

Env fallbacks mirror the flags: `OCTARINE_IOS_TEAM`, `OCTARINE_IOS_IDENTITY`,
`OCTARINE_IOS_METHOD`, `OCTARINE_PROJECT`, `OCTARINE_HOST_BAKE_EXE`.

#### Unsigned archive — what it's for

`--unsigned-archive` is a CI smoke test: it runs the full `ship-ios-device`
configure + the `xcodebuild archive` action with `CODE_SIGNING_ALLOWED=NO`,
then copies `Products/Applications/<App>.app` out of the `.xcarchive` and
zips it into a `Payload/`-shaped `.ipa`. The result is a valid `.ipa`
**shape** that will not install on a real device without resigning — but it
proves the device-target compile/link/archive end-to-end without any Apple
secrets. The `ios-ipa.yml` CI leg runs it on every PR.

To produce a signed device `.ipa`, drop `--unsigned-archive` and pass
`--team`. The script:

1. Configures the `ship-ios-device` preset (octarine_package wires
   `Info.plist`, stages the project, sets the codesign passthrough).
2. Runs `xcodebuild archive` → `.xcarchive`.
3. Writes an `ExportOptions.plist` with the chosen `method` + `teamID` +
   `signingStyle=automatic`.
4. Runs `xcodebuild -exportArchive` → `.ipa` at `--output`.

Method picks who can install:

- **development** — only on team-registered UDIDs (your test devices).
- **ad-hoc** — wider internal distribution to enrolled devices.
- **app-store** — for TestFlight / App Store Connect upload.
- **enterprise** — Apple Enterprise program only.

### Bake on iOS

A cross-compiled iOS binary can't run on the build host, so the bake doesn't
happen at package time the way desktop CPack does. Two options:

- **Commit `asset_manifest.lua`** to the project repo (the same contract as
  Android). The example repo commits its manifest, gitignored on desktop
  projects, opt-in elsewhere.
- **Pass `-DOCTARINE_HOST_BAKE_EXE=/path/to/desktop/OctarineEngine`** at
  configure (or `--host-bake-exe` on the script). The iOS branch of
  `octarine_package` adds a `PRE_BUILD` step that re-bakes the staged
  project before xcodebuild signs the bundle.

If neither is true, configure fails with a clear `FATAL_ERROR` rather than
shipping a `.app` that dies at first scene load.

### Codesign passthrough — CMake cache vars

The iOS branch of `octarine_package` reads two cache vars:

- `OCTARINE_IOS_DEVELOPMENT_TEAM` → `XCODE_ATTRIBUTE_DEVELOPMENT_TEAM`
- `OCTARINE_IOS_CODE_SIGN_IDENTITY` → `XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY`

These are pure passthroughs; the dev team/identity are deployment concerns
and live outside the source tree.

### Signed CI — what's missing

`ios-ipa.yml` runs unsigned today. The signed flip is:

1. Add GitHub Actions secrets: `APPLE_TEAM_ID`, base64
   `IOS_P12` + `IOS_P12_PASSWORD`, base64 `IOS_PROVISIONING_PROFILE`.
2. Add a keychain bootstrap step that imports the `.p12` and drops the
   provisioning profile into `~/Library/MobileDevice/Provisioning Profiles/`.
3. Drop `--unsigned-archive` from the script invocation, add
   `--team "${{ secrets.APPLE_TEAM_ID }}"`.

See `ai/DeviceShippingPlan.md` § Stage 3 for the keychain bootstrap commands.

---

## 8. CI — what runs where

| Workflow                    | Runner          | Triggers                                | Output                                         |
|-----------------------------|-----------------|-----------------------------------------|------------------------------------------------|
| `.github/workflows/package.yml`  | matrix (win/linux/mac/mac-universal) | dispatch + push to `main` + tags `v*` + PRs | per-OS ZIP/TGZ/DMG                |
| `.github/workflows/android.yml`  | ubuntu-latest   | dispatch + push/PR to `main` + tags `v*` | debug APK + signed release AAB (multi-ABI)   |
| `.github/workflows/ios.yml`      | macos-latest    | dispatch + push/PR to `main`            | simulator `.app` + simctl launch assertion    |
| `.github/workflows/ios-ipa.yml`  | macos-latest    | dispatch + push/PR to `main` + tags `v*` | unsigned device `.ipa`                       |

### Common machinery

All four workflows share:

- `VCPKG_BINARY_SOURCES: "clear;x-gha,readwrite"` — vcpkg uses the GitHub
  Actions cache for binary artifacts. First-ever run for a new triplet is
  cold (~60 minutes); subsequent runs warm-hit the cache (~5–10 minutes per
  leg).
- A `Resolve version` step that converts tag pushes `v0.1.2` →
  `version_name=0.1.2`, `version_code=<git rev-list --count HEAD>`. Non-tag
  runs leave both blank and the driver falls through to `project.ini` /
  defaults. This is how a tag drives both stores' version numbers without
  editing the file.
- The example project is checked out into `example_repo/` from
  `mblackman/Octarine-Engine-Example` and used as the staged project.

### `package.yml` — desktop matrix

Per-OS leg: configures `ship-release` (or `ship-mac-universal`) against
`example_repo`, builds, runs `cmake --build … --target package`. The
install-time manifest bake inside CPack is the CI gate. macOS universal
additionally `lipo -archs` the binary out of the mounted DMG to assert both
slices made it through.

### `android.yml`

ubuntu runner: JDK 17, `android-actions/setup-android` + `sdkmanager`
installs the pinned NDK + cmake. Builds debug APK (`assembleDebug`) and
multi-ABI release AAB (`bundleRelease -Poctarine.abis=arm64-v8a,armeabi-v7a,x86_64`).
The AAB step uses the GitHub-secret-driven keystore if available, else falls
back to the debug key. A sanity step `unzip -l` the AAB and asserts
`base/lib/<abi>/libmain.so` is present for every ABI.

### `ios.yml` — simulator

macos-latest: brew autotools, `ship-ios-simulator` configure, build via
`cmake --build` with `CODE_SIGNING_ALLOWED=NO`. Sanity-checks bundle
contents, then runs the **simulator launch assertion**:

- `xcrun simctl create OctarineCI` against the first available iOS runtime
  + iPhone device type pulled from the runtime's `supportedDeviceTypes`
  (avoids picking a device that needs a newer runtime than the one
  installed).
- `boot`, `bootstatus -b`, `install`, `launch`.
- `xcrun simctl spawn booted log show --predicate 'subsystem ==
  "com.octarine.engine"'` polled up to ~90 seconds.
- Passes if both `AssetCatalog: loaded N entries from manifest` *and* an
  `AudioSystem initialized` (or graceful-degrade `MIX_…` line) appear.

This catches a class of regression a build-only leg can't see: a `.app`
that links but won't actually run (SDL_main missing, Metal renderer
failing, bundle resource resolution at `SDL_GetBasePath()` broken).

### `ios-ipa.yml` — unsigned device archive

macos-latest: identical prereqs, runs `scripts/build-ios-ipa.sh
--unsigned-archive` against the example. Sanity-check `unzip -l` the
`.ipa` and asserts `Payload/OctarineEngine.app/{config.ini,asset_manifest.lua,
game.lua}` is present. Exercises the **device-target** compile/link/archive
which the simulator leg can't reach (different sysroot, different vcpkg
triplet, Mach-O for `iphoneos` not `iphonesimulator`).

A signed device leg is one step away once codesign secrets exist; see § 7.5.

---

## 9. Verifying a build

After producing an artifact, the manifest-load log line is the single most
useful confirmation that asset resolution actually wired up. Look for:

```
AssetCatalog: baked manifest found, loading (scan skipped)
AssetCatalog: loaded N entries from manifest
```

If you see `scanning filesystem` instead, the engine compiled without
`OCTARINE_SHIPPED` defined. Inside a read-only bundle (APK / iOS `.app`)
the next scene load will fail; on desktop the binary will run but isn't
shippable (it's reading loose files from a dev tree, not the manifest).

### Platform-specific verifies

- **Desktop:** unpack to a clean dir, double-click the binary, watch the
  log. On macOS check the bundle has `Contents/MacOS/<binary>` + populated
  `Contents/Resources/`.
- **Android:** `adb install -r app-release.apk`, then `adb logcat -s SDL
  Octarine OctarineLua` while you launch. The pinned ABI must match the
  device (`adb shell getprop ro.product.cpu.abilist`).
- **iOS simulator:** `xcrun simctl spawn booted log stream --predicate
  'subsystem == "com.octarine.engine"'`.
- **iOS device (signed):** install via Xcode `Devices and Simulators` or via
  Apple Configurator; logs in Console.app filtered on the
  `com.octarine.engine` subsystem.

### Sanity checks worth running before a release

- AAB carries every ABI:
  ```bash
  unzip -l app-release.aab | grep base/lib
  # expect base/lib/arm64-v8a/libmain.so + armeabi-v7a + x86_64
  ```
- Universal macOS binary carries both slices:
  ```bash
  lipo -archs OctarineEngine.app/Contents/MacOS/OctarineEngine
  # expect: arm64 x86_64
  ```
- Android AAB signed with the upload key (not debug):
  ```powershell
  apksigner verify --print-certs app-release.aab
  ```
- iOS bundle contains the manifest + startup script:
  ```bash
  ls OctarineEngine.app/{config.ini,asset_manifest.lua,game.lua}
  ```

---

## 10. Troubleshooting

### `AssetCatalog` scanned the filesystem in a shipped build

`OCTARINE_SHIPPED` was off at compile time. Verify the preset
(`ship-release`, `ship-ios-*`) or that you called `octarine_package`
(which force-defines `OCTARINE_SHIPPED` on the packaged target).

### Desktop binary crashes with `0xC0000135` (Windows)

A transitive DLL didn't ship beside the EXE. The packaging code globs
`$<TARGET_FILE_DIR>/*.dll` to grab vcpkg's app-local copies; if you're not
using `octarine_package` (e.g. a manual install), you'll need to bundle
them yourself. `$<TARGET_RUNTIME_DLLS>` only lists *direct* imports and
misses transitive deps like freetype/libpng/jpeg/zlib.

### Desktop binary access-violates in Release (Windows MSVC)

Make sure you didn't regress the ECS chunk over-alignment in
`src/ECS/Component.h`. MSVC Release `/arch:AVX2` emits 32B-aligned
`vmovdqa` over component arrays; plain `new unsigned char[]` is only
16B-aligned and access-violates. The chunk storage uses
`::operator new[](size, std::align_val_t{64})` with matching aligned
`delete`.

### Android `Could not find android ndk` at vcpkg configure

`ANDROID_NDK_HOME` isn't set in the shell that's running `gradlew`. Set
it at User scope, **then open a fresh terminal**.

### Android APK installs but immediately closes

Likely an ABI mismatch. Default APK is arm64-only and stock desktop
emulators are x86_64. Rebuild with `-Poctarine.abi=x86_64`, or use an arm64
system image: native on Apple-silicon Macs; on x86_64 hosts (Windows / Intel
Mac / Linux) use a Berberis-backed Pixel image, which translates arm64 →
x86_64 inside the emulator.

### Android `bundleRelease` produces an unsigned AAB

The four `octarine.storeFile/storePassword/keyAlias/keyPassword` properties
weren't set. Without them the `signingConfigs.release` block isn't
instantiated; `signingConfig` then falls back to `signingConfigs.debug` so
the AAB is debug-signed (still installable, not Play-uploadable).

### Android `bundleRelease` strips JNI symbols and the app fails to find native methods

`minifyEnabled = true` got flipped on without the SDL JNI keep-rules.
`proguard-rules.pro` already has them; verify it's referenced in the
release `buildType`. Spot-check with `llvm-nm -D libmain.so | grep
Java_org_libsdl_app_`.

### iOS configure fails: `no asset_manifest.lua in project and OCTARINE_HOST_BAKE_EXE is unset`

The project doesn't ship a baked manifest and you didn't point at a
host-native binary to bake at build time. Either commit the manifest, or
pass `-DOCTARINE_HOST_BAKE_EXE=/path/to/desktop/OctarineEngine`.

### iOS `.xcarchive` has empty `Products/Applications/`

CMake-generated Xcode targets default to `SKIP_INSTALL=YES`, so the `.app`
lands in `UninstalledProducts/iphoneos/` instead. The iOS branch of
`octarine_package` sets `XCODE_ATTRIBUTE_SKIP_INSTALL=NO` +
`XCODE_ATTRIBUTE_INSTALL_PATH=/Applications` to fix this. If you're
configuring iOS without `octarine_package`, set those attributes yourself.

### vcpkg first-run is very slow

Expected — first run for a new triplet builds every dep from source.
SDL3 + image/mixer/ttf + freetype/harfbuzz/libpng/libogg/libvorbis is
~20–60 minutes. The GitHub Actions binary cache (`x-gha`) makes subsequent
CI runs ~5–10 minutes per leg; locally, set `VCPKG_BINARY_SOURCES` to a
shared `files` cache to amortize across checkouts.

### iOS simulator runs but logs are empty

iOS routes engine logs through `os_log`, not stdout. Use Console.app or
`log stream --predicate 'subsystem == "com.octarine.engine"'`. Console
filtering on the `Octarine` / `OctarineLua` categories narrows it further.

---

## 11. Quick start — "ship something today"

You have a game project at `/path/to/MyGame` with a `project.ini`, an
asset tree, and a committed `asset_manifest.lua` (run
`OctarineEngine /path/to/MyGame -m bake` once and commit the output).

**Local Windows ZIP:**

```bash
cmake --preset ship-release -DOCTARINE_PACKAGE_PROJECT=/path/to/MyGame
cmake --build build/ship-release --config Release --parallel
cmake --build build/ship-release --target package
# -> build/ship-release/MyGame-0.1.0-win64.zip
```

**Local Linux TGZ:**

```bash
cmake --preset ship-release -DOCTARINE_PACKAGE_PROJECT=/path/to/MyGame
cmake --build build/ship-release --config Release --parallel
cmake --build build/ship-release --target package
# -> build/ship-release/MyGame-0.1.0-Linux.tar.gz
```

**Local macOS DMG (universal):**

```bash
cmake --preset ship-mac-universal -DOCTARINE_PACKAGE_PROJECT=/path/to/MyGame
cmake --build build/ship-mac-universal --config Release --parallel
cmake --build build/ship-mac-universal --target package
# -> build/ship-mac-universal/MyGame-0.1.0-Darwin.dmg
```

**Local Android signed AAB:**

```powershell
cd android
.\gradlew.bat bundleRelease `
  -Poctarine.projectDir=/path/to/MyGame `
  -Poctarine.abis=arm64-v8a,armeabi-v7a,x86_64
# (with -Poctarine.storeFile/... in ~/.gradle/gradle.properties)
# -> android/app/build/outputs/bundle/release/app-release.aab
```

**Local iOS unsigned device `.ipa`:**

```bash
scripts/build-ios-ipa.sh \
  --project /path/to/MyGame \
  --unsigned-archive \
  --output build/MyGame-unsigned.ipa
```

**Local iOS signed device `.ipa`** (requires Apple Developer team + cert +
profile):

```bash
scripts/build-ios-ipa.sh \
  --project /path/to/MyGame \
  --team ABCDE12345 \
  --method development \
  --output build/MyGame.ipa
```

**Push a release tag** and CI produces the shipping artifacts:

```bash
git tag v0.1.0 && git push --tags
```

Tag push fires three workflows:

- `package.yml` — per-OS ZIP/TGZ/DMG + universal macOS DMG.
- `android.yml` — debug APK + signed multi-ABI AAB (debug-key fallback
  when the keystore secrets aren't wired).
- `ios-ipa.yml` — unsigned device `.ipa` (swap to signed once the codesign
  secrets land; see § 7.5).

`ios.yml` (simulator launch assertion) runs on push/PR to `main` only — no
tag trigger, since the simulator `.app` isn't a shipping artifact. Add
`tags: ['v*']` to its `on:` block if you want a tag-driven sim build too.

Drop the shipping artifacts into Steam / Play Store / TestFlight once
distribution automation lands (see `ai/DeviceShippingPlan.md` § Stage 6).

---

## 12. Where to look in the source

| Concern                          | File                                                  |
|----------------------------------|-------------------------------------------------------|
| CMake presets                    | `CMakePresets.json`                                   |
| Android / iOS shipping flag forces | `CMakeLists.txt` (`if (ANDROID)` / iOS blocks)      |
| Desktop + iOS packaging          | `cmake/OctarinePackage.cmake`                         |
| Icon / splash generator          | `scripts/octarine-icons.cmake`                        |
| iOS `.ipa` wrap script           | `scripts/build-ios-ipa.sh`                            |
| Android host app                 | `android/app/build.gradle`, `android/README.md`       |
| Asset bake mode                  | `Game::Bake` + `acquire_scene_assets` / `load_asset`  |
| Manifest load gate               | `AssetCatalog::Build` (`allowManifest` parameter)     |
| `project.ini` parsers            | `OctarinePackage.cmake` `octarine_read_project_ini`, `build.gradle` `identityProp` |
| Logger sink selection            | `src/General/Logger.cpp` (`android_logger` / `os_log` / stdout) |

For the historical plan and the rationale behind each stage, see
`ai/AssetPipelineAndDeviceBuildsPlan.md` (closed) and
`ai/DeviceShippingPlan.md` (active — distribution work picking up where
the asset pipeline left off).
