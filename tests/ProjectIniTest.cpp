// Unit checks for the project.ini reader. gtest-free; exit code = failed-check count.
// Registered with ctest as ProjectIniTest. Mirrors the parsing tolerance + shipping-validation
// rules of cmake/OctarinePackage.cmake and android/app/build.gradle.

#include "Project/ProjectIni.h"

#include <algorithm>
#include <iostream>
#include <string>

using octarine::project::ProjectIni;

namespace
{
    int g_failures = 0;

    void Check(const bool cond, const std::string& what)
    {
        if (cond)
        {
            std::cout << "  ok   " << what << "\n";
        }
        else
        {
            std::cerr << "  FAIL " << what << "\n";
            ++g_failures;
        }
    }

    bool Contains(const std::vector<std::string>& v, const std::string& needle)
    {
        return std::any_of(v.begin(), v.end(),
                           [&](const std::string& s) { return s.find(needle) != std::string::npos; });
    }
} // namespace

int main()
{
    // Happy path: every known key parses + lands on the matching field.
    {
        const std::string body =
            "# top comment\n"
            "name = Octarine Example\n"
            "package_id = com.octarine.example\n"
            "version_name = 1.2.3\n"
            "version_code = 42\n"
            "vendor=Mateo\n"
            "description = A small example\n"
            "orientation = landscape\n"
            "fullscreen = true\n"
            "permissions = internet,recording\n"
            "category = game\n"
            "icon = assets/icon.png\n"
            "\n"
            "# trailing comment\n";

        std::vector<std::string> warns;
        const ProjectIni ini = ProjectIni::Parse(body, &warns);
        Check(warns.empty(), "no parse warnings on well-formed file");
        Check(ini.name == "Octarine Example", "name parsed");
        Check(ini.package_id == "com.octarine.example", "package_id parsed");
        Check(ini.version_name == "1.2.3", "version_name parsed");
        Check(ini.version_code == "42", "version_code parsed");
        Check(ini.vendor == "Mateo", "vendor parsed (no spaces around =)");
        Check(ini.description == "A small example", "description parsed");
        Check(ini.orientation == "landscape", "orientation parsed");
        Check(ini.fullscreen == "true", "fullscreen parsed");
        Check(ini.permissions == "internet,recording", "permissions parsed");
        Check(ini.category == "game", "category parsed");
        Check(ini.icon == "assets/icon.png", "icon parsed");

        Check(ini.ValidateForShipping().empty(), "valid project passes ValidateForShipping");
    }

    // Unknown keys preserved in `all` map but don't fall on a typed field.
    {
        const ProjectIni ini = ProjectIni::Parse("future_key = experimental\nname = X\n");
        auto it = ini.all.find("future_key");
        Check(it != ini.all.end() && it->second == "experimental", "unknown keys retained in `all`");
    }

    // Malformed line surfaces in warnings but doesn't break the parse.
    {
        std::vector<std::string> warns;
        const ProjectIni ini = ProjectIni::Parse("not an assignment\nname = OK\n", &warns);
        Check(ini.name == "OK", "subsequent lines parse after a malformed line");
        Check(!warns.empty() && Contains(warns, "malformed"), "malformed line surfaces in warnings");
    }

    // Validation: missing required keys.
    {
        const ProjectIni ini;
        const auto errs = ini.ValidateForShipping();
        Check(Contains(errs, "name"), "missing name reported");
        Check(Contains(errs, "version_name"), "missing version_name reported");
        Check(Contains(errs, "package_id"), "missing package_id reported");
    }

    // Validation: package_id must be reverse-DNS.
    {
        ProjectIni ini;
        ini.name = "X";
        ini.version_name = "1.0";
        ini.package_id = "not-a-valid-id";
        const auto errs = ini.ValidateForShipping();
        Check(Contains(errs, "reverse-DNS"), "non-reverse-DNS package_id rejected");
    }
    {
        ProjectIni ini;
        ini.name = "X";
        ini.version_name = "1.0";
        ini.package_id = "com.studio.MyGame"; // uppercase rejected by the CMake/Gradle regex too
        const auto errs = ini.ValidateForShipping();
        Check(!errs.empty(), "uppercase in package_id rejected (matches CMake/Gradle regex)");
    }
    {
        ProjectIni ini;
        ini.name = "X";
        ini.version_name = "1.0";
        ini.package_id = "com.studio.mygame";
        Check(ini.ValidateForShipping().empty(), "valid reverse-DNS package_id accepted");
    }

    return g_failures == 0 ? 0 : 1;
}
