#include "Project/ProjectIni.h"

#include <cctype>
#include <fstream>
#include <regex>
#include <sstream>

namespace octarine::project {
namespace {
// Mirrors the CMake regex in cmake/OctarinePackage.cmake and the Gradle check in
// android/app/build.gradle. Lowercase reverse-DNS, segments separated by '.'.
const std::regex& PackageIdRegex() {
  static const std::regex re(R"(^[a-z][a-z0-9_]*(\.[a-z][a-z0-9_]*)+$)");
  return re;
}

std::string_view Trim(std::string_view s) {
  const auto is_ws = [](char c) { return c == ' ' || c == '\t' || c == '\r'; };
  while (!s.empty() && is_ws(s.front())) {
    s.remove_prefix(1);
  }
  while (!s.empty() && is_ws(s.back())) {
    s.remove_suffix(1);
  }
  return s;
}

void Assign(ProjectIni& ini, const std::string& key, std::string value) {
  if (key == "name")
    ini.name = value;
  else if (key == "package_id")
    ini.package_id = value;
  else if (key == "version_name")
    ini.version_name = value;
  else if (key == "version_code")
    ini.version_code = value;
  else if (key == "vendor")
    ini.vendor = value;
  else if (key == "description")
    ini.description = value;
  else if (key == "orientation")
    ini.orientation = value;
  else if (key == "fullscreen")
    ini.fullscreen = value;
  else if (key == "permissions")
    ini.permissions = value;
  else if (key == "category")
    ini.category = value;
  else if (key == "icon")
    ini.icon = value;
  ini.all[key] = std::move(value);
}
}  // namespace

ProjectIni ProjectIni::Parse(std::string_view text, std::vector<std::string>* parse_warnings) {
  ProjectIni ini;
  std::string line;
  size_t pos = 0;
  size_t line_no = 0;
  while (pos <= text.size()) {
    const size_t nl = text.find('\n', pos);
    const size_t end = (nl == std::string_view::npos) ? text.size() : nl;
    line.assign(text.data() + pos, end - pos);
    pos = end + 1;
    ++line_no;

    std::string_view trimmed = Trim(line);
    if (trimmed.empty() || trimmed.front() == '#') {
      if (nl == std::string_view::npos) break;
      continue;
    }

    const auto eq = trimmed.find('=');
    if (eq == std::string_view::npos) {
      if (parse_warnings) {
        std::ostringstream w;
        w << "line " << line_no << ": malformed (no '='): " << trimmed;
        parse_warnings->push_back(w.str());
      }
      if (nl == std::string_view::npos) break;
      continue;
    }

    std::string_view key_sv = Trim(trimmed.substr(0, eq));
    std::string_view val_sv = Trim(trimmed.substr(eq + 1));

    if (key_sv.empty()) {
      if (parse_warnings) {
        std::ostringstream w;
        w << "line " << line_no << ": empty key";
        parse_warnings->push_back(w.str());
      }
      if (nl == std::string_view::npos) break;
      continue;
    }

    Assign(ini, std::string(key_sv), std::string(val_sv));

    if (nl == std::string_view::npos) break;
  }
  return ini;
}

std::optional<ProjectIni> ProjectIni::Load(const std::filesystem::path& project_dir) {
  const std::filesystem::path path = project_dir / "project.ini";
  std::error_code ec;
  if (!std::filesystem::exists(path, ec)) {
    return std::nullopt;
  }
  std::ifstream f(path, std::ios::binary);
  if (!f) {
    return std::nullopt;
  }
  std::ostringstream ss;
  ss << f.rdbuf();
  ProjectIni ini = Parse(ss.str());
  ini.source_path = path;
  return ini;
}

std::vector<std::string> ProjectIni::ValidateForShipping() const {
  std::vector<std::string> errors;
  if (name.empty()) {
    errors.emplace_back("missing required key: name");
  }
  if (version_name.empty()) {
    errors.emplace_back("missing required key: version_name");
  }
  if (package_id.empty()) {
    errors.emplace_back("missing required key: package_id (Android bundle id)");
  } else if (!std::regex_match(package_id, PackageIdRegex())) {
    errors.push_back("package_id '" + package_id + "' must be reverse-DNS (e.g. com.studio.mygame)");
  }
  return errors;
}
}  // namespace octarine::project
