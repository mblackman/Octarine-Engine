#pragma once

#include <functional>
#include <sol/sol.hpp>
#include <string>
#include <vector>

#include "ECS/Entity.h"
#include "ECS/Registry.h"
#include "Lua/Bindings/LuaBinding.h"

class LuaComponentRegistry {
 public:
  struct Entry {
    std::string luaKey;
    std::string usertypeName;
    std::function<void(Registry*, Entity, const sol::object&)> attach;
    std::function<void(sol::state&)> bindUsertype;
    std::function<void(sol::table&, Registry*)> bindRegistryAccessors;
  };

  template <typename T>
  static void registerComponent() {
    using B = LuaBinding<T>;
    Entry entry;
    entry.luaKey = B::kLuaKey;
    entry.usertypeName = B::kUsertypeName;
    entry.attach = [](Registry* r, Entity e, const sol::object& d) { r->AddComponent(e, B::fromLua(d)); };
    entry.bindUsertype = [](sol::state& lua) { B::bindUsertype(lua); };
    entry.bindRegistryAccessors = [key = std::string(B::kLuaKey)](sol::table& reg, Registry* r) {
      reg.set_function("has_" + key, [r](Entity e) { return r->HasComponent<T>(e); });
      reg.set_function("get_" + key, [r](Entity e) -> T& { return r->GetComponent<T>(e); });
    };
    entries().push_back(std::move(entry));
  }

  static const std::vector<Entry>& all() { return entries(); }

  static const Entry* find(const std::string& luaKey) {
    for (const auto& entry : entries()) {
      if (entry.luaKey == luaKey) return &entry;
    }
    return nullptr;
  }

  static void clear() { entries().clear(); }

 private:
  static std::vector<Entry>& entries() {
    static std::vector<Entry> instance;
    return instance;
  }
};
