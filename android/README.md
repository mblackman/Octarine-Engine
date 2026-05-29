# Android build

Builds the example into an `.apk`/`.aab`. The engine cross-compiles to `libmain.so` (via CMake + the
NDK, with SDL3 + deps resolved from a vcpkg android triplet); this Gradle host app (the SDL3 template)
loads it through `SDLActivity`. SDL3 is statically linked into `libmain.so`, so there is no separate
`libSDL3.so` — `SDLActivity.getLibraries()` loads only `"main"`.

Asset resolution: the build is forced to `OCTARINE_SHIPPED=ON`, so the engine loads the baked
`asset_manifest.lua` and reads asset bytes through SDL IO against the APK asset root (no filesystem
scan, no `-p` argument).

## Prerequisites

| Tool          | Version / note                                                            |
|---------------|---------------------------------------------------------------------------|
| JDK           | 17 (AGP 8.7.3). Android Studio's bundled JBR works.                        |
| Android SDK   | platform `android-35`, build-tools `35.0.0`.                              |
| Android NDK   | **`28.2.13676358`** — must match `ndkVersion` in `app/build.gradle`.      |
| CMake         | `3.22.1` (SDK-managed) or any ≥ 3.15 on `PATH`.                            |
| vcpkg         | `VCPKG_ROOT` set; an android triplet available (see below).               |

### Install the SDK pieces

With Android Studio installed (or "Command line tools only" unzipped to
`<sdk>\cmdline-tools\latest\`):

```powershell
$sdk = "$env:LOCALAPPDATA\Android\Sdk"
& "$sdk\cmdline-tools\latest\bin\sdkmanager.bat" `
  "platform-tools" "platforms;android-35" "build-tools;35.0.0" `
  "ndk;28.2.13676358" "cmake;3.22.1" `
  "emulator" "system-images;android-35;google_apis;x86_64"
```

### Environment

```powershell
[Environment]::SetEnvironmentVariable("ANDROID_HOME", "$env:LOCALAPPDATA\Android\Sdk", "User")
[Environment]::SetEnvironmentVariable("ANDROID_NDK_HOME", "$env:LOCALAPPDATA\Android\Sdk\ndk\28.2.13676358", "User")
# VCPKG_ROOT must already point at your vcpkg checkout.
```

> **`ANDROID_NDK_HOME` is required, not optional.** vcpkg's port builds use their own android
> toolchain (`scripts/toolchains/android.cmake`), which finds the NDK *only* via this env var — the
> `VCPKG_CHAINLOAD_TOOLCHAIN_FILE` that `build.gradle` passes covers the engine's own configure but
> not vcpkg's dependency builds. Without it the build dies early with `Could not find android ndk`.
> Set it at User scope (above) **and** ensure it's present in the shell that runs `gradlew` (open a
> fresh terminal after setting it).

### `android/local.properties` (gitignored, machine-specific)

```properties
sdk.dir=C:\\Users\\<you>\\AppData\\Local\\Android\\Sdk
```

### vcpkg triplet

Default is `arm64-android` (physical arm64 devices). It ships with vcpkg in `triplets/` (alongside
`x64-android` for emulators); `arm64-android-release` etc. live in `triplets/community/`. Confirm:

```powershell
Get-ChildItem "$env:VCPKG_ROOT\triplets","$env:VCPKG_ROOT\triplets\community" -Filter "*android*"
```

For the standard `x86_64` Windows emulator, build `x64-android` and switch the ABI (see below).

## Build

First run builds all vcpkg android deps from source (SDL3, freetype, lua, …) — slow, one-time
(20–60 min).

```powershell
cd android
.\gradlew.bat assembleDebug
# release: .\gradlew.bat assembleRelease   (or bundleRelease for an .aab)
```

Output: `app/build/outputs/apk/debug/app-debug.apk`.

### Building for your own project (reusable host app)

> **Your project must ship a baked `asset_manifest.lua`** at its root. Android is a shipped build
> (manifest-load, no filesystem scan), so staging requires it — commit the manifest to your project
> (`OctarineEngine <project> -m bake`, then commit), or pass `-Poctarine.bakeExe=<desktop binary>` to
> bake it during the Gradle build. The example repo commits its manifest.

This host app is engine-generic — **no tracked files need editing per project**. Point it at your game
dir and override identity with `-Poctarine.*` properties:

```powershell
cd android
.\gradlew.bat assembleRelease `
  -Poctarine.projectDir=C:\path\to\MyGame `
  -Poctarine.applicationId=com.you.mygame `
  -Poctarine.appName="My Game" `
  -Poctarine.versionCode=3 -Poctarine.versionName=1.2
```

### Gradle properties

| Property                    | Default                           | Purpose                                                     |
|-----------------------------|-----------------------------------|-------------------------------------------------------------|
| `-Poctarine.projectDir`     | sibling `Octarine-Engine-Example` | Game project staged into the APK assets.                    |
| `-Poctarine.applicationId`  | `com.octarine.example`            | Android package id.                                         |
| `-Poctarine.appName`        | `Octarine Example`                | Launcher label (`app_name`).                                |
| `-Poctarine.versionCode`    | `1`                               | Integer version code.                                       |
| `-Poctarine.versionName`    | `1.0`                             | Display version.                                            |
| `-Poctarine.abi`            | `arm64-v8a`                       | Target ABI; **triplet auto-derives** (see emulator note).   |
| `-Poctarine.triplet`        | derived from `abi`                | vcpkg triplet override.                                     |
| `-Poctarine.minSdk`         | `28`                              | Min SDK (floor pinned by the triplet API level).            |
| `-Poctarine.ndkVersion`     | `28.2.13676358`                   | NDK version (`ndkVersion`).                                 |
| `-Poctarine.bakeExe`        | _unset_                           | Desktop `OctarineEngine` to re-bake the manifest.           |
| `-Poctarine.vcpkgRoot`      | `$env:VCPKG_ROOT`                 | vcpkg checkout (fallback if env unset).                     |
| `-Poctarine.storeFile` (+ `storePassword`/`keyAlias`/`keyPassword`) | _unset → debug key_ | Release signing keystore.        |

The `stageOctarineAssets` task copies the project into the APK assets (scratch excluded) and uses its
committed baked `asset_manifest.lua`. Pass `-Poctarine.bakeExe=<desktop binary>` to re-bake so the
manifest matches the staged tree exactly.

## Run

```powershell
adb install -r app\build\outputs\apk\debug\app-debug.apk
adb logcat -s SDL Octarine
```

### ABI / emulator note

Default ABI is `arm64-v8a` (physical arm64 phones). Standard desktop emulator images are `x86_64`,
which **won't** run an arm64-only APK. For the emulator it's a **one-flag change** — the triplet
auto-derives from the ABI:

```powershell
.\gradlew.bat assembleDebug -Poctarine.abi=x86_64   # selects the x64-android triplet too
```

## sol2 / NDK clang workaround

`build.gradle` passes `-DVCPKG_OVERLAY_PORTS=vcpkg-overlay-ports`, which overrides the stock `sol2`
port with `vcpkg-overlay-ports/sol2`. The NDK r28 clang (19.0.1) hits LLVM bug
[#91362](https://github.com/llvm/llvm-project/issues/91362) (sol2
[#1678](https://github.com/ThePhD/sol2/issues/1678) / [#1581](https://github.com/ThePhD/sol2/issues/1581)):
it can't take the address of sol2's `call(...) noexcept(std::is_nothrow_copy_assignable_v<T>)` static
member templates when binding usertype member variables, so the whole scripting surface fails to
compile. The overlay re-installs sol2 with that (optimization-only) `noexcept` stripped. It triggers a
one-time sol2 rebuild on first configure; nothing else changes. Desktop builds are untouched (they
don't use the overlay).

## CI

`.github/workflows/android.yml` builds the APK on every push/PR to `main` (and on demand) — the gate
that any project still cross-compiles for Android. ubuntu runner: JDK 17, `sdkmanager` installs the
pinned NDK + cmake, vcpkg android deps from the shared `x-gha` binary cache, `gradlew assembleDebug`
against the example, APK uploaded as an artifact. Build-only (no device/emulator).

## Status

**Builds:** `gradlew assembleDebug`/`assembleRelease` produces an APK (`com.octarine.example`,
label `Octarine Example`, `libmain.so` + `libc++_shared.so` + staged assets/manifest, `arm64-v8a`).
Host app is **project-generic** (`-Poctarine.*`, no per-project file edits). Emulator runtime
verified (Pixel arm64 via Berberis, manifest-load + tilemap + sprites render — see
`runtime-screen.png`). Release strip + JNI export survival on a stripped APK is the last open item.
