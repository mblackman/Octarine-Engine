#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <SDL3/SDL.h>

// SDL file/folder dialog callbacks for the editor. Each receives the Game* via `userdata`
// (passed through SDL_ShowOpen*Dialog) and writes the chosen path back into EditorPersistence.
namespace octarine::editor::panels {

/// Folder-picker callback for the Project Selector. Stores the selected folder as the project to
/// reopen and persists it globally.
void SDLCALL OnProjectFolderSelected(void* userdata, const char* const* filelist, int filter);

/// File-picker callback for Scene Management. Stores a project-relative scene path when the file
/// lives inside the project, otherwise the absolute path.
void SDLCALL OnSceneFileSelected(void* userdata, const char* const* filelist, int filter);

}  // namespace octarine::editor::panels

#endif  // OCTARINE_WITH_EDITOR
