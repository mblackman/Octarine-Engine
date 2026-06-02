# Editor

The editor is the engine binary built with `OCTARINE_WITH_EDITOR=ON` — every
`editor-*` preset turns it on. It hosts the game inside an ImGui dockspace
alongside scene controls, an entity inspector, a Lua console, build/export
tooling, and an embedded debug overlay surface.

Audience: project authors driving the editor day-to-day. The shipping bake
step and packaging live in [`docs/asset-pipeline.md`](asset-pipeline.md) and
[`docs/device-builds.md`](device-builds.md).

---

## Launching

The editor takes the project directory as a positional argument:

```bash
./build/editor-debug/bin/debug/OctarineEngine ../Octarine-Engine-Example
```

Launched with no argument, the editor opens the **Project Selector** so a
project can be picked. The selection is persisted globally (see
[Persistence](#persistence)); subsequent launches without an argument reopen
the previous project after the next restart.

---

## Layout

A typical editor session looks like this:

```
┌────────────────────────────────────────────────────────────────────┐
│ File   Layout   Windows                                            │  ← main menu bar
├────────────────────────────────────────────────────────────────────┤
│ Play  Pause  Step  Stop  Mute  Run Player  Export Build...         │  ← playback toolbar
├──────────────┬────────────────────────────────────┬────────────────┤
│              │                                    │                │
│  Hierarchy   │           Scene View               │  Inspector /   │
│              │     (live game render target)      │  Engine Opts   │
│              │                                    │                │
├──────────────┴────────────────────────────────────┴────────────────┤
│ Lua Console │ Player Output │ Export Output │ Profiler │ Asset Br. │
└────────────────────────────────────────────────────────────────────┘
```

Windows are ImGui-docked — drag tabs anywhere, split, or float. The current
arrangement is saved to disk and restored on next launch.

---

## Menus

### File

| Item | Shortcut label | What it does |
|------|---------------|--------------|
| Open Project... | Ctrl+O | Opens the Project Selector window. |
| Save Preferences | Ctrl+S | Writes global + per-project prefs immediately (auto-saved on quit anyway). |
| Quit | Alt+F4 | Exits the editor. |

The Ctrl+O / Ctrl+S / Alt+F4 labels are menu hints — only Alt+F4 is wired by
the OS. Use the menu items directly or rely on the auto-save on quit.

### Layout

Editor layouts (window positions + which windows are open) are saved as named
`.layout` files in the SDL pref dir under `layouts/`.

| Item | What it does |
|------|--------------|
| Apply → `<name>` | Loads a saved preset. |
| Delete → `<name>` | Removes a saved preset. |
| Save Current As... | Prompts for a name and writes the current layout. |
| Reset to Default | Restores the built-in default layout. |

### Windows

Each item toggles the corresponding window's visibility. The visible-window
flags are persisted per-project. See [Windows](#windows) below for what each
one does.

---

## Playback toolbar

A row pinned directly under the main menu bar. Buttons drive the in-editor
scene's run state and the build/export pipelines.

| Button | Effect |
|--------|--------|
| **Play** | Resume the scene. If no scene is running and one is configured (see Scene Management), it loads first. |
| **Pause** | Pause simulation. The render still updates. |
| **Step** | Pause + advance exactly one frame. |
| **Stop** | Halt the scene and reset to the paused-and-ready state. |
| **Mute / Unmute** | Toggle `EngineOptions::audioEnabled`. Persisted globally. |
| **Run Player / Stop Player** | Spawn (or terminate) a separate `OctarineEngine-player` process pointed at the loaded project. See [Run Player](#run-player). |
| **Export Build... / Stop Export** | Open the Export Build modal, or terminate the in-flight build script. See [Export Build](#export-build). |

---

## Windows

### Scene View

Displays the game's render target inside an ImGui image. Aspect ratio is
preserved; the image is letterboxed inside the dock panel. Mouse and keyboard
input mapped through this image is forwarded to the running scene via the
`ViewportInfo` singleton (input systems translate window-space cursor
coordinates into game-space via that record).

Open by default. Closing it hides the in-editor game render — the game
continues to tick when Play is active, just without a visible surface.

### Scene Management

The scene loader for the editor.

- A text field plus **...** browse button to pick a scene's Lua entry point.
  A scene path inside the project resolves to a project-relative string
  (e.g. `scripts/level1.lua`); a path outside stays absolute.
- **Load** invokes `Game::LoadScene(path)`.
- Once a scene is loaded, **Reload** and **Stop** appear.

The currently-selected scene path is persisted per-project so each project
remembers its last scene.

### Hierarchy / Entity Inspector

Two-pane view:

- **Left** — a filterable list of every user entity (engine-internal entities
  are hidden). Names come from `NameComponent`; entities without one display
  as `Entity <id>`.
- **Right** — for the selected entity: rename via `NameComponent`, **Destroy
  Entity**, and a per-component inspector block for every component the
  entity has. The **Add Component** combo lists every component type with a
  registered default-add inspector.

Inspector widgets live in `src/Editor/Inspectors/` — one header per component
type, registered through `ComponentInspectorRegistry`.

### Asset Browser

A flat catalog readout — collapsing headers for Textures, Fonts, and Audio
Clips, each listing the ids the runtime currently has loaded. Read-only.

### Lua Console

REPL into the running `sol::state`.

- The history shows `Logger` output, colorized by severity
  (`[Error]` red, `[Warn]` yellow, `[Info]` green, `> command` blue).
- **Clear** wipes the displayed history.
- Type Lua at the bottom and press Enter to execute. Errors print as Lua
  errors; print output flows through `Logger::LogLua`.
- Up / Down arrows walk submitted-command history.
- The log auto-scrolls when already pinned to the bottom; scroll up to break
  the follow.

### Performance Profiler

Two tables sourced from `PerfUtils`:

- **System/Scope** — every `PROFILE_*` scope's accumulated time in ms.
- **Counter** — every named perf counter's current value.

Requires that whatever code path is being profiled compiles in the
`OCTARINE_ENABLE_PROFILING` macro (i.e. the `player-profile` preset, or
adding `-DOCTARINE_ENABLE_PROFILING=ON` to an editor preset).

### Engine Options

Live-editable runtime knobs:

- Show ImGui Demo Window, Show FPS Counter, Show Entity Info
- Draw Colliders, Log Input Events
- Audio Enabled, Master Volume slider
- Pause Execution, Step Frame, Time Scale slider

These mutate `EngineOptions` directly; values are persisted via
`GameConfig::SaveUserPreferences` (File → Save Preferences) and auto-saved on
exit.

### Editor Settings

Editor-only appearance:

- **Font Size** slider (10–40 px). Font atlas rebuild defers until the slider
  is released to avoid lag.
- **Reset** restores the baseline 16 px.
- **Theme** combo: Dark, Light, Classic.

Persisted globally — applies to every project.

### Player Output

Captures stdout/stderr from the player subprocess launched by **Run Player**.

- Status colored chip: Idle / Running / Exited / Failed to launch (and the
  exit code when the process has ended).
- **Binary**: the resolved player binary path.
- **Clear** + Auto-scroll toggle.
- stderr lines render red; stdout in default text.

The buffer is ring-capped at 4096 lines; the header shows the dropped count
when older lines have rolled off.

### Export Output

Same shape as Player Output, but for the build-script subprocess spawned by
**Export Build**. Reports Idle / Building / Succeeded / Failed plus exit code
and the resolved script path under `<project>/scripts/`.

### Signing Settings

Stores Android release-signing credentials in the OS secret backend
(DPAPI on Windows, Keychain on macOS). Fields:

- Keystore path (visible — it isn't sensitive on its own; surfaces directly).
- Store password (password field; a `(stored)` chip indicates a saved value).
- Key alias.
- Key password (password field; same `(stored)` chip).

**Save** writes any field that has a value typed in; empty fields are
preserved. Password buffers are zeroed in memory immediately after save.
**Clear all** wipes every stored entry.

On platforms without a secret backend, the window shows a fallback message
pointing at the `OCTARINE_ANDROID_*` env vars instead. The Export Build
modal reads from the same env names regardless of backend.

### Project Selector

Auto-opens when the editor starts without a loaded project, or from File →
Open Project... Pick a directory via the text field or the **...** browser,
then **Open Project**. The selection is saved globally; a "Restart Required"
modal reminds the user to relaunch — the engine only loads a project's
asset catalog at startup.

### FPS / Entity Info / ImGui Demo Window

Auxiliary overlays controlled from the Windows menu. FPS plots the last 120
frames; Entity Info shows the live user-entity count; ImGui Demo Window is
the standard ImGui demo for reference.

---

## Run Player

The standalone player binary is the same engine without the editor surface —
the runtime the game actually ships with. **Run Player** spawns it as a
subprocess pointed at the currently-loaded project and pipes its
stdout/stderr into the Player Output window.

`PlayerLauncher` probes a dev-tree layout: editor binary in
`build/editor-<config>/bin/<config>/` finds the player in the sibling
`build/player-<config>/bin/<config>/`. Run Player flips status to **Failed to
launch** with a log line when the binary can't be located.

One concurrent player at a time. **Stop Player** sends a terminate; the
shutdown path also fires automatically when the editor exits.

---

## Export Build

Wraps the project's scaffolded build script
(`<project>/scripts/build-desktop.{sh,ps1}` or
`scripts/build-android.{sh,ps1}`). The script is generated by
`octarine-init-build` — see [`docs/device-builds.md`](device-builds.md).

The modal:

- **Target**: Desktop (host OS, `ship-release` preset), Android (debug APK),
  or Android (release AAB).
- **Version name / Version code**: leave blank to fall through to
  `project.ini`.
- For Android release, surfaces a reminder of the `OCTARINE_ANDROID_*` env
  vars the build expects. Configure them via Signing Settings if a secret
  backend is available, or set them in the shell that launches the editor.
- **Re-validate** re-runs `ExportBuilder::Validate` (verifies project.ini and
  the target's build script exist).
- **Build** spawns the script. Output streams into the Export Output window.

A build in flight disables the modal; the toolbar button becomes **Stop
Export**, which terminates the script subprocess.

---

## Hotkeys

| Key | What |
|-----|------|
| `` ` `` (backtick / grave) | Toggle `EngineOptions::showDebugGUI` — the Lua-driven `onDebugGUI` overlay surface. |
| `Esc` | Quit the engine. |

The File menu displays `Ctrl+O`, `Ctrl+S`, and `Alt+F4` labels for menu
discoverability, but only `Alt+F4` is wired (by the OS).

---

## Persistence

The editor maintains two stores, both INI-keyed text:

| Scope | File | Keys |
|-------|------|------|
| Global | `SDL_GetPrefPath("Octarine","Engine")/editor_settings.ini` | `lastProjectPath`, `editorFontSize`, `editorStyleIndex`, `audioMuted`, `masterVolume` |
| Per-project | `<project>/editor_prefs.ini` | `currentScenePath` and every `show*` window-visibility flag |

`SDL_GetPrefPath` resolves to:

| Platform | Path |
|----------|------|
| Windows | `%APPDATA%\Octarine\Engine\` |
| macOS | `~/Library/Application Support/Octarine/Engine/` |
| Linux | `~/.local/share/Octarine/Engine/` (respects `$XDG_DATA_HOME`) |

Layout presets are `.layout` files under
`SDL_GetPrefPath("Octarine","Engine")/layouts/`. Each file encodes the
window-visibility flags and a copied `[imgui]` section for ImGui's docking
state. Listed alphabetically in Layout → Apply / Delete.

Auto-save: prefs are written on quit, on File → Save Preferences, and after
mute / unmute toggles. Project prefs require a loaded project — the editor
silently skips per-project save with no project.

---

## See also

- [`docs/lua-scripting.md`](lua-scripting.md) — how Lua scripts wire into the
  engine and the `onDebugGUI` hook driven by the playback toolbar.
- [`docs/scenes.md`](scenes.md) — scene lifecycle, `load_scene` /
  `reload_scene` (what the Scene Management window calls under the hood).
- [`docs/device-builds.md`](device-builds.md) — `project.ini` identity keys
  and what the build scripts that Export Build spawns actually do.
- [`docs/asset-pipeline.md`](asset-pipeline.md) — the catalog the Asset
  Browser reads and the bake step.
