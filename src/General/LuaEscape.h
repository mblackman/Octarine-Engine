#pragma once

#include <string>

// Escape a string for embedding inside a Lua double-quoted literal. Control characters (< 0x20
// or DEL) use \xNN hex escapes; backslash and double-quote are prefixed with a backslash. Lua
// 5.1+ parses \xNN in string literals. Shared by the asset-manifest emitter (AssetCatalog) and
// the Lua table serializer behind storage.write_table — both emit Lua source from untrusted
// strings, so the escaping rules must not drift apart.
std::string EscapeLua(const std::string& s);
