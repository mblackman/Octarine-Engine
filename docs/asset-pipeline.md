# Asset pipeline

How Octarine discovers, processes, and loads project assets — the dev-time
filesystem scan, the bake step that emits `asset_manifest.lua`, and the
runtime resolution path used by both dev builds and shipped bundles.

Audience: project authors adding new assets, contributors touching
`src/AssetManager/`. The shipping side (CPack, packaging, signing) lives in
[`docs/device-builds.md`](device-builds.md).

---

## The model

Every asset has a stable **id** (a string) and a single file on disk. The
engine maintains a catalog mapping `id → CatalogEntry`. Lua code references
assets only by id; the catalog owns the path, format, and any optional
processing (atlasing, normalization).

Two ways the catalog gets populated:

| Mode | When | What happens |
|------|------|--------------|
| **Filesystem scan** | Dev builds; bake step | Recursively walks the project directory, classifies files by extension, parses `.meta` sidecars, emits one `CatalogEntry` per asset. |
| **Manifest load** | Shipped builds (`OCTARINE_SHIPPED=ON`); dev override `--use-manifest` | Reads `asset_manifest.lua` from the project root through SDL_IO (works inside a read-only APK / `.app`), restores entries verbatim. No filesystem walk. |

The shipped manifest is produced by the **bake step** (§ 4) — a headless run
that scans the project, validates every asset reference, optionally packs
atlases / rasterizes glyphs / normalizes audio, then writes
`asset_manifest.lua`.

---

## Supported asset types

Classified by extension. Files with an unsupported extension or no sidecar
default still produce a catalog entry as long as the extension matches.

| Type | Extensions | Default id |
|------|-----------|-----------|
| Texture | `.png`, `.jpg`, `.jpeg`, `.bmp` | Filename stem (e.g. `tank.png` → `tank`). |
| Font | `.ttf`, `.otf` | `<stem>-<size>` per requested glyph size (default size `16`). |
| Audio | `.wav`, `.ogg`, `.mp3`, `.flac` | Filename stem. |

Override the derived id via `meta.id` in the sidecar (§ 3). Id collisions
across the project are a hard error — the bake refuses to emit and the dev
build refuses to start.

---

## `.meta` sidecars

A `.meta` sidecar is an **optional Lua file** next to the asset that returns
a table of per-asset options. `foo.png.meta` belongs to `foo.png`.

Schema is type-specific. Anything not set falls through to the project
default (from `GameConfig`) and then to the engine default. A missing
sidecar means "all defaults".

### Texture (`*.png.meta`, `*.jpg.meta`, …)

```lua
return {
    id         = "player",     -- override the derived id (filename stem)
    scale_mode = "nearest",    -- "nearest" or "linear"; falls back to GameConfig DefaultScalingMode
    atlas      = "main",       -- atlas group name; nil = loose texture
    no_atlas   = false,        -- true = never folded into an atlas, even if the group is packed
}
```

`no_atlas = true` is the pixel-art escape hatch: bilinear bleed at atlas
boundaries can shift sub-pixel `src_rect` math. Use it when a texture must
remain a standalone PNG.

### Font (`*.ttf.meta`, `*.otf.meta`)

```lua
return {
    id    = "ui",          -- override the derived id base
    sizes = { 12, 16, 24 } -- one catalog entry per size: "ui-12", "ui-16", "ui-24"
    -- or single-size:
    -- size = 16
}
```

Default is `{ 16 }`. Use `sizes` for multi-size font ladders, `size` for
single-size convenience.

### Audio (`*.wav.meta`, `*.ogg.meta`, …)

```lua
return {
    id        = "explosion",   -- override the derived id (filename stem)
    stream    = false,         -- true = stream from disk; false = full-decode into memory
    normalize = true,          -- bake-time BS.1770 loudness normalize (WAV only — see § 5)
}
```

Default `stream = false`, `normalize = false`.

### Recipe: a folder of sprites going into one atlas

```
images/
├── tank.png       + tank.png.meta:       { atlas = "main" }
├── enemy.png      + enemy.png.meta:      { atlas = "main" }
├── pickup.png     + pickup.png.meta:     { atlas = "main" }
└── tilemap.png    + tilemap.png.meta:    { no_atlas = true }
```

The first three pack into `__atlas_main`; `tilemap.png` ships as a loose
texture so its source-rect math stays pixel-exact.

---

## The bake step

```
OctarineEngine <project-dir> -m bake
```

Headless run that:

1. **Scans the filesystem.** Builds the catalog from the live tree
   (sidecars + extension classification). No manifest probe, even if one
   exists — the bake is the source of truth.
2. **Runs the project headless.** Executes the startup script
   (`config.ini` `StartupScript=`). Scene loaders call
   `acquire_scene_assets`, which validates references but skips the GPU /
   mixer upload (no renderer, no audio device in bake mode).
3. **Packs atlases.** `TextureAtlasBaker` walks the catalog for
   `meta.atlas = <group>` entries, packs each group into one square PNG at
   `<project>/_atlases/<group>.png`, mutates member entries to record their
   atlas slice, and adds one new catalog entry per atlas (id
   `__atlas_<group>`).
4. **Rasterizes glyph atlases.** For each font/size pair, `AtlasBaker`
   produces `<font>.atlas.png` + `<font>.atlas.lua` (codepoint metrics).
   Runtime `GlyphAtlas` blits from this surface instead of paying
   per-string `TTF_RenderText`.
5. **Normalizes audio.** Entries with `normalize = true` get BS.1770
   integrated-loudness measurement; gain is applied to land at the target
   LUFS (default `-16.0`). Out-of-range samples clamp to int16. WAV-only —
   `.ogg` / `.mp3` skip normalize with a warning (the engine doesn't vendor
   their decoders yet).
6. **Emits `asset_manifest.lua`.** Lua table mapping every id to
   `{ type, file, ...metadata }`. Paths are project-relative and
   forward-slashed for portability.
7. **Returns nonzero on any unresolved reference.** This is the CI gate for
   shipping builds. Desktop CPack runs the bake at install time, so a
   broken reference fails the package rather than the runtime.

Bake mode is selected with `-m bake`. The default mode is `play` (run the
game). `--startup-mode bench` and `--startup-mode stress` are for the
benchmark harness.

### When to commit `asset_manifest.lua`

| Platform | Commit it? |
|----------|------------|
| Desktop dev | No. Dev runs scan; manifest is regenerated at package time. |
| Android | **Yes** — or pass `-Poctarine.bakeExe=<host binary>` so Gradle re-bakes during the APK build. The cross-compiled engine binary can't run on the build host, so Gradle can't bake at package time. Most projects commit. |
| iOS | (Parked on `defer/ios`.) Same constraint as Android once it reattaches. |

The `--use-manifest` CLI flag forces a non-shipped engine binary to load
the manifest path instead of scanning. Useful for verifying the
manifest-load branch from a dev build.

---

## Atlas packing

### Textures

`TextureAtlasBaker` groups entries by `meta.atlas = <group>` and packs each
group into one PNG via `stb_rect_pack`. Defaults:

- **Padding:** 2 px transparent gutter between sprites (`kDefaultPaddingPx`).
- **Max dimension:** 4096 px square (`kDefaultMaxAtlasDim`). Conservative
  ceiling that survives every shipping target.

A group that won't fit fails the bake. There is **no silent fallback to
multiple atlases per group** — the bake forces you to either split the
group or shrink the art so the manifest stays one-atlas-per-group.

Packing is deterministic: same input catalog → same atlas bytes. Member
ordering follows the catalog's `std::map` id order; `stb_rect_pack`'s seed
is fixed.

### Glyphs

`AtlasBaker` rasterizes a codepoint set into a single packed PNG +
companion metrics Lua file:

- Default codepoint set is ASCII printable (32–126).
- Accented Latin or non-Latin scripts need an explicit list — `meta.glyphs`
  is on the roadmap but not yet plumbed; track it through the doc rather
  than working around it.

Runtime `GlyphAtlas` loads the PNG into a CPU surface and blits glyph rects
out of it during text compose. `RenderTextSystem` falls back to live
`TTF_RenderText` when the atlas isn't present, so missing atlases degrade
gracefully on dev builds.

---

## Audio loudness normalize

`meta.normalize = true` opts a clip into BS.1770-4 integrated-loudness
normalize at bake time. Implementation:

- K-weighting filter chain (pre-filter high-shelf + RLB high-pass).
- 400 ms gated mean-square blocks, 75% overlap.
- Absolute gate `-70 LUFS` + relative gate `-10 LU` below ungated mean
  (BS.1770 spec).
- Uniform gain to reach the target loudness (default `-16.0 LUFS`).
- Samples that would clip int16 are hard-clamped.

**Format support:** PCM 16-bit WAV in / out, mono or stereo, any sample
rate (filter coefficients derived per-rate via bilinear transform). `.ogg`
/ `.mp3` are skipped — they need a decoder this engine doesn't yet vendor.

When normalize fails (unsupported format, signal entirely below `-70 LUFS`,
I/O error), the bake falls through to shipping the original. Check the
bake log for `AudioNormalizer:` warnings.

---

## Loading assets from Lua

Two paths. Prefer the first for new code.

### Declarative (recommended): `acquire_scene_assets`

Scene returns a table; `acquire_scene_assets` walks it, collects every
asset reference (sprite `texture_asset_id`, text `font_id`, audio source
ids, tilemap `texture_asset_id`, plus the explicit `preload = { ... }`
list), validates each against the catalog, then acquires the lot.

```lua
local scene = {
    preload = { "explosion", "music-menu" }, -- ids the static scan can't see
    entities = {
        {
            components = {
                sprite = { texture_asset_id = assets.tank, ... },
                -- ...
            }
        }
    }
}

acquire_scene_assets(scene)
return scene
```

Use `preload` for runtime-spawned ids (projectiles fired from script,
click-spawned enemies) and load-time injection (Lua adding components
before the loader scans). It's the documented escape hatch, not a
zero-maintenance promise.

`SceneAssetScanner` is the implementation; the rule is "static-scannable
ids should not appear in `preload`."

### Explicit (legacy): `load_asset`

```lua
load_asset({ type = "texture", id = "player", file = "images/player.png" })
load_asset({ type = "font",    id = "ui",     file = "fonts/ui.ttf", font_size = 16 })
```

Bypasses the catalog — `file` is taken verbatim. Used for projects that
predate `acquire_scene_assets`. The bake step still validates that `file`
exists on disk, so a typo'd path fails the bake the same way an unresolved
catalog id does.

### The `assets` table

`AssetCatalog::InstallLuaIdTable` exposes every catalog id as a global
`assets.<id>`. Direct field access; reading an unknown id raises a Lua
error rather than silently passing through a bad string.

```lua
sprite = { texture_asset_id = assets.tank }   -- errors at script load if 'tank' is missing
```

Strongly preferred over hand-rolling string literals.

---

## File layout

```
<project>/
├── config.ini
├── project.ini
├── game.lua
├── asset_manifest.lua          # committed for Android; produced by bake elsewhere
├── _atlases/                   # produced by the bake; safe to add to .gitignore on desktop
│   └── main.png
├── images/
│   ├── tank.png
│   └── tank.png.meta
├── fonts/
│   ├── ui.ttf
│   └── ui.ttf.atlas.png        # produced by glyph bake
│   └── ui.ttf.atlas.lua
├── sounds/
│   ├── boom.wav
│   └── boom.wav.meta
└── scripts/
    └── …
```

The directory names (`images/`, `fonts/`, `sounds/`, `scripts/`) are
convention, not enforced. The catalog scan is recursive and classifies by
extension — put assets wherever you want as long as ids stay unique.

---

## Troubleshooting

**Bake exits nonzero with "id 'X' maps to a missing file"** — `load_asset`
was called with a `file=` that doesn't exist. Real FS path is checked
during bake mode. Fix the path or switch to the catalog (`assets.X`).

**Bake exits nonzero with "acquire_scene_assets: N unresolved
reference(s)"** — a scene references an id the catalog doesn't have. The
bake tallies every miss across every scene the startup script loads (it
doesn't stop at the first), so the log lists all of them. Add the file,
fix the id, or add the id to `preload` if it's runtime-spawned.

**"id collision 'X' between A and B"** — two files would derive the same
id. Set `meta.id = "...different..."` on one of them.

**Atlas pack fails** — group doesn't fit in one 4096² atlas. Split the
group (give some sprites a different `meta.atlas` value), shrink the art,
or move textures out of the group with `no_atlas = true`.

**`AudioNormalizer: format not supported`** — `normalize = true` on a
non-WAV file. Convert the source to PCM WAV, or drop `normalize` and ship
the original.

**Manifest hijacks dev iteration** — a stale `asset_manifest.lua` sitting
in the project root froze the catalog on an old snapshot. Dev builds gate
manifest loads off (`OCTARINE_SHIPPED=OFF` + no `--use-manifest`), but if
you flipped one of those, delete the manifest and re-run.

---

## Where to look in the source

| Concern | File |
|---------|------|
| Catalog model, sidecar parsing, scan / manifest paths | `src/AssetManager/AssetCatalog.{h,cpp}` |
| `.meta` field schema + defaults | `src/AssetManager/AssetMetadata.h` |
| Texture atlas packer (bake-time) | `src/AssetManager/TextureAtlasBaker.{h,cpp}` |
| Glyph atlas rasterizer (bake-time) | `src/AssetManager/AtlasBaker.{h,cpp}` |
| Glyph atlas runtime (load + lookup) | `src/AssetManager/GlyphAtlas.{h,cpp}` |
| Audio loudness normalize (bake-time) | `src/AssetManager/AudioNormalizer.{h,cpp}` |
| Scene asset scanner | `src/AssetManager/SceneAssetScanner.{h,cpp}` |
| `load_asset` / `acquire_scene_assets` Lua bindings | `src/Lua/Modules/SceneModuleLuaBinding.cpp` |
| Runtime acquire / refcount / release | `src/AssetManager/AssetManager.{h,cpp}` |
