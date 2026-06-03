#include "EditorPersistence.h"

#ifdef OCTARINE_WITH_EDITOR

#include <SDL3/SDL_filesystem.h>

#include <cctype>
#include <fstream>
#include <sstream>

#include "General/Logger.h"

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

bool ParseIniLine(const std::string& line, std::string& key, std::string& value) {
  const auto eq = line.find('=');
  if (eq == std::string::npos) return false;
  key = line.substr(0, eq);
  value = line.substr(eq + 1);
  // Trim trailing whitespace / CR so CRLF files and hand edits round-trip cleanly.
  while (!value.empty() && std::isspace(static_cast<unsigned char>(value.back()))) {
    value.pop_back();
  }
  return true;
}

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
  file << "audioMuted=" << (audioMuted ? "true" : "false") << "\n";
  file << "masterVolume=" << masterVolume << "\n";
}

void EditorPersistence::LoadGlobal() {
  const auto path = GlobalPath();
  if (path.empty()) return;

  std::ifstream file(path);
  if (!file.is_open()) return;

  std::string line;
  std::string key;
  std::string value;
  while (std::getline(file, line)) {
    if (!ParseIniLine(line, key, value)) continue;

    if (key == "lastProjectPath") {
      lastProjectPath = value;
    } else if (key == "editorFontSize") {
      try {
        editorFontSize = std::stof(value);
      } catch (...) {
      }
    } else if (key == "editorStyleIndex") {
      try {
        editorStyleIndex = std::stoi(value);
      } catch (...) {
      }
    } else if (key == "audioMuted") {
      audioMuted = (value == "true");
    } else if (key == "masterVolume") {
      try {
        masterVolume = std::stof(value);
      } catch (...) {
      }
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
  for (const auto& [key, member] : kWindowFlags) {
    file << key << "=" << (this->*member ? "true" : "false") << "\n";
  }
}

void EditorPersistence::LoadProject(const std::string& projectAssetPath) {
  const auto path = ProjectPath(projectAssetPath);
  if (path.empty()) return;

  std::ifstream file(path);
  if (!file.is_open()) return;  // Silent — fine if it doesn't exist yet.

  std::string line;
  std::string key;
  std::string value;
  while (std::getline(file, line)) {
    if (!ParseIniLine(line, key, value)) continue;

    if (key == "currentScenePath") {
      currentScenePath = value;
      continue;
    }
    for (const auto& [flagKey, member] : kWindowFlags) {
      if (key == flagKey) {
        this->*member = (value == "true");
        break;
      }
    }
  }
}

#endif  // OCTARINE_WITH_EDITOR
