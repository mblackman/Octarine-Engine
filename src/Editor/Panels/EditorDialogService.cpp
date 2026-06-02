#include "Editor/Panels/EditorDialogService.h"

#ifdef OCTARINE_WITH_EDITOR

#include <filesystem>
#include <string>
#include <system_error>

#include "AssetManager/AssetManager.h"
#include "Editor/EditorPersistence.h"
#include "Game/Game.h"
#include "General/Logger.h"

namespace octarine::editor::panels {

void SDLCALL OnProjectFolderSelected(void* userdata, const char* const* filelist, int /*filter*/) {
  auto* game = static_cast<Game*>(userdata);
  if (filelist && *filelist) {
    auto& editorPersistence = game->GetRegistry()->Get<EditorPersistence>();
    editorPersistence.lastProjectPath = *filelist;
    editorPersistence.SaveGlobal();
    Logger::Info("Project folder selected: " + std::string(*filelist));
  }
}

void SDLCALL OnSceneFileSelected(void* userdata, const char* const* filelist, int /*filter*/) {
  auto* game = static_cast<Game*>(userdata);
  if (filelist && *filelist) {
    auto& editorPersistence = game->GetRegistry()->Get<EditorPersistence>();
    auto& assetManager = game->GetRegistry()->Get<AssetManager>();
    const std::filesystem::path selectedPath(*filelist);
    const std::filesystem::path basePath(assetManager.GetBasePath());

    // Prefer a project-relative path (e.g. "scripts/level1.lua"), but only when the file
    // actually lives inside the project — a relative result starting with ".." escapes it,
    // so keep the absolute path in that case.
    std::error_code ec;
    const std::filesystem::path relativePath = std::filesystem::relative(selectedPath, basePath, ec);
    const bool insideProject = !ec && !relativePath.empty() && relativePath.begin()->string() != "..";
    const std::string scenePath = insideProject ? relativePath.generic_string() : selectedPath.string();

    editorPersistence.currentScenePath = scenePath;
    Logger::Info("Scene file selected: " + scenePath);
  }
}

}  // namespace octarine::editor::panels

#endif  // OCTARINE_WITH_EDITOR
