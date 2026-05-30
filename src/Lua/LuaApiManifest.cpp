#include "Lua/LuaApiManifest.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <utility>
#include <vector>

#include "General/Logger.h"
#include "Lua/Bindings/LuaComponentRegistry.h"
#include "Lua/Modules/LuaModuleRegistry.h"

namespace
{
    struct NamedObject
    {
        std::string name;
        sol::object value;
    };

    // Sol2 leaks internal metatable bookkeeping entries (the ♻ recycle marker, and class names that
    // contain raw typeid strings like `glm::vec<2, float, glm::packed_highp>`) into the global table.
    // Those mangled names are platform-specific (MSVC vs libstdc++ stringify templates differently)
    // and would cause endless CI drift; they are also not real Lua surface. Skip anything that is
    // obviously not a valid Lua identifier path.
    bool IsSolInternalName(const std::string& name)
    {
        return name.find("\xe2\x99\xbb") != std::string::npos  // ♻ U+267B
            || name.find("::") != std::string::npos
            || name.find('<') != std::string::npos;
    }

    // Collect string-keyed entries of a table, sorted by name for stable output.
    std::vector<NamedObject> SortedEntries(const sol::table& table)
    {
        std::vector<NamedObject> entries;
        for (const auto& [key, value] : table)
        {
            if (key.get_type() != sol::type::string) continue;
            entries.push_back({key.as<std::string>(), value});
        }
        std::sort(entries.begin(), entries.end(),
                  [](const NamedObject& a, const NamedObject& b) { return a.name < b.name; });
        return entries;
    }

    void EmitScalar(std::ofstream& out, const std::string& name, const sol::object& value)
    {
        switch (value.get_type())
        {
            case sol::type::number:
                out << "---@type number\n" << name << " = 0\n\n";
                break;
            case sol::type::boolean:
                out << "---@type boolean\n" << name << " = " << (value.as<bool>() ? "true" : "false") << "\n\n";
                break;
            case sol::type::string:
                out << "---@type string\n" << name << " = \"\"\n\n";
                break;
            default:
                out << "-- " << name << " (unbound-introspectable type)\n\n";
                break;
        }
    }

    // Functions bound from C++ carry no Lua type info, so parameters are emitted as `...`.
    void EmitFunction(std::ofstream& out, const std::string& qualifiedName)
    {
        out << "function " << qualifiedName << "(...) end\n\n";
    }

    // A table or usertype: emit it as a namespace, then its function members as `table.fn`.
    // One level deep — engine tables (registry, input) and usertypes are flat.
    void EmitTable(std::ofstream& out, const std::string& name, const sol::table& table)
    {
        out << "---@class " << name << "\n" << name << " = {}\n\n";
        for (const auto& member : SortedEntries(table))
        {
            if (member.name.rfind("__", 0) == 0) continue; // skip metamethods (__index, ...)
            if (IsSolInternalName(member.name)) continue;
            if (member.value.get_type() == sol::type::function)
            {
                EmitFunction(out, name + "." + member.name);
            }
            else
            {
                out << "---@field " << member.name << " any   -- on " << name << "\n";
            }
        }
        out << "\n";
    }
} // namespace

namespace LuaApiManifest
{
    std::unordered_set<std::string> SnapshotGlobals(sol::state& lua)
    {
        std::unordered_set<std::string> names;
        for (const auto& [key, value] : lua.globals())
        {
            (void)value;
            if (key.get_type() == sol::type::string) names.insert(key.as<std::string>());
        }
        return names;
    }

    bool Write(sol::state& lua, const std::unordered_set<std::string>& before, const std::string& outPath)
    {
        std::ofstream out(outPath);
        if (!out.is_open())
        {
            Logger::Error("LuaApiManifest: could not open '" + outPath + "' for writing.");
            return false;
        }

        // Everything the engine added since the snapshot — the standard library is filtered out
        // because those names were already present in `before`.
        std::vector<NamedObject> added;
        for (const auto& [key, value] : lua.globals())
        {
            if (key.get_type() != sol::type::string) continue;
            const std::string name = key.as<std::string>();
            if (before.contains(name)) continue;
            if (IsSolInternalName(name)) continue;
            added.push_back({name, value});
        }
        std::sort(added.begin(), added.end(),
                  [](const NamedObject& a, const NamedObject& b) { return a.name < b.name; });

        out << "-- AUTO-GENERATED Octarine Lua API surface. Do not edit by hand.\n"
            << "-- Introspected from the live sol::state after all bindings install.\n"
            << "-- Regenerate by setting OCTARINE_DUMP_LUA_API=<path> and launching the engine.\n"
            << "---@meta\n"
            << "---@diagnostic disable: lowercase-global, missing-return\n\n";

        for (const auto& entry : added)
        {
            switch (entry.value.get_type())
            {
                case sol::type::function:
                    EmitFunction(out, entry.name);
                    break;
                case sol::type::table:
                    // Named usertypes register as iterable tables of their members.
                    EmitTable(out, entry.name, entry.value.as<sol::table>());
                    break;
                case sol::type::userdata:
                    // A bound instance/handle — members aren't safely iterable; emit a bare class.
                    out << "---@class " << entry.name << "\n" << entry.name << " = {}\n\n";
                    break;
                default:
                    EmitScalar(out, entry.name, entry.value);
                    break;
            }
        }

        Logger::Info("LuaApiManifest: wrote " + std::to_string(added.size()) + " entries to " + outPath);
        return true;
    }

    bool MaybeDumpFromEnv(sol::state& lua, const std::unordered_set<std::string>& before)
    {
#ifdef _MSC_VER
        char* env = nullptr;
        size_t len = 0;
        const bool hasEnv = _dupenv_s(&env, &len, "OCTARINE_DUMP_LUA_API") == 0 && env != nullptr;
        const std::string value = hasEnv ? std::string(env) : std::string();
        free(env);
        if (value.empty()) return false;
#else
        const char* env = std::getenv("OCTARINE_DUMP_LUA_API");
        if (env == nullptr || env[0] == '\0') return false;
        const std::string value(env);
#endif
        const std::string outPath = (value == "1") ? "lua_api.lua" : value;
        return Write(lua, before, outPath);
    }

    namespace
    {
        // Minimal JSON string escaping. Component/module identifiers and global names are plain
        // ASCII Lua keys so this only ever needs to deal with the standard control chars; spelling
        // out a few cases beats pulling in a JSON dep just for this manifest.
        void WriteJsonString(std::ofstream& out, const std::string& s)
        {
            out << '"';
            for (const char c : s)
            {
                switch (c)
                {
                    case '"': out << "\\\""; break;
                    case '\\': out << "\\\\"; break;
                    case '\n': out << "\\n"; break;
                    case '\r': out << "\\r"; break;
                    case '\t': out << "\\t"; break;
                    default:
                        if (static_cast<unsigned char>(c) < 0x20)
                        {
                            char buf[8];
                            std::snprintf(buf, sizeof(buf), "\\u%04x", c);
                            out << buf;
                        }
                        else
                        {
                            out << c;
                        }
                }
            }
            out << '"';
        }
    } // namespace

    bool WriteComponentsJson(const std::string& outPath)
    {
        std::ofstream out(outPath);
        if (!out.is_open())
        {
            Logger::Error("LuaApiManifest: could not open '" + outPath + "' for writing.");
            return false;
        }

        // Snapshot entries and sort by lua_key for stable output.
        auto entries = LuaComponentRegistry::all();
        std::sort(entries.begin(), entries.end(),
                  [](const auto& a, const auto& b) { return a.luaKey < b.luaKey; });

        out << "{\n  \"_generated_by\": \"OctarineLuaApiTest via LuaApiManifest::WriteComponentsJson\",\n";
        out << "  \"components\": [\n";
        for (std::size_t i = 0; i < entries.size(); ++i)
        {
            const auto& e = entries[i];
            out << "    {\n      \"lua_key\": ";
            WriteJsonString(out, e.luaKey);
            out << ",\n      \"usertype\": ";
            WriteJsonString(out, e.usertypeName);
            out << ",\n      \"has_accessor\": ";
            WriteJsonString(out, "registry.has_" + e.luaKey);
            out << ",\n      \"get_accessor\": ";
            WriteJsonString(out, "registry.get_" + e.luaKey);
            out << "\n    }";
            if (i + 1 < entries.size()) out << ",";
            out << "\n";
        }
        out << "  ]\n}\n";

        Logger::Info("LuaApiManifest: wrote " + std::to_string(entries.size()) + " components to " + outPath);
        return true;
    }

    bool WriteModulesJson(const std::string& outPath)
    {
        std::ofstream out(outPath);
        if (!out.is_open())
        {
            Logger::Error("LuaApiManifest: could not open '" + outPath + "' for writing.");
            return false;
        }

        // Preserve install order — that order matters for setup-ordering gotchas, so capturing it
        // makes modules.json useful when diagnosing "what installed before what."
        const auto& modules = LuaModuleRegistry::all();

        out << "{\n  \"_generated_by\": \"OctarineLuaApiTest via LuaApiManifest::WriteModulesJson\",\n";
        out << "  \"modules\": [\n";
        for (std::size_t i = 0; i < modules.size(); ++i)
        {
            const auto& m = modules[i];
            out << "    {\n      \"name\": ";
            WriteJsonString(out, m.name);
            out << ",\n      \"binding_header\": ";
            WriteJsonString(out, "src/Lua/Modules/" + m.name + "ModuleLuaBinding.h");
            out << ",\n      \"globals\": [";
            // Sort globals within a module for stable output (module order is install order; globals within are unordered).
            std::vector<std::string> globals = m.globals;
            std::sort(globals.begin(), globals.end());
            for (std::size_t g = 0; g < globals.size(); ++g)
            {
                if (g > 0) out << ", ";
                WriteJsonString(out, globals[g]);
            }
            out << "]\n    }";
            if (i + 1 < modules.size()) out << ",";
            out << "\n";
        }
        out << "  ]\n}\n";

        Logger::Info("LuaApiManifest: wrote " + std::to_string(modules.size()) + " modules to " + outPath);
        return true;
    }
} // namespace LuaApiManifest
