#pragma once

#include <sol/sol.hpp>

#include <functional>
#include <vector>

// Specialize for each system that exposes a Lua surface. Each specialization MUST provide:
//   static void bind(sol::state& lua, S& system);
template <typename S>
struct LuaSystemBinding;

class LuaSystemRegistry
{
public:
    template <typename S>
    static void registerSystem(S& system)
    {
        binders().push_back([&system](sol::state& lua) { LuaSystemBinding<S>::bind(lua, system); });
    }

    static void bindAll(sol::state& lua)
    {
        for (auto& binder : binders()) binder(lua);
    }

    static void clear() { binders().clear(); }

private:
    static std::vector<std::function<void(sol::state &)>>& binders()
    {
        static std::vector<std::function<void(sol::state &)>> instance;
        return instance;
    }
};
