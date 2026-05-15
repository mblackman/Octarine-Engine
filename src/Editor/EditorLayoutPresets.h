#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <string>
#include <vector>

struct EditorPersistence;

namespace octarine::editor::layouts {

std::vector<std::string> ListPresets();

bool SavePreset(const std::string& name, const EditorPersistence& persistence);

bool LoadPreset(const std::string& name, EditorPersistence& persistence);

bool DeletePreset(const std::string& name);

bool ApplyDefaultPreset(EditorPersistence& persistence);

bool HasImGuiIni();

}  // namespace octarine::editor::layouts

#endif  // OCTARINE_WITH_EDITOR
