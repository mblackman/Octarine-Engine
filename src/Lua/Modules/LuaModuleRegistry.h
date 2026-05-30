#pragma once

#include <string>
#include <vector>

// Per-module record of the free-function globals a Lua module installs into the sol::state.
// Populated by OctarineLuaApiTest by snapshotting globals around each module's install call.
// LuaApiManifest::WriteModulesJson consumes it to emit modules.json — the machine-readable index
// agents grep for "what Lua globals does each module own."
class LuaModuleRegistry
{
public:
    struct Entry
    {
        std::string name;
        std::vector<std::string> globals;
    };

    static void registerModule(std::string name, std::vector<std::string> globals)
    {
        entries().push_back({std::move(name), std::move(globals)});
    }

    static const std::vector<Entry>& all() { return entries(); }

    static void clear() { entries().clear(); }

private:
    static std::vector<Entry>& entries()
    {
        static std::vector<Entry> instance;
        return instance;
    }
};
