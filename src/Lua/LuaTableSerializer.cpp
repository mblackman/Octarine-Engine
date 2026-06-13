#include "Lua/LuaTableSerializer.h"

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <unordered_set>
#include <utility>
#include <vector>

#include "General/LuaEscape.h"

namespace {

// Deep saves are fine; anything past this is almost certainly a cycle the visited-set missed
// (e.g. through metatables) or runaway generated data.
constexpr int kMaxDepth = 64;

// Format a Lua number preserving the integer/float subtype split. Detected via lua_isinteger on
// the actual stack value — sol's is<int64_t> conversion rules vary with safety config, the VM's
// answer doesn't. Non-finite floats have no Lua literal, so emit constant expressions that need
// no environment.
std::string NumberToLua(const sol::object& obj) {
  lua_State* state = obj.lua_state();
  obj.push(state);
  const bool isInteger = lua_isinteger(state, -1) != 0;
  const lua_Integer intValue = isInteger ? lua_tointeger(state, -1) : 0;
  const lua_Number numValue = isInteger ? 0 : lua_tonumber(state, -1);
  lua_pop(state, 1);

  if (isInteger) {
    return std::to_string(static_cast<long long>(intValue));
  }
  // Classify non-finite values via the IEEE-754 bit pattern, not std::isinf/isnan — the release
  // build compiles with -ffast-math, under which those predicates may constant-fold to false and
  // let "inf"/"nan" (not valid Lua) leak into the output.
  const auto value = static_cast<double>(numValue);
  uint64_t bits = 0;
  static_assert(sizeof(bits) == sizeof(value));
  std::memcpy(&bits, &value, sizeof(bits));
  constexpr uint64_t kExponentMask = 0x7ff0000000000000ULL;
  constexpr uint64_t kMantissaMask = 0x000fffffffffffffULL;
  constexpr int kSignBitShift = 63;
  if ((bits & kExponentMask) == kExponentMask) {
    if ((bits & kMantissaMask) != 0) return "0/0";         // NaN
    return (bits >> kSignBitShift) != 0 ? "-1/0" : "1/0";  // +/- infinity
  }
  constexpr int kBufSize = 32;  // %.17g worst case is 24 chars + nul
  char buf[kBufSize];
  std::snprintf(buf, sizeof(buf), "%.17g", value);
  return buf;
}

std::string KeyToLua(const sol::object& key, std::string* error) {
  switch (key.get_type()) {
    case sol::type::number:
      return "[" + NumberToLua(key) + "]";
    case sol::type::string:
      return "[\"" + EscapeLua(key.as<std::string>()) + "\"]";
    case sol::type::boolean:
      return key.as<bool>() ? "[true]" : "[false]";
    default:
      *error = "unsupported key type: " + std::string(sol::type_name(key.lua_state(), key.get_type()));
      return {};
  }
}

// Gather the table's keys in deterministic emission order: numbers ascending, then strings
// lexicographic, then false, true. pairs() order is unspecified, and write_table output should
// diff cleanly in git. Returns false (with *error set) on an unsupported key type.
bool CollectOrderedKeys(const sol::table& table, std::vector<sol::object>& orderedKeys, std::string* error) {
  std::vector<std::pair<double, sol::object>> numberKeys;
  std::vector<std::pair<std::string, sol::object>> stringKeys;
  std::vector<sol::object> boolKeys;
  for (const auto& [key, unused] : table) {
    switch (key.get_type()) {
      case sol::type::number:
        numberKeys.emplace_back(key.as<double>(), key);
        break;
      case sol::type::string:
        stringKeys.emplace_back(key.as<std::string>(), key);
        break;
      case sol::type::boolean:
        boolKeys.push_back(key);
        break;
      default:
        KeyToLua(key, error);  // sets *error
        return false;
    }
  }
  std::sort(numberKeys.begin(), numberKeys.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
  std::sort(stringKeys.begin(), stringKeys.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
  std::sort(boolKeys.begin(), boolKeys.end(),
            [](const sol::object& a, const sol::object& b) { return !a.as<bool>() && b.as<bool>(); });

  orderedKeys.reserve(numberKeys.size() + stringKeys.size() + boolKeys.size());
  for (auto& [unused, key] : numberKeys) orderedKeys.push_back(std::move(key));
  for (auto& [unused, key] : stringKeys) orderedKeys.push_back(std::move(key));
  for (auto& key : boolKeys) orderedKeys.push_back(std::move(key));
  return true;
}

bool SerializeValue(const sol::object& value, std::string& out, std::string* error, int depth,
                    std::unordered_set<const void*>& visited);

// Mutually recursive with SerializeValue by design — tables nest. Bounded by kMaxDepth and the
// visited set, so the recursion cannot run away.
// NOLINTNEXTLINE(misc-no-recursion)
bool SerializeTable(const sol::table& table, std::string& out, std::string* error, const int depth,
                    std::unordered_set<const void*>& visited) {
  if (depth > kMaxDepth) {
    *error = "table nesting exceeds " + std::to_string(kMaxDepth) + " levels";
    return false;
  }
  if (!visited.insert(table.pointer()).second) {
    *error = "cyclic table reference";
    return false;
  }

  std::vector<sol::object> orderedKeys;
  if (!CollectOrderedKeys(table, orderedKeys, error)) return false;

  const std::string indent(static_cast<size_t>(depth + 1) * 2, ' ');
  out += "{";
  bool first = true;
  for (const auto& key : orderedKeys) {
    const std::string keyText = KeyToLua(key, error);
    if (keyText.empty()) return false;
    out += first ? "\n" : ",\n";
    first = false;
    out += indent;
    out += keyText;
    out += " = ";
    if (!SerializeValue(table.get<sol::object>(key), out, error, depth + 1, visited)) return false;
  }
  if (!first) {
    out += "\n";
    out += std::string(static_cast<size_t>(depth) * 2, ' ');
  }
  out += "}";

  visited.erase(table.pointer());
  return true;
}

// NOLINTNEXTLINE(misc-no-recursion) — see SerializeTable.
bool SerializeValue(const sol::object& value, std::string& out, std::string* error, const int depth,
                    std::unordered_set<const void*>& visited) {
  switch (value.get_type()) {
    case sol::type::table:
      return SerializeTable(value.as<sol::table>(), out, error, depth, visited);
    case sol::type::string:
      out += "\"" + EscapeLua(value.as<std::string>()) + "\"";
      return true;
    case sol::type::number:
      out += NumberToLua(value);
      return true;
    case sol::type::boolean:
      out += value.as<bool>() ? "true" : "false";
      return true;
    default:
      *error = "unsupported value type: " + std::string(sol::type_name(value.lua_state(), value.get_type()));
      return false;
  }
}

}  // namespace

namespace lua_table_serializer {

bool Serialize(const sol::table& table, std::string& out, std::string* error) {
  std::string localError;
  if (error == nullptr) error = &localError;
  std::unordered_set<const void*> visited;
  return SerializeTable(table, out, error, 0, visited);
}

}  // namespace lua_table_serializer
