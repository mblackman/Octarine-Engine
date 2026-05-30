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
| ImageMagick   | `magick` (or `convert`) on `PATH`, **only if** the project sets `icon=`.  |

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

### Launcher icon + Android-12 splash

Set `icon=<path.png>` (1024×1024 PNG, project-relative) and optionally `splash_color=#rrggbb` in
`project.ini`. The `generateOctarineIcons` Gradle task invokes
`scripts/octarine-icons.cmake` to produce `mipmap-*/ic_launcher.png` (legacy),
`mipmap-*/ic_launcher_foreground.png` + `mipmap-anydpi-v26/ic_launcher.xml` (Android 8+ adaptive),
and a `values-v31/octarine_splash.xml` override that wires `windowSplashScreenAnimatedIcon` /
`windowSplashScreenBackground` for Android 12+. Without `icon=` (or without ImageMagick on
`PATH`), the task warn-skips and the APK ships the green-robot defaults + a launcher-icon flash on
12+.

Adaptive icons crop the outer ~16% — leave safe padding in the source PNG so the cropped circle
shows the same art Android 7 sees in the legacy square.

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

## Release signing & AAB

Play Store wants an `.aab` (Android App Bundle), not an `.apk`, and it must be signed with an
**upload key** consistent across every update — losing it means you can never update the app on
that listing again. Treat the keystore like a secret with no expiry: back it up out-of-band the day
you generate it.

### Generate an upload keystore

```powershell
# One-time, anywhere outside the repo. Picks RSA-2048, 25-year validity (Play minimum is 25y).
keytool -genkeypair -v `
  -keystore octarine-upload.jks `
  -alias octarine-upload `
  -keyalg RSA -keysize 2048 -validity 9125 `
  -storetype JKS
```

`keytool` prompts for two passwords (store + key) and a distinguished-name block — fill it in for
your studio identity. Keep `octarine-upload.jks` outside the repo (e.g. a 1Password vault or
encrypted drive). Lose the file or the passwords and the Play listing is orphaned; back both up.

### Local signed build

`build.gradle` reads four `-Poctarine.*` properties; supply them via `~/.gradle/gradle.properties`
(gitignored, machine-scoped) so you don't pass passwords on the command line:

```properties
octarine.storeFile=C:\\path\\to\\octarine-upload.jks
octarine.storePassword=...
octarine.keyAlias=octarine-upload
octarine.keyPassword=...
```

```powershell
cd android
.\gradlew.bat bundleRelease -Poctarine.projectDir=C:\path\to\MyGame
# -> app\build\outputs\bundle\release\app-release.aab
```

Verify the result is signed with your upload key, not the debug key:

```powershell
& "$env:LOCALAPPDATA\Android\Sdk\build-tools\35.0.0\apksigner.bat" verify --print-certs `
  app\build\outputs\bundle\release\app-release.aab
```

### CI signing

`.github/workflows/android.yml` decodes the keystore from a GitHub secret per run and forwards it
to Gradle. Configure these repository secrets (Settings → Secrets and variables → Actions):

| Secret                              | Value                                                     |
|-------------------------------------|-----------------------------------------------------------|
| `OCTARINE_ANDROID_KEYSTORE`         | `base64 -w0 octarine-upload.jks` output (single line).    |
| `OCTARINE_ANDROID_STORE_PASSWORD`   | Keystore password.                                        |
| `OCTARINE_ANDROID_KEY_ALIAS`        | Key alias (e.g. `octarine-upload`).                       |
| `OCTARINE_ANDROID_KEY_PASSWORD`     | Key password (often same as store password).             |

With the secrets set, `bundleRelease` on every push/PR signs the AAB with your upload key. With the
secrets unset (e.g. fork PRs), the workflow falls through to the debug key — the AAB is still a
valid build-passes signal, but it isn't Play-uploadable.

### Play Store upload (internal track)

Tag pushes (`v*`) auto-upload the AAB to the Play Store **internal track** via
`r0adkll/upload-google-play@v1`. Requires a Google Cloud service account with **Release Manager**
on the `com.octarine.example` Play Console entry (Play Console → Users and permissions → Invite
new user → service-account email → grant app-level Release Manager).

| Secret                              | Value                                                     |
|-------------------------------------|-----------------------------------------------------------|
| `PLAY_SERVICE_ACCOUNT_JSON`         | Full JSON key contents from Google Cloud (NOT base64).    |

Generate the key: Google Cloud Console → IAM & Admin → Service Accounts → create account → Keys
tab → Add Key → JSON. Paste the full file contents into the GitHub secret.

The upload step is gated on `startsWith(github.ref, 'refs/tags/v')`, so non-tag pushes and PRs
never burn upload quota or accidentally promote a build. When the secret is unset (fork PRs,
pre-account state), the step skips with no error.

**Rotation.** Service-account keys don't expire by default, but rotate annually as hygiene:
generate a new key, update `PLAY_SERVICE_ACCOUNT_JSON`, then delete the old key from the Service
Accounts console. The Play Console permission grant is on the *account*, not the key — no Play-side
change needed.

**Track promotion.** Internal track is the only track this workflow targets. Promote
internal → closed → open → production manually from the Play Console release dashboard once a
build is QA'd. Adding a `track` matrix axis is straightforward if the workflow needs to push to
multiple tracks per release.

### R8 / minification

`minifyEnabled` defaults to `false` on the release build type. `proguard-rules.pro` already keeps
the SDL JNI surface plus a safety-net `native <methods>;` rule, so flipping it on is a probe rather
than a leap of faith. CI runs the probe automatically on every push/dispatch via the
`android (minify probe)` job (single-ABI arm64-v8a `bundleRelease -Poctarine.minify=true` plus
SDL JNI symbol assertion). Run it locally before a release:

```powershell
.\gradlew.bat bundleRelease -Poctarine.minify=true -Poctarine.projectDir=C:\path\to\MyGame
# Spot-check the stripped JNI symbols survived:
& "$env:LOCALAPPDATA\Android\Sdk\ndk\28.2.13676358\toolchains\llvm\prebuilt\windows-x86_64\bin\llvm-nm.exe" `
  -D app\build\intermediates\merged_native_libs\release\out\lib\arm64-v8a\libmain.so `
  | Select-String 'Java_org_libsdl_app_'
```

Re-enable in `build.gradle` (or just keep `-Poctarine.minify=true` in your project's CI) once you've
confirmed startup works on a real device for that project. Size win is modest (only the Java side
shrinks; SDL3 + engine are in `libmain.so`, untouched).

## CI

`.github/workflows/android.yml` builds the APK and AAB on every push/PR to `main` (and on demand) —
the gate that any project still cross-compiles for Android. ubuntu runner: JDK 17, `sdkmanager`
installs the pinned NDK + cmake, vcpkg android deps from the shared `x-gha` binary cache, then runs
`gradlew assembleDebug` (debug APK) followed by `gradlew bundleRelease` (release AAB) against the
example. Both artifacts upload. Build-only (no device/emulator). Release signing uses the
`OCTARINE_ANDROID_*` secrets (see above) when set; otherwise falls back to the debug key so fork
PRs still build a complete artifact.

## Status

**Builds:** `gradlew assembleDebug`/`assembleRelease`/`bundleRelease` produces APK/AAB
(`com.octarine.example`, label `Octarine Example`, `libmain.so` + `libc++_shared.so` + staged
assets/manifest, `arm64-v8a`). Host app is **project-generic** (`-Poctarine.*`, no per-project
file edits). Release signing wired via `-Poctarine.storeFile`/`storePassword`/`keyAlias`/
`keyPassword` (or the matching CI secrets), debug-key fallback. R8/minify off by default,
opt-in via `-Poctarine.minify=true`. Emulator runtime verified (Pixel arm64 via Berberis,
manifest-load + tilemap + sprites render — see `runtime-screen.png`).
