#include "EditorPersistence.h"

#ifdef OCTARINE_WITH_EDITOR

#include <SDL3/SDL_filesystem.h>

#include <fstream>
#include <sstream>

#include "../General/Logger.h"

namespace {
constexpr auto kGlobalFileName = "editor_settings.ini";
constexpr auto kProjectFileName = "editor_prefs.ini";

std::string GlobalPath() {
  char* prefPath = SDL_GetPrefPath("Octarine", "Engine");
  if (!prefPath) return {};
  std::string out = std::string(prefPath) + kGlobalFileName;
  SDL_free(prefPath);
  return out;
}

std::string ProjectPath(const std::string& projectAssetPath) {
  if (projectAssetPath.empty()) return {};
  return projectAssetPath + "/" + kProjectFileName;
}
}  // namespace

void EditorPersistence::SaveGlobal() const {
  const auto path = GlobalPath();
  if (path.empty()) {
    Logger::Error("Failed to resolve editor global pref path: " + std::string(SDL_GetError()));
    return;
  }

  std::ofstream file(path);
  if (!file.is_open()) {
    Logger::Error("Failed to open editor global preferences file for writing: " + path);
    return;
  }

  file << "lastProjectPath=" << lastProjectPath << "\n";
  file << "editorFontSize=" << editorFontSize << "\n";
  file << "editorStyleIndex=" << editorStyleIndex << "\n";
}

void EditorPersistence::LoadGlobal() {
  const auto path = GlobalPath();
  if (path.empty()) return;

  std::ifstream file(path);
  if (!file.is_open()) return;

  std::string line;
  while (std::getline(file, line)) {
    const auto eq = line.find('=');
    if (eq == std::string::npos) continue;
    const auto key = line.substr(0, eq);
    const auto value = line.substr(eq + 1);

    if (key == "lastProjectPath") {
      lastProjectPath = value;
    } else if (key == "editorFontSize") {
      try { editorFontSize = std::stof(value); } catch (...) {}
    } else if (key == "editorStyleIndex") {
      try { editorStyleIndex = std::stoi(value); } catch (...) {}
    }
  }
}

void EditorPersistence::SaveProject(const std::string& projectAssetPath) const {
  const auto path = ProjectPath(projectAssetPath);
  if (path.empty()) return;

  std::ofstream file(path);
  if (!file.is_open()) {
    Logger::Error("Failed to open editor project preferences file for writing: " + path);
    return;
  }

  file << "currentScenePath=" << currentScenePath << "\n";
  file << "showProfiler=" << (showProfiler ? "true" : "false") << "\n";
  file << "showHierarchy=" << (showHierarchy ? "true" : "false") << "\n";
  file << "showAssetBrowser=" << (showAssetBrowser ? "true" : "false") << "\n";
  file << "showLuaConsole=" << (showLuaConsole ? "true" : "false") << "\n";
  file << "showSceneWindow=" << (showSceneWindow ? "true" : "false") << "\n";
  file << "showSceneManagement=" << (showSceneManagement ? "true" : "false") << "\n";
}

void EditorPersistence::LoadProject(const std::string& projectAssetPath) {
  const auto path = ProjectPath(projectAssetPath);
  if (path.empty()) return;

  std::ifstream file(path);
  if (!file.is_open()) return;  // Silent — fine if it doesn't exist yet.

  std::string line;
  while (std::getline(file, line)) {
    const auto eq = line.find('=');
    if (eq == std::string::npos) continue;
    const auto key = line.substr(0, eq);
    const auto value = line.substr(eq + 1);

    if (key == "currentScenePath") currentScenePath = value;
    else if (key == "showProfiler") showProfiler = (value == "true");
    else if (key == "showHierarchy") showHierarchy = (value == "true");
    else if (key == "showAssetBrowser") showAssetBrowser = (value == "true");
    else if (key == "showLuaConsole") showLuaConsole = (value == "true");
    else if (key == "showSceneWindow") showSceneWindow = (value == "true");
    else if (key == "showSceneManagement") showSceneManagement = (value == "true");
  }
}

#endif  // OCTARINE_WITH_EDITOR
