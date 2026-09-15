# Desktop Builds

This document organizes our understanding, progress, and architectural plans for CMake release builds of games made on the Octarine Engine.

---

## Progress & Completed Work

### 1. Build System Architecture & Documentation
- [x] Analyzed and documented the complete CMake build system end-to-end in [cmake-build-system.md](cmake-build-system.md):
  - Target presets (`editor-debug`, `editor-release`, `player-debug`, `player-release`, `ship-release`, `ship-mac-universal`).
  - Layered static library architecture (`octarine_core` through `octarine_engine`).
  - Packaging lifecycle in `cmake/OctarinePackage.cmake` (identity parsing, icon generation, headless bake validation gate, script protection, dependency bundling, CPack).
  - Runtime asset contract (`SDL_GetBasePath()` and `OCTARINE_SHIPPED` manifest loading).

### 2. CMake Codebase Cleanup & Modernization
- [x] Cleaned up root [CMakeLists.txt](CMakeLists.txt):
  - Pruned conversational, historical, and problem-log comments while preserving crucial maintainer context.
  - Standardized layout with clear section banners (Project, Options, Overrides, Toolchain, Dependencies, Artifacts, Tooling, Benchmarks, Tests).
- [x] Simplified the test suite:
  - Extracted test declarations into [tests/CMakeLists.txt](tests/CMakeLists.txt) via `add_subdirectory(tests)`.
  - Introduced deduplicated registration helpers (`octarine_add_core_test`, `octarine_add_engine_test`, `octarine_add_test`).
  - Removed redundant per-target C++20 property definitions and centralized MSVC compiler options.
  - Reduced root `CMakeLists.txt` from **656 lines to 339 lines** (~48% reduction) while keeping all 20 CTest targets and names 100% compatible.
- [x] Fixed `.gitignore` to prevent loose `*build-*` globs from ignoring markdown files (`!*.md`).

### 3. macOS Prototype Release Build & Packaging Validation (*Gravewarden* / `dash-vs`)
- [x] **Compiler & Target Fixes**:
  - Fixed Clang `-Werror` failure on unused private field `sol::state& lua_` in `src/Engine/FrameLoop.h` when `OCTARINE_SHIPPED` is defined (tagged with `[[maybe_unused]]`).
  - Fixed output naming in `cmake/OctarinePackage.cmake`: target `OUTPUT_NAME` is now set to resolved `_pkg_name` so the generated binary and `.app` bundle are named after the game (`Gravewarden.app`) instead of generic `OctarineEngine.app`.
- [x] **Asset Staging & Developer File Filtering**:
  - Filtered developer tooling and local configs out of the installed game bundle (`.github`, `.vscode`, `.idea`, `.env*`, `.engine`, `.gitattributes`, `*.sh`, `*.ps1`, `types`).
  - Added support for `package_exclude` in `project.ini` and `-DOCTARINE_PACKAGE_EXCLUDE` to allow projects to specify custom exclusion globs.
- [x] **Packaging Pipeline Execution**:
  - Successfully configured and compiled `ship-release` against `/Users/mblackman/workspace/gh/dash-vs`.
  - Headless asset bake gate (`-m bake`) verified: executed headless and emitted `asset_manifest.lua` directly in the project directory, packaging cataloged assets, glyph atlases, and normalized audio into `asset_bundle.pak`.
  - Lua script protection verified: `game.lua` compiled to stripped Lua 5.4 bytecode in the staged bundle.
  - CPack verified: produced a `Gravewarden-0.0.1-Darwin.dmg` containing `Gravewarden.app`.
- [x] **macOS App Bundle Metadata & Packaging Hygiene**:
  - Audited unpacked DMG contents and resolved missing bundle metadata: set `MACOSX_BUNDLE_GUI_IDENTIFIER`, `MACOSX_BUNDLE_BUNDLE_NAME`, `MACOSX_BUNDLE_SHORT_VERSION_STRING`, `MACOSX_BUNDLE_BUNDLE_VERSION`, `MACOSX_BUNDLE_COPYRIGHT`, and `MACOSX_BUNDLE_INFO_STRING` properties on target.
  - Added additional default exclusions in `_install_excludes` for `.DS_Store`, `.luarc.json`, `.luacheckrc`, `.stylua.toml`, `BUILDING.md`, and `README.md`.
  - Rebuilt package and verified `Info.plist` is fully populated with valid bundle ID (`com.mblackman.gravewarden`), version (`0.0.1`), build (`1`), and description.
  - Verified static linkage: native binary has zero dynamic third-party dylib dependencies (only macOS system frameworks).
- [x] **Zero-Parameter Runtime Boot Verification**:
  - Tested running the packaged `Gravewarden.app/Contents/MacOS/Gravewarden` directly with dummy video/audio drivers.
  - Verified runtime log:
    ```
    AssetCatalog: baked manifest found, loading (scan skipped): .../Contents/Resources/asset_manifest.lua
    AssetCatalog: loaded 2 entries from manifest .../Contents/Resources/asset_manifest.lua
    ```
  - Confirmed the game starts with zero command-line arguments, resolves `SDL_GetBasePath()` to `Contents/Resources`, and loads assets via the pre-baked manifest without filesystem scanning.

---

## Tasks (Current Iteration)

### Task 1: Document Build System Architecture
- [x] Break down the current cmake setup into a document that goes into detail of how the entire process works. (Completed in [cmake-build-system.md](cmake-build-system.md)).

### Task 2: Prototype Game Release Builds (*Gravewarden* / `dash-vs`)
- [x] **macOS Local Build**: Run and validate packaging against the prototype game (`dash-vs`).
- [ ] **Linux Release Build**: Validate Linux artifact generation (`.tar.gz`) and dependency resolution (`$ORIGIN` RPATH).
- [ ] **Windows Release Build**: Validate Windows artifact generation (`.zip` / optional NSIS) and MSVC CRT redistribution.

### Task 3: Developer Build Workflows & Ergonomics
- [ ] **Quick Local Unpacked Test Builds**:
  - Provide an easy path for developers to stage and run an unpacked release player locally without waiting for full DMG/ZIP compression (e.g. `cmake --install build/ship-release --prefix <out_dir>`).
- [ ] **Project Build Scripts**:
  - Validate and refine `scripts/octarine-init-build.sh` / `.ps1` (emitted `build-desktop.sh` / `.ps1`).
- [ ] **CI & Remote Packaging**:
  - Document and streamline the GitHub Actions packaging workflow (`package-desktop.yml` / `_package-os.yml`).

### Task 4: Build Customization & Extensibility
- [x] **Asset Filtering in `project.ini`**:
  - Supported `package_exclude` in `project.ini` and `-DOCTARINE_PACKAGE_EXCLUDE` for custom exclusion patterns.
  - Verified project-level exclusion of `BUILDING.md` via `dash-vs/project.ini` without requiring engine-level hardcoding.
- [x] **Asset Pack Redundancy Elimination & Allowlist Packaging**:
  - Replaced blanket `install(DIRECTORY)` and fragile negative pattern exclusions with a clean allowlist model in `cmake/OctarinePackage.cmake`.
  - Staged files strictly limited to runtime essentials: `config.ini`, `project.ini`, `LICENSE*`, `THIRD_PARTY_LICENSES.txt`, `asset_manifest.lua`, `asset_bundle.pak`, and custom entries via `package_include`.
  - Resolved `atlases/` runtime requirement: connected `FontStore::Add` and `GlyphAtlas::Load` to `AssetManager::OpenAssetIO` via `SDL_IOStream`, allowing font glyph atlases to load directly from `asset_bundle.pak` without loose files on disk.
  - Removed loose `atlases/` and `fonts/` directories from the release bundle; raw unbaked assets are 100% eliminated from the package.
  - Rebuilt package and verified zero developer tooling leaks, 100% CTest pass (20/20), and flawless zero-parameter boot.
- [x] **Script Bundling into Asset Pak (`asset_bundle.pak`)**:
  - Bundled all game Lua scripts (`game.lua`, `scripts/**/*.lua`) directly into `asset_bundle.pak` alongside cataloged assets and font atlases.
  - Re-routed `Game::LoadGame`, `SceneLoader::LoadScene`, and the engine `dofile` hook (`LuaDofileViaSDL`) through `AssetManager::OpenAssetIO(fullPath)` so all script loads resolve transparently from `asset_bundle.pak` or loose disk fallback.
  - Implemented pre-bake script compilation in `cmake/OctarinePackage.cmake`: scripts are staged into `${CMAKE_BINARY_DIR}/_octarine_staged_scripts`, compiled to stripped Lua bytecode (`luac -s`), and optionally encrypted with rolling XOR without mutating developer source files in the project workspace.
  - Added `--scripts-dir <dir>` flag to headless bake mode (`-m bake`), allowing `Game::Bake` to swap source files for compiled bytecode while maintaining original relative path keys in the pak TOC.
  - Shipped releases now contain **zero loose `.lua` files** in `Contents/Resources`, delivering tamper resistance, instantaneous load times with zero filesystem seek syscalls, and modding capability (loose files on disk still load if not present in the pak).
- [ ] **Metadata & License Drop-ins**:
  - Ensure projects can easily provide custom icons, window titles, and third-party attribution drops in `THIRD_PARTY_LICENSES.d/`.

---

## Future Concerns

- **Mobile Platforms**: iOS (currently parked on `defer/ios`) and Android (managed via Gradle in `android/`).
- **Web Builds**: WebAssembly / Emscripten considerations (threads, asset packaging, and Lua binding portability).