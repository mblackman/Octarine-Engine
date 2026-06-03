#include "EditorLayoutPresets.h"

#ifdef OCTARINE_WITH_EDITOR

#include <SDL3/SDL_filesystem.h>
#include <imgui.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include "EditorPersistence.h"
#include "General/Logger.h"
#include "Layouts/DefaultLayout.h"

namespace octarine::editor::layouts {

namespace {

constexpr auto kPresetExtension = ".layout";
constexpr auto kImGuiSectionHeader = "[imgui]";

std::filesystem::path LayoutsDir() {
  char* prefPath = SDL_GetPrefPath("Octarine", "Engine");
  if (!prefPath) return {};
  std::filesystem::path out = std::filesystem::path(prefPath) / "layouts";
  SDL_free(prefPath);
  return out;
}

std::filesystem::path PresetPath(const std::string& name) {
  auto dir = LayoutsDir();
  if (dir.empty() || name.empty()) return {};
  return dir / (name + kPresetExtension);
}

std::string SerializeFlags(const EditorPersistence& p) {
  std::ostringstream out;
  for (const auto& [key, member] : EditorPersistence::kWindowFlags) {
    out << key << "=" << (p.*member ? "true" : "false") << "\n";
  }
  return out.str();
}

void ApplyParsedContent(const std::string& content, EditorPersistence& p) {
  std::string imguiBlob;
  std::istringstream stream(content);
  std::string line;
  bool inImGuiSection = false;

  while (std::getline(stream, line)) {
    // Trim trailing whitespace (including \r from CRLF files)
    while (!line.empty() && std::isspace(static_cast<unsigned char>(line.back()))) {
      line.pop_back();
    }
    if (line.empty() && !inImGuiSection) continue;

    if (!inImGuiSection && line == kImGuiSectionHeader) {
      inImGuiSection = true;
      continue;
    }

    if (inImGuiSection) {
      imguiBlob.append(line);
      imguiBlob.push_back('\n');
      continue;
    }

    std::string key;
    std::string value;
    if (!ParseIniLine(line, key, value)) continue;
    const bool truthy = (value == "true");
    for (const auto& [flagKey, member] : EditorPersistence::kWindowFlags) {
      if (key == flagKey) {
        p.*member = truthy;
        break;
      }
    }
  }

  if (!imguiBlob.empty()) {
    ImGui::LoadIniSettingsFromMemory(imguiBlob.c_str(), imguiBlob.size());
  }
}

}  // namespace

std::vector<std::string> ListPresets() {
  std::vector<std::string> names;
  const auto dir = LayoutsDir();
  if (dir.empty()) return names;

  std::error_code ec;
  if (!std::filesystem::exists(dir, ec)) return names;

  for (const auto& entry : std::filesystem::directory_iterator(dir, ec)) {
    if (ec) break;
    if (!entry.is_regular_file()) continue;
    if (entry.path().extension() != kPresetExtension) continue;
    names.push_back(entry.path().stem().string());
  }
  std::sort(names.begin(), names.end());
  return names;
}

bool SavePreset(const std::string& name, const EditorPersistence& persistence) {
  const auto path = PresetPath(name);
  if (path.empty()) {
    Logger::Error("Failed to resolve layout preset path for: " + name);
    return false;
  }

  std::error_code ec;
  std::filesystem::create_directories(path.parent_path(), ec);
  if (ec) {
    Logger::Error("Failed to create layouts directory: " + ec.message());
    return false;
  }

  std::ofstream file(path);
  if (!file.is_open()) {
    Logger::Error("Failed to open layout preset for writing: " + path.string());
    return false;
  }

  file << SerializeFlags(persistence);
  file << kImGuiSectionHeader << "\n";

  size_t imguiSize = 0;
  const char* imguiData = ImGui::SaveIniSettingsToMemory(&imguiSize);
  if (imguiData != nullptr && imguiSize > 0) {
    file.write(imguiData, static_cast<std::streamsize>(imguiSize));
  }

  Logger::Info("Saved layout preset: " + name);
  return true;
}

bool LoadPreset(const std::string& name, EditorPersistence& persistence) {
  const auto path = PresetPath(name);
  if (path.empty()) return false;

  std::ifstream file(path);
  if (!file.is_open()) {
    Logger::Warn("Layout preset not found: " + path.string());
    return false;
  }

  std::ostringstream buf;
  buf << file.rdbuf();
  ApplyParsedContent(buf.str(), persistence);
  Logger::Info("Applied layout preset: " + name);
  return true;
}

bool DeletePreset(const std::string& name) {
  const auto path = PresetPath(name);
  if (path.empty()) return false;

  std::error_code ec;
  if (!std::filesystem::remove(path, ec)) {
    Logger::Warn("Failed to delete layout preset: " + path.string());
    return false;
  }
  Logger::Info("Deleted layout preset: " + name);
  return true;
}

bool ApplyDefaultPreset(EditorPersistence& persistence) {
  ApplyParsedContent(kDefaultLayoutContents, persistence);
  Logger::Info("Applied default editor layout");
  return true;
}

bool HasImGuiIni() {
  const char* iniName = ImGui::GetIO().IniFilename;
  if (iniName == nullptr) return false;
  std::error_code ec;
  return std::filesystem::exists(iniName, ec);
}

}  // namespace octarine::editor::layouts

#endif  // OCTARINE_WITH_EDITOR
