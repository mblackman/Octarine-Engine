# octarine-icons.cmake — single-source-of-truth icon/splash generator.
#
# Invoked from two callers via `cmake -P` (so Gradle and CMake share one implementation):
#   • cmake/OctarinePackage.cmake (iOS + desktop install paths)
#   • android/app/build.gradle    (Android pre-build res generation)
#
# Inputs (pass via -D...):
#   OCTARINE_ICON_PROJECT   absolute path to the game project dir (contains project.ini)
#   OCTARINE_ICON_PLATFORM  one of: android | ios | desktop
#   OCTARINE_ICON_OUT_DIR   output root; per-platform layout written below it
#   OCTARINE_ICON_DESKTOP_OS (desktop only) windows | macos | linux
#
# project.ini keys consumed:
#   icon            path relative to project dir; should be a 1024×1024 PNG with safe padding
#                   (Android adaptive icons crop the outer ~16% — reserve it in your art).
#   splash_color    `#rrggbb` background for the iOS LaunchScreen and the Android 12+ splash.
#                   Defaults to `#000000` when unset.
#
# Behavior on missing inputs (each is a non-fatal skip, so projects mid-bootstrap still build):
#   • icon= absent           → status message, no files emitted (platform falls back to template
#                              defaults: SDL green robot on Android, black launch screen on iOS,
#                              CPack default icons on desktop).
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
#   ios/
#     Assets.xcassets/Contents.json
#     Assets.xcassets/AppIcon.appiconset/Contents.json + icon-*.png ladder
#     LaunchScreen.storyboard                  (solid splash_color background)
#   desktop/
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

# Validate splash_color and split into 0..1 floats for storyboard color insertion. We use
# string(REGEX MATCH) over `if(... MATCHES ...)` because CMake's `if(<var> MATCHES <re>)` mode
# treats the rhs as needing the var to be a quoted string, but a literal-looking value like
# "#112233" can be re-evaluated as a variable reference whose contents are then matched against
# itself — which fails because the empty regex doesn't match the value. string(REGEX MATCH) is
# unambiguous: rhs is always the pattern.
string(REGEX MATCH "^#[0-9A-Fa-f][0-9A-Fa-f][0-9A-Fa-f][0-9A-Fa-f][0-9A-Fa-f][0-9A-Fa-f]$" _sc_match "${_splash_color}")
if (NOT _sc_match)
    message(FATAL_ERROR "octarine-icons: splash_color `${_splash_color}` must be #rrggbb")
endif ()
string(SUBSTRING "${_splash_color}" 1 2 _hex_r)
string(SUBSTRING "${_splash_color}" 3 2 _hex_g)
string(SUBSTRING "${_splash_color}" 5 2 _hex_b)
math(EXPR _i_r "0x${_hex_r}")
math(EXPR _i_g "0x${_hex_g}")
math(EXPR _i_b "0x${_hex_b}")
# CMake math() is integer-only; emit color floats as "N / 255" strings the storyboard XML can hold.
set(_f_r "${_i_r}")
set(_f_g "${_i_g}")
set(_f_b "${_i_b}")

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

elseif (OCTARINE_ICON_PLATFORM STREQUAL "ios")
    set(_root "${OCTARINE_ICON_OUT_DIR}/ios")
    set(_xcassets "${_root}/Assets.xcassets")
    set(_appicon "${_xcassets}/AppIcon.appiconset")
    file(MAKE_DIRECTORY "${_appicon}")

    # AppIcon ladder. App Store review requires the 1024 marketing icon; device sizes round out the
    # ladder so a tap on a home screen, settings, spotlight, or notification picks the right asset.
    # Format: idiom, scale, size_pt, px (size_pt*scale), filename.
    set(_icons
        "iphone 2x 20 40 icon-20@2x.png"
        "iphone 3x 20 60 icon-20@3x.png"
        "iphone 2x 29 58 icon-29@2x.png"
        "iphone 3x 29 87 icon-29@3x.png"
        "iphone 2x 40 80 icon-40@2x.png"
        "iphone 3x 40 120 icon-40@3x.png"
        "iphone 2x 60 120 icon-60@2x.png"
        "iphone 3x 60 180 icon-60@3x.png"
        "ipad 1x 20 20 icon-ipad-20.png"
        "ipad 2x 20 40 icon-ipad-20@2x.png"
        "ipad 1x 29 29 icon-ipad-29.png"
        "ipad 2x 29 58 icon-ipad-29@2x.png"
        "ipad 1x 40 40 icon-ipad-40.png"
        "ipad 2x 40 80 icon-ipad-40@2x.png"
        "ipad 1x 76 76 icon-ipad-76.png"
        "ipad 2x 76 152 icon-ipad-76@2x.png"
        "ipad 2x 83.5 167 icon-ipad-83.5@2x.png"
        "ios-marketing 1x 1024 1024 icon-marketing.png")

    set(_contents_imgs "")
    foreach (_row IN LISTS _icons)
        string(REPLACE " " ";" _parts "${_row}")
        list(GET _parts 0 _idiom)
        list(GET _parts 1 _scale)
        list(GET _parts 2 _size_pt)
        list(GET _parts 3 _size_px)
        list(GET _parts 4 _fname)
        _octarine_emit_png("${_appicon}/${_fname}" ${_size_px})
        # Build Contents.json entry. Marketing icon uses platform=ios; the rest use idiom only.
        if (_idiom STREQUAL "ios-marketing")
            set(_entry "    {\n      \"idiom\" : \"${_idiom}\",\n      \"size\" : \"${_size_pt}x${_size_pt}\",\n      \"scale\" : \"${_scale}\",\n      \"filename\" : \"${_fname}\"\n    }")
        else ()
            set(_entry "    {\n      \"idiom\" : \"${_idiom}\",\n      \"size\" : \"${_size_pt}x${_size_pt}\",\n      \"scale\" : \"${_scale}\",\n      \"filename\" : \"${_fname}\"\n    }")
        endif ()
        list(APPEND _contents_imgs "${_entry}")
    endforeach ()
    string(REPLACE ";" ",\n" _images_block "${_contents_imgs}")
    file(WRITE "${_appicon}/Contents.json"
"{
  \"images\" : [
${_images_block}
  ],
  \"info\" : {
    \"author\" : \"octarine\",
    \"version\" : 1
  }
}
")
    file(WRITE "${_xcassets}/Contents.json"
"{
  \"info\" : {
    \"author\" : \"octarine\",
    \"version\" : 1
  }
}
")

    # LaunchScreen: solid splash_color background. Apple just needs a valid storyboard to suppress
    # the default black launch flash; branding lives in the icon itself once the app draws.
    # Storyboard colors are 0..1 floats — emit as `N / 255.0` strings (Xcode accepts arithmetic? no,
    # needs floats). CMake has no float type, so format manually.
    function(_int_to_float OUT IN)
        # IN is 0..255; emit "x.yyy" string.
        math(EXPR _whole "${IN} * 1000 / 255")
        math(EXPR _w "${_whole} / 1000")
        math(EXPR _f "${_whole} - (${_w} * 1000)")
        # Zero-pad the fractional part to 3 digits so 0.05 doesn't round-trip as 0.5.
        if (_f LESS 10)
            set(_f "00${_f}")
        elseif (_f LESS 100)
            set(_f "0${_f}")
        endif ()
        set(${OUT} "${_w}.${_f}" PARENT_SCOPE)
    endfunction()
    _int_to_float(_fr ${_i_r})
    _int_to_float(_fg ${_i_g})
    _int_to_float(_fb ${_i_b})

    file(WRITE "${_root}/LaunchScreen.storyboard"
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<document type=\"com.apple.InterfaceBuilder3.CocoaTouch.Storyboard.XIB\" version=\"3.0\" toolsVersion=\"22155\" targetRuntime=\"iOS.CocoaTouch\" propertyAccessControl=\"none\" useAutolayout=\"YES\" launchScreen=\"YES\" useTraitCollections=\"YES\" useSafeAreas=\"YES\" colorMatched=\"YES\" initialViewController=\"01J-lp-oVM\">
    <device id=\"retina6_1\" orientation=\"portrait\" appearance=\"light\"/>
    <dependencies>
        <plugIn identifier=\"com.apple.InterfaceBuilder.IBCocoaTouchPlugin\" version=\"22131\"/>
        <capability name=\"Safe area layout guides\" minToolsVersion=\"9.0\"/>
        <capability name=\"documents saved in the Xcode 8 format\" minToolsVersion=\"8.0\"/>
    </dependencies>
    <scenes>
        <scene sceneID=\"EHf-IW-A2E\">
            <objects>
                <viewController id=\"01J-lp-oVM\" sceneMemberID=\"viewController\">
                    <view key=\"view\" contentMode=\"scaleToFill\" id=\"Ze5-6b-2t3\">
                        <rect key=\"frame\" x=\"0.0\" y=\"0.0\" width=\"375\" height=\"667\"/>
                        <autoresizingMask key=\"autoresizingMask\" widthSizable=\"YES\" heightSizable=\"YES\"/>
                        <viewLayoutGuide key=\"safeArea\" id=\"Bcu-3y-fUS\"/>
                        <color key=\"backgroundColor\" red=\"${_fr}\" green=\"${_fg}\" blue=\"${_fb}\" alpha=\"1\" colorSpace=\"custom\" customColorSpace=\"sRGB\"/>
                    </view>
                </viewController>
                <placeholder placeholderIdentifier=\"IBFirstResponder\" id=\"iYj-Kq-Ea1\" userLabel=\"First Responder\" sceneMemberID=\"firstResponder\"/>
            </objects>
        </scene>
    </scenes>
</document>
")
    message(STATUS "octarine-icons: iOS appiconset + LaunchScreen emitted under ${_root}")

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
