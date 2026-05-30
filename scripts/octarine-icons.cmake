# octarine-icons.cmake — single-source-of-truth icon/splash generator.
#
# Invoked from two callers via `cmake -P` (so Gradle and CMake share one implementation):
#   • cmake/OctarinePackage.cmake (desktop install paths)
#   • android/app/build.gradle    (Android pre-build res generation)
#
# Inputs (pass via -D...):
#   OCTARINE_ICON_PROJECT   absolute path to the game project dir (contains project.ini)
#   OCTARINE_ICON_PLATFORM  one of: android | desktop
#   OCTARINE_ICON_OUT_DIR   output root; per-platform layout written below it
#   OCTARINE_ICON_DESKTOP_OS (desktop only) windows | macos | linux
#
# project.ini keys consumed:
#   icon            path relative to project dir; should be a 1024×1024 PNG with safe padding
#                   (Android adaptive icons crop the outer ~16% — reserve it in your art).
#   splash_color    `#rrggbb` background for the Android 12+ splash. Defaults to `#000000`.
#
# Behavior on missing inputs (each is a non-fatal skip, so projects mid-bootstrap still build):
#   • icon= absent           → status message, no files emitted (platform falls back to template
#                              defaults: SDL green robot on Android, CPack default icons on desktop).
#   • magick/convert absent  → warning, no files emitted (same fallback path).
#   • icon file missing      → fatal error (a typoed path is a misconfig, not a fallback).
#
# Generated layout under OUT_DIR:
#   android/
#     mipmap-{m,h,xh,xxh,xxxh}dpi/ic_launcher.png
#     mipmap-{m,h,xh,xxh,xxxh}dpi/ic_launcher_foreground.png
#     mipmap-anydpi-v26/ic_launcher.xml
#     values/octarine_icon_colors.xml
#     values-v31/octarine_splash.xml           (overrides AppTheme w/ splash attrs on API 31+)
#   desktop/
#     (iOS Assets.xcassets/AppIcon.appiconset + LaunchScreen.storyboard emitter is parked on
#     defer/ios pending an Apple Developer account; see ai/iOSDeferralPlan.md.)
#     octarine_icon.ico                        (Windows; multi-resolution embed)
#     octarine_icon.icns                       (macOS; magick writes the .icns directly)
#     octarine_icon.png                        (Linux; 256×256 PNG for .desktop)

cmake_minimum_required(VERSION 3.20)

if (NOT DEFINED OCTARINE_ICON_PROJECT OR NOT DEFINED OCTARINE_ICON_PLATFORM OR NOT DEFINED OCTARINE_ICON_OUT_DIR)
    message(FATAL_ERROR "octarine-icons: missing -DOCTARINE_ICON_PROJECT / -DOCTARINE_ICON_PLATFORM / -DOCTARINE_ICON_OUT_DIR")
endif ()

# ---- project.ini parse --------------------------------------------------------------------------

set(_ini "${OCTARINE_ICON_PROJECT}/project.ini")
if (NOT EXISTS "${_ini}")
    message(STATUS "octarine-icons: no ${_ini} — skipping icon generation (defaults will ship).")
    return()
endif ()

set(_icon_rel "")
set(_splash_color "#000000")
file(STRINGS "${_ini}" _lines)
foreach (_line IN LISTS _lines)
    if (_line MATCHES "^[ \t]*#")
        continue()
    endif ()
    if (_line MATCHES "^[ \t]*([A-Za-z0-9_]+)[ \t]*=[ \t]*(.*)$")
        set(_k "${CMAKE_MATCH_1}")
        set(_v "${CMAKE_MATCH_2}")
        # Strip trailing whitespace AND any stray CR — file(STRINGS) usually normalizes line endings
        # on Windows, but PowerShell-emitted UTF-16/CRLF files occasionally slip a \r through.
        string(REGEX REPLACE "[ \t\r]+$" "" _v "${_v}")
        if (_k STREQUAL "icon")
            set(_icon_rel "${_v}")
        elseif (_k STREQUAL "splash_color")
            set(_splash_color "${_v}")
        endif ()
    endif ()
endforeach ()

if ("${_icon_rel}" STREQUAL "")
    message(STATUS "octarine-icons: no `icon=` in ${_ini} — skipping (default icon will ship).")
    return()
endif ()

# Resolve icon path relative to project dir; absolute paths pass through unchanged.
if (IS_ABSOLUTE "${_icon_rel}")
    set(_icon_src "${_icon_rel}")
else ()
    set(_icon_src "${OCTARINE_ICON_PROJECT}/${_icon_rel}")
endif ()
if (NOT EXISTS "${_icon_src}")
    message(FATAL_ERROR "octarine-icons: icon `${_icon_rel}` from project.ini not found at ${_icon_src}")
endif ()

# Validate splash_color — Android XML consumes the `#rrggbb` literal directly. We use
# string(REGEX MATCH) over `if(... MATCHES ...)` because CMake's `if(<var> MATCHES <re>)` mode
# treats the rhs as needing the var to be a quoted string, but a literal-looking value like
# "#112233" can be re-evaluated as a variable reference whose contents are then matched against
# itself — which fails because the empty regex doesn't match the value. string(REGEX MATCH) is
# unambiguous: rhs is always the pattern.
string(REGEX MATCH "^#[0-9A-Fa-f][0-9A-Fa-f][0-9A-Fa-f][0-9A-Fa-f][0-9A-Fa-f][0-9A-Fa-f]$" _sc_match "${_splash_color}")
if (NOT _sc_match)
    message(FATAL_ERROR "octarine-icons: splash_color `${_splash_color}` must be #rrggbb")
endif ()

# ---- ImageMagick locate -------------------------------------------------------------------------

find_program(_magick NAMES magick convert)
if (NOT _magick)
    message(WARNING "octarine-icons: ImageMagick (magick/convert) not on PATH — skipping icon generation. "
                    "Install it (apt: imagemagick, brew: imagemagick, choco: imagemagick) and re-run.")
    return()
endif ()
# `magick convert in out` is the IM7 form; IM6's `convert` takes the same args without the prefix.
get_filename_component(_magick_name "${_magick}" NAME_WE)
if (_magick_name STREQUAL "magick")
    set(_im_cmd "${_magick}")
else ()
    set(_im_cmd "${_magick}")  # IM6: convert directly
endif ()

# Helper: resize source into a square PNG.
function(_octarine_emit_png OUT_PATH SIZE)
    get_filename_component(_dir "${OUT_PATH}" DIRECTORY)
    file(MAKE_DIRECTORY "${_dir}")
    execute_process(
            COMMAND "${_im_cmd}" "${_icon_src}" -resize ${SIZE}x${SIZE} -background none -gravity center -extent ${SIZE}x${SIZE} "${OUT_PATH}"
            RESULT_VARIABLE _rc
            ERROR_VARIABLE _err)
    if (NOT _rc EQUAL 0)
        message(FATAL_ERROR "octarine-icons: magick failed (${SIZE}px → ${OUT_PATH}):\n${_err}")
    endif ()
endfunction()

# ---- platform dispatch --------------------------------------------------------------------------

if (OCTARINE_ICON_PLATFORM STREQUAL "android")
    # Legacy launcher: full-bleed icon at each mipmap density.
    set(_android_legacy
        mdpi    48
        hdpi    72
        xhdpi   96
        xxhdpi  144
        xxxhdpi 192)
    # Adaptive foreground: 108×108 dp safe area inside a 432×432 canvas at xxxhdpi.
    set(_android_foreground
        mdpi    108
        hdpi    162
        xhdpi   216
        xxhdpi  324
        xxxhdpi 432)
    set(_root "${OCTARINE_ICON_OUT_DIR}/android")

    foreach (_pair IN ITEMS legacy foreground)
        if (_pair STREQUAL "legacy")
            set(_list "${_android_legacy}")
            set(_name "ic_launcher")
        else ()
            set(_list "${_android_foreground}")
            set(_name "ic_launcher_foreground")
        endif ()
        list(LENGTH _list _n)
        math(EXPR _last "${_n} - 1")
        foreach (_i RANGE 0 ${_last} 2)
            list(GET _list ${_i} _density)
            math(EXPR _ip1 "${_i} + 1")
            list(GET _list ${_ip1} _px)
            _octarine_emit_png("${_root}/mipmap-${_density}/${_name}.png" ${_px})
        endforeach ()
    endforeach ()

    # Adaptive icon descriptor: background drawable + foreground mipmap. API 26+ honors it; older
    # APIs fall through to the legacy mipmap-*/ic_launcher.png squares above.
    file(MAKE_DIRECTORY "${_root}/mipmap-anydpi-v26")
    file(WRITE "${_root}/mipmap-anydpi-v26/ic_launcher.xml"
"<?xml version=\"1.0\" encoding=\"utf-8\"?>
<adaptive-icon xmlns:android=\"http://schemas.android.com/apk/res/android\">
    <background android:drawable=\"@color/ic_launcher_background\" />
    <foreground android:drawable=\"@mipmap/ic_launcher_foreground\" />
</adaptive-icon>
")
    file(MAKE_DIRECTORY "${_root}/values")
    file(WRITE "${_root}/values/octarine_icon_colors.xml"
"<?xml version=\"1.0\" encoding=\"utf-8\"?>
<resources>
    <color name=\"ic_launcher_background\">${_splash_color}</color>
</resources>
")

    # Android 12+ splash. Overrides AppTheme so the platform splash uses our color + icon. Pre-12
    # devices keep the AppTheme from values/styles.xml (no splash mechanism, launcher icon flashes).
    file(MAKE_DIRECTORY "${_root}/values-v31")
    file(WRITE "${_root}/values-v31/octarine_splash.xml"
"<?xml version=\"1.0\" encoding=\"utf-8\"?>
<resources>
    <style name=\"AppTheme\" parent=\"android:Theme.NoTitleBar.Fullscreen\">
        <item name=\"android:windowSplashScreenBackground\">${_splash_color}</item>
        <item name=\"android:windowSplashScreenAnimatedIcon\">@mipmap/ic_launcher_foreground</item>
    </style>
</resources>
")

    message(STATUS "octarine-icons: android resources emitted under ${_root}")

elseif (OCTARINE_ICON_PLATFORM STREQUAL "desktop")
    if (NOT DEFINED OCTARINE_ICON_DESKTOP_OS)
        message(FATAL_ERROR "octarine-icons: desktop platform requires -DOCTARINE_ICON_DESKTOP_OS=windows|macos|linux")
    endif ()
    set(_root "${OCTARINE_ICON_OUT_DIR}/desktop")
    file(MAKE_DIRECTORY "${_root}")

    if (OCTARINE_ICON_DESKTOP_OS STREQUAL "windows")
        # Multi-resolution .ico. Windows picks the closest match at runtime.
        execute_process(
                COMMAND "${_im_cmd}" "${_icon_src}" -define icon:auto-resize=256,128,64,48,32,16
                        "${_root}/octarine_icon.ico"
                RESULT_VARIABLE _rc ERROR_VARIABLE _err)
        if (NOT _rc EQUAL 0)
            message(FATAL_ERROR "octarine-icons: .ico emit failed:\n${_err}")
        endif ()
        message(STATUS "octarine-icons: ${_root}/octarine_icon.ico")
    elseif (OCTARINE_ICON_DESKTOP_OS STREQUAL "macos")
        # ImageMagick writes .icns directly (single-image; macOS scales for the various Finder
        # sizes). A more polished build would use `iconutil` to pack an iconset of multiple
        # resolutions — defer that until someone needs sharper Finder thumbnails.
        execute_process(
                COMMAND "${_im_cmd}" "${_icon_src}" -resize 1024x1024
                        "${_root}/octarine_icon.icns"
                RESULT_VARIABLE _rc ERROR_VARIABLE _err)
        if (NOT _rc EQUAL 0)
            message(FATAL_ERROR "octarine-icons: .icns emit failed:\n${_err}")
        endif ()
        message(STATUS "octarine-icons: ${_root}/octarine_icon.icns")
    elseif (OCTARINE_ICON_DESKTOP_OS STREQUAL "linux")
        _octarine_emit_png("${_root}/octarine_icon.png" 256)
        message(STATUS "octarine-icons: ${_root}/octarine_icon.png")
    else ()
        message(FATAL_ERROR "octarine-icons: unknown OCTARINE_ICON_DESKTOP_OS=${OCTARINE_ICON_DESKTOP_OS}")
    endif ()

else ()
    message(FATAL_ERROR "octarine-icons: unknown OCTARINE_ICON_PLATFORM=${OCTARINE_ICON_PLATFORM}")
endif ()
