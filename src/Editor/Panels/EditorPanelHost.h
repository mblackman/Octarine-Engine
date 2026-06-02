#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <SDL3/SDL.h>

class Game;
struct EditorPersistence;

// Editor chrome orchestration. RenderDebugGUISystem::Render calls these four hooks around the
// ImGui frame; the host owns the dockspace, toolbar, modals, and dispatch to the individual panel
// files under Editor/Panels/.
namespace octarine::editor::panels {

/// Apply a layout preset queued via EditorPersistence::pendingLayoutLoad. Call before NewFrame.
void ApplyPendingLayout(EditorPersistence& persistence);

/// Draw the full editor UI for this frame: dockspace, menu bar + playback toolbar, build modals,
/// and every show-flagged panel. Call between NewFrame and Render while in an editor session.
void DrawEditorChrome(Game* game, SDL_Texture* gameTexture);

/// Draw the Project Selector when no project is loaded or the user opened it from the File menu.
/// Drawn after the game overlays so it floats above them. Call between NewFrame and Render.
void DrawProjectSelectorIfNeeded(Game* game, bool projectLoaded);

/// Honor a deferred font-atlas rebuild requested from Editor Settings. Call AFTER ImGui::Render,
/// since atlas rebuilds inside an active frame corrupt draw state.
void ProcessFontRebuild(EditorPersistence& persistence);

}  // namespace octarine::editor::panels

#endif  // OCTARINE_WITH_EDITOR
