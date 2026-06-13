#pragma once

#include <sol/sol.hpp>
#include <string>

// Serializes a Lua table to a loadable Lua literal (the body after `return `). Backs
// storage.write_table / project.write_table; the inverse is a plain load() of the file with an
// empty environment (see StorageModuleLuaBinding), so the emitted text must be pure data — no
// identifiers, no library calls.
//
// Supported: nested tables, strings, numbers (integer subtype preserved; non-finite values emit
// the env-free expressions 1/0, -1/0, 0/0), booleans. Keys may be numbers, strings, or booleans.
// Functions, userdata, threads, and cyclic tables are errors — the caller gets false + a message
// rather than a silently incomplete save.
//
// Output is deterministic (number keys ascending, then string keys lexicographic, then false,
// true) so emitted files diff cleanly under version control — project.write_table output is
// expected to be committed to game repos.
namespace lua_table_serializer {

// Appends the serialized form of `table` to `out`. Returns false on unsupported types, cycles,
// or nesting deeper than an internal sanity bound, with a human-readable reason in *error.
bool Serialize(const sol::table& table, std::string& out, std::string* error);

}  // namespace lua_table_serializer
