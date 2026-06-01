#pragma once

// C++ reader for the game project's `project.ini` — third corner alongside the CMake parser
// (`cmake/OctarinePackage.cmake :: octarine_read_project_ini`) and the Gradle parser
// (`android/app/build.gradle :: java.util.Properties`). Used by the editor to surface project
// identity in panels and to validate before Export Build spawns a packaging script.
//
// Schema matches the other two parsers; validation mirrors the shipping-build identity rules
// (required: name, version_name, package_id; package_id must be reverse-DNS).

#include <filesystem>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace octarine::project {
struct ProjectIni {
  // Required for shipping builds.
  std::string name;
  std::string package_id;
  std::string version_name;

  // Optional identity / packaging knobs.
  std::string version_code;
  std::string vendor;
  std::string description;
  std::string orientation;
  std::string fullscreen;
  std::string permissions;
  std::string category;
  std::string icon;

  // Every parsed key (including the named ones above), preserved verbatim so callers can
  // read keys the editor doesn't know about yet.
  std::unordered_map<std::string, std::string> all;

  // Source path the file was loaded from (empty for in-memory parses).
  std::filesystem::path source_path;

  // Loads + parses `<dir>/project.ini`. Returns nullopt if the file doesn't exist; an empty
  // ProjectIni if the file exists but is empty.
  static std::optional<ProjectIni> Load(const std::filesystem::path& project_dir);

  // Parses raw INI text. Malformed lines produce an entry in `parse_warnings`. Comments
  // (`#`) and blank lines are skipped.
  static ProjectIni Parse(std::string_view text, std::vector<std::string>* parse_warnings = nullptr);

  // Validates identity for a shipping build (mirrors the CMake/Gradle policy):
  //   - name, version_name, package_id must be non-empty
  //   - package_id must match `^[a-z][a-z0-9_]*(\.[a-z][a-z0-9_]*)+$`
  // Returns the list of error messages; empty = valid. Caller decides whether to surface
  // the errors as a warning (dev build) or a fatal (shipping/export build).
  std::vector<std::string> ValidateForShipping() const;
};
}  // namespace octarine::project
