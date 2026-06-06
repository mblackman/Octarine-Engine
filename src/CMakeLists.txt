# Engine layer split. Each STATIC library represents one architectural layer; PUBLIC
# link deps enforce the dependency direction at link time. Headers under src/ are reachable
# from every consumer (octarine_core publishes the include dir) — layering is enforced via
# symbol resolution, not via per-dir include walls.
#
# Layer map:
#   octarine_core      General/, ECS/, Components/, EventBus/, Events/, Game/GameConfig,
#                      Process/, Project/  (POD/data + cross-cutting utilities)
#   octarine_assets    AssetManager/ (catalog, atlas bakers, audio normalizer, pak)
#   octarine_renderer  Renderer/
#   octarine_lua       Lua/Bindings, Lua/Modules, Lua/HotReload, Lua/LuaApiManifest
#   octarine_systems   Systems/ (minus RenderDebugGUISystem.cpp; that's editor-only)
#   octarine_editor    Editor/ + Systems/RenderDebugGUISystem.cpp + Secrets/* (per-platform)
#                      (gated by OCTARINE_WITH_EDITOR / OCTARINE_WITH_IMGUI)
#   octarine_engine    Game/Game + Engine/EngineBootstrap (top-level coordinator)
#
# OctarineEngine (the desktop exe / Android shared lib) is declared in the root CMakeLists.txt
# and links octarine_engine only — everything else flows through PUBLIC deps.

# -----------------------------------------------------------------------------
# octarine_core — POD/data foundation. No SDL_image, no SDL_mixer, no imgui.
# -----------------------------------------------------------------------------
set(OCTARINE_CORE_SOURCES
        General/Logger.cpp
        ECS/Registry.cpp
        Game/GameConfig.cpp
        Deploy/Adb.cpp
        Process/Process.cpp
        Project/ProjectIni.cpp
)
if (UNIX AND NOT APPLE AND NOT ANDROID)
    list(APPEND OCTARINE_CORE_SOURCES General/SDL3_Linux_Stubs.cpp)
endif ()

octarine_library(octarine_core ${OCTARINE_CORE_SOURCES})
target_link_libraries(octarine_core PUBLIC
        glm::glm
        SDL3::SDL3
        spdlog::spdlog
        sol2::sol2
        ${LUA_LIBRARIES}
        Threads::Threads
)

# Compile defines shared by every layer (consumers inherit via PUBLIC).
target_compile_definitions(octarine_core PUBLIC GLM_FORCE_INTRINSICS)
target_compile_definitions(octarine_core PUBLIC OCTARINE_LOG_LEVEL_NAME="${_octarine_default_log_level}")
if (OCTARINE_ENABLE_PROFILING)
    target_compile_definitions(octarine_core PUBLIC OCTARINE_PROFILING)
endif ()
if (OCTARINE_WITH_IMGUI)
    target_compile_definitions(octarine_core PUBLIC OCTARINE_WITH_IMGUI)
endif ()
if (OCTARINE_WITH_EDITOR)
    target_compile_definitions(octarine_core PUBLIC OCTARINE_WITH_EDITOR)
endif ()
if (OCTARINE_SHIPPED)
    target_compile_definitions(octarine_core PUBLIC OCTARINE_SHIPPED)
endif ()

# -----------------------------------------------------------------------------
# octarine_assets — texture / font / audio handle store + catalog + bakers + pak.
# Per-file warning suppressions cover vendored stb single-header includes
# (TextureAtlasBaker, AtlasBaker, GlyphAtlas) and the BS.1770 size_t→double math
# in AudioNormalizer that trips the engine's strict -Wconversion baseline.
# -----------------------------------------------------------------------------
octarine_library(octarine_assets
        AssetManager/AssetCatalog.cpp
        AssetManager/AssetManager.cpp
        AssetManager/AssetPak.cpp
        AssetManager/AtlasBaker.cpp
        AssetManager/AudioClipStore.cpp
        AssetManager/AudioNormalizer.cpp
        AssetManager/FontStore.cpp
        AssetManager/GlyphAtlas.cpp
        AssetManager/TextureAtlasBaker.cpp
        AssetManager/TextureStore.cpp
        AssetManager/SceneAssetScanner.cpp
)
set_source_files_properties(
        AssetManager/TextureAtlasBaker.cpp
        AssetManager/AtlasBaker.cpp
        AssetManager/GlyphAtlas.cpp
        AssetManager/AudioNormalizer.cpp
        PROPERTIES COMPILE_OPTIONS "$<IF:$<CXX_COMPILER_ID:MSVC>,/w,-w>")
target_link_libraries(octarine_assets PUBLIC
        octarine_core
        SDL3_ttf::SDL3_ttf
        $<IF:$<TARGET_EXISTS:SDL3_mixer::SDL3_mixer>,SDL3_mixer::SDL3_mixer,SDL3_mixer::SDL3_mixer-static>
        $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,SDL3_image::SDL3_image-shared,SDL3_image::SDL3_image-static>
)
# libs/stb is its own include root so TextureAtlasBaker.cpp / AtlasBaker.cpp / GlyphAtlas.cpp can
# write `#include "stb_image.h"` (matches the pattern in scripts/octarine-icon-tool/).
target_include_directories(octarine_assets PRIVATE ${CMAKE_SOURCE_DIR}/libs/stb)

# -----------------------------------------------------------------------------
# octarine_renderer — render-queue producer drain + SDL_Renderer wrapper.
# -----------------------------------------------------------------------------
octarine_library(octarine_renderer
        Renderer/Renderer.cpp
)
target_link_libraries(octarine_renderer PUBLIC
        octarine_core
        octarine_assets
)

# -----------------------------------------------------------------------------
# octarine_lua — sol2 component/module bindings + EmmyLua API manifest + hot reload.
# -----------------------------------------------------------------------------
octarine_library(octarine_lua
        Lua/LuaApiManifest.cpp
        Lua/Bindings/InputSystemLuaBinding.cpp
        Lua/Bindings/RegisterAllBindings.cpp
        Lua/HotReload/ScriptHotReload.cpp
        Lua/HotReload/ScriptWatcher.cpp
        Lua/Modules/AssetsModuleLuaBinding.cpp
        Lua/Modules/AudioModuleLuaBinding.cpp
        Lua/Modules/EntityModuleLuaBinding.cpp
        Lua/Modules/GameModuleLuaBinding.cpp
        Lua/Modules/IoModuleLuaBinding.cpp
        Lua/Modules/LogModuleLuaBinding.cpp
        Lua/Modules/RegisterAllModules.cpp
        Lua/Modules/SceneModuleLuaBinding.cpp
)
target_link_libraries(octarine_lua PUBLIC
        octarine_core
        octarine_assets
        octarine_renderer
)

# -----------------------------------------------------------------------------
# octarine_systems — per-frame Update systems + event-driven systems.
# RenderDebugGUISystem (editor/IMGUI-only) lives in octarine_editor instead.
# -----------------------------------------------------------------------------
octarine_library(octarine_systems
        Systems/AudioSystem.cpp
        Systems/InputSystem.cpp
)
target_link_libraries(octarine_systems PUBLIC
        octarine_core
        octarine_assets
        octarine_renderer
        octarine_lua
)

# -----------------------------------------------------------------------------
# octarine_editor — desktop-only editor UI + inspectors + RenderDebugGUISystem +
# SecretStore (per-platform credential storage backing the editor's signing
# secrets surface). Gated by OCTARINE_WITH_EDITOR; OCTARINE_WITH_IMGUI alone
# (player-debug build) still needs RenderDebugGUISystem, so the lib is created
# when EITHER is on and selectively compiles the editor-specific .cpps.
# -----------------------------------------------------------------------------
if (OCTARINE_WITH_IMGUI OR OCTARINE_WITH_EDITOR)
    set(OCTARINE_EDITOR_SOURCES "")
    if (OCTARINE_WITH_IMGUI)
        list(APPEND OCTARINE_EDITOR_SOURCES Systems/RenderDebugGUISystem.cpp)
    endif ()
    if (OCTARINE_WITH_EDITOR)
        list(APPEND OCTARINE_EDITOR_SOURCES
                Editor/EditorLayoutPresets.cpp
                Editor/EditorPersistence.cpp
                Editor/ExportBuilder.cpp
                Editor/PlayerLauncher.cpp
                Editor/Inspectors/RegisterAllInspectors.cpp
                # Editor UI panels (Stage 9 split of RenderDebugGUISystem). Each owns one window /
                # service; RenderDebugGUISystem keeps only the IMGUI-shared frame begin/end + game
                # overlays and delegates the editor chrome to EditorPanelHost.
                Editor/Panels/EditorAssetBrowserPanel.cpp
                Editor/Panels/EditorBuildDialog.cpp
                Editor/Panels/EditorConsolePanel.cpp
                Editor/Panels/EditorDevicesPanel.cpp
                Editor/Panels/EditorDialogService.cpp
                Editor/Panels/EditorHierarchyPanel.cpp
                Editor/Panels/EditorImGuiBackend.cpp
                Editor/Panels/EditorOutputPanels.cpp
                Editor/Panels/EditorPanelHost.cpp
                Editor/Panels/EditorProjectPanels.cpp
                Editor/Panels/EditorSceneWindow.cpp
                Editor/Panels/EditorSettingsPanels.cpp
                Editor/Panels/EditorSigningPanel.cpp
                Editor/Panels/EditorToolbarPanel.cpp
        )
        # SecretStore: one backend per host so the OS-native API links cleanly without
        # cross-platform shims. Linux backend is a no-op stub the editor UI falls back
        # to env-var-only mode on.
        if (WIN32)
            list(APPEND OCTARINE_EDITOR_SOURCES Secrets/SecretStore_Windows.cpp)
        elseif (APPLE)
            list(APPEND OCTARINE_EDITOR_SOURCES Secrets/SecretStore_macOS.cpp)
        else ()
            list(APPEND OCTARINE_EDITOR_SOURCES Secrets/SecretStore_Linux.cpp)
        endif ()
    endif ()
    octarine_library(octarine_editor ${OCTARINE_EDITOR_SOURCES})
    target_link_libraries(octarine_editor PUBLIC
            octarine_core
            octarine_assets
            octarine_renderer
            octarine_lua
            octarine_systems
    )
    if (OCTARINE_WITH_IMGUI)
        target_link_libraries(octarine_editor PUBLIC imgui::imgui sol2_ImGui_Bindings::sol2_ImGui_Bindings)
    endif ()
    # SecretStore backend libs. Crypt32 ships DPAPI's CryptProtectData/CryptUnprotectData on
    # Windows; Security.framework ships SecItem* on macOS. Linux backend is a stub and needs no
    # link. Editor-only — gated alongside the editor source compile above.
    if (OCTARINE_WITH_EDITOR)
        if (WIN32)
            target_link_libraries(octarine_editor PUBLIC crypt32)
        elseif (APPLE)
            target_link_libraries(octarine_editor PUBLIC
                    "-framework CoreFoundation"
                    "-framework Security")
        endif ()
    endif ()
endif ()

# -----------------------------------------------------------------------------
# octarine_engine — top-level coordinator. Owns Game/Game.cpp + EngineBootstrap, pulls every
# downstream lib in transitively. Editor lives on the PUBLIC link line when built so its
# symbols (RenderDebugGUISystem::Render, EditorPersistence::*, SecretStore::*) resolve at the
# final exe link without forcing the engine TU to know about editor concretely.
# -----------------------------------------------------------------------------
set(OCTARINE_ENGINE_SOURCES
        Engine/EditorBootstrap.cpp
        Engine/EngineBootstrap.cpp
        Engine/EngineRuntime.cpp
        Engine/FrameLoop.cpp
        Engine/Platform/PlatformPaths.cpp
        Engine/SceneLoader.cpp
        Game/Game.cpp
)
# DevListenServer (Stage 6): TCP listener for the dev iterate loop. Available in editor + player
# presets; compile-stripped from ship-release (OCTARINE_SHIPPED).
if (NOT OCTARINE_SHIPPED)
    list(APPEND OCTARINE_ENGINE_SOURCES Dev/DevListenServer.cpp Dev/DevListenDispatch.cpp)
endif ()
octarine_library(octarine_engine ${OCTARINE_ENGINE_SOURCES})
target_link_libraries(octarine_engine PUBLIC
        octarine_core
        octarine_assets
        octarine_renderer
        octarine_lua
        octarine_systems
)
if (TARGET octarine_editor)
    target_link_libraries(octarine_engine PUBLIC octarine_editor)
endif ()
# Winsock provides the BSD-socket API (ws2_32) on Windows; POSIX sockets come from libc so no
# extra link is needed there. Only when the dev server is compiled in (non-shipped builds).
if (NOT OCTARINE_SHIPPED AND WIN32)
    target_link_libraries(octarine_engine PUBLIC ws2_32)
endif ()
